/**************************************
Exercise: 	DS - TRIE
Date:	    27/8/2025
Developer:	Tal Hindi
Reviewer: 	Daniel Gamliel
Status:		
**************************************/

#include <stdlib.h> /* malloc */

#include "trie.h" /* TRIECreate */

#define RIGHT 1
#define LEFT 0

#define MAX_PATH_LENGTH 32
#define BIT_SHIFT_ONE 1
#define LEAF_NODE 1
#define NON_LEAF_NODE 0
#define HAS_FREE_SPACE 1
#define NO_FREE_SPACE 0

#define LEFT_CHILD(node) ((node)->children[LEFT])
#define RIGHT_CHILD(node) ((node)->children[RIGHT])
#define HAS_LEFT_CHILD(node) (LEFT_CHILD(node) != NULL)
#define HAS_RIGHT_CHILD(node) (RIGHT_CHILD(node) != NULL)
#define HAS_AVAILABLE_CHILD_SLOT(node) (!HAS_LEFT_CHILD(node) || !HAS_RIGHT_CHILD(node))
#define HAS_FREE_IN_SUBTREE(node) ((node)->has_free_in_subtree)
#define IS_LEAF(node) ((node)->is_leaf)

struct trie_node
{
    trie_node_t* children[2];
    int is_leaf;
    int has_free_in_subtree;
};

/* ======================================= PROTOTYPES ======================================= */
static int IsBitSet(unsigned int value, size_t bit_index);
static trie_node_t* TRIECreateNode(void);
static trie_node_t* TraversePath(trie_node_t* root, unsigned int ip_address, size_t depth);
static trie_status_e TRIEInsertHelper(trie_node_t* root, unsigned int* allocated_ip, 
                                     unsigned int requested_ip, size_t depth);
static trie_status_e AllocateAtTarget(trie_node_t* current_node, trie_node_t** path_nodes,
                                     int path_count, unsigned int target_ip, size_t remaining_depth, unsigned int* allocated_ip);
static trie_status_e FindAndAllocateNextFree(trie_node_t* root, unsigned int base_ip, 
                                           unsigned int* allocated_ip, size_t depth);
static void TRIEUpdateMetadata(trie_node_t** path_nodes, int path_count);
static int HasChildWithFreeSpace(trie_node_t* node);                           

/* ======================================= API IMP ======================================= */
trie_node_t* TRIECreate()
{
    trie_node_t* root = (trie_node_t*)malloc(sizeof(trie_node_t));
    
    if(!root)
    {
        return NULL;
    }

    root->is_leaf = NON_LEAF_NODE;
    root->has_free_in_subtree = HAS_FREE_SPACE;
    root->children[RIGHT] = NULL;
    root->children[LEFT] = NULL;

    return root;
}

trie_status_e TRIEInsert(trie_node_t* root, unsigned int* allocated_ip, unsigned int requested_ip, size_t depth)
{
    if(!root)
    {
        return TRIE_ALLOC_FAILURE;
    }
    
    if(!HAS_FREE_IN_SUBTREE(root))
    {
        return TRIE_NO_IP_AVAILABLE;
    }
    
    return TRIEInsertHelper(root, allocated_ip, requested_ip, depth);
}

int TRIESearch(trie_node_t* root, unsigned int ip_address, size_t depth)
{
    trie_node_t* target = TraversePath(root, ip_address, depth);
    
    if(!target)
    {
        return NO_FREE_SPACE;
    }

    return IS_LEAF(target);
}

trie_status_e TRIEFree(trie_node_t* root, unsigned int ip_address, size_t depth)
{
    trie_node_t* target = NULL;
    trie_node_t* path_nodes[MAX_PATH_LENGTH];
    int path_count = 0;
    trie_node_t* current_node = root;
    int bit_position = 0;
    
    target = TraversePath(root, ip_address, depth);
    
    if(!target)
    {
        return TRIE_IP_NOT_FOUND;
    }
    
    if(!IS_LEAF(target))
    {
        return TRIE_DOUBLE_FREE;
    }
    
    target->is_leaf = NON_LEAF_NODE;
    
    for(bit_position = (int)depth - 1; bit_position >= 0; --bit_position)
    {
        path_nodes[path_count++] = current_node;
        current_node = IsBitSet(ip_address, bit_position) ? RIGHT_CHILD(current_node) : LEFT_CHILD(current_node);
    }
    
    TRIEUpdateMetadata(path_nodes, path_count);
    
    return TRIE_SUCCESS;
}

void TRIEDestroy(trie_node_t* root)
{
    if(NULL == root)
    {
        return;
    }

    TRIEDestroy(LEFT_CHILD(root));
    TRIEDestroy(RIGHT_CHILD(root));

    free(root);
}

size_t TRIECountFree(trie_node_t* root, size_t depth)
{
    if(!root || depth == 0)
    {
        return root && !IS_LEAF(root) ? 1 : 0;
    }
    
    return (HAS_LEFT_CHILD(root) ? TRIECountFree(LEFT_CHILD(root), depth - 1) : (size_t)(1 << (depth - 1))) +
           (HAS_RIGHT_CHILD(root) ? TRIECountFree(RIGHT_CHILD(root), depth - 1) : (size_t)(1 << (depth - 1))) -
           (IS_LEAF(root) ? 1 : 0);
}

static int IsBitSet(unsigned int value, size_t bit_index)
{
	return (value >> bit_index) & 1;
}

static trie_node_t* TRIECreateNode(void)
{
    trie_node_t* new_node = (trie_node_t*)malloc(sizeof(trie_node_t));
    if(!new_node)
    {
        return NULL;
    } 

    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->is_leaf = NON_LEAF_NODE;
    new_node->has_free_in_subtree = HAS_FREE_SPACE;

    return new_node;
}

static trie_status_e TRIEInsertHelper(trie_node_t* root, unsigned int* allocated_ip, 
                                        unsigned int requested_ip, size_t depth)
{
    trie_node_t* current_node = root;
    trie_node_t* path_nodes[MAX_PATH_LENGTH];
    trie_node_t* pivot_node = NULL;
    int path_count = 0;
    int pivot_depth = -1;
    int bit_position = 0;
    
    for(bit_position = (int)depth - 1; bit_position >= 0; --bit_position)
    {
        int direction = IsBitSet(requested_ip, bit_position);
        trie_node_t* child = current_node->children[direction];
        
        path_nodes[path_count++] = current_node;
        
        if(!direction && current_node->children[RIGHT] && HAS_FREE_IN_SUBTREE(current_node->children[RIGHT]))
        {
            pivot_node = current_node;
            pivot_depth = bit_position;
        }
        
        if(!child)
        {
            return AllocateAtTarget(current_node, path_nodes, path_count, requested_ip, bit_position + 1, allocated_ip);
        }
        
        if(!HAS_FREE_IN_SUBTREE(child))
        {
            return FindAndAllocateNextFree(pivot_node, requested_ip, allocated_ip, pivot_depth);
        }
        
        current_node = child;
    }
    
    if(IS_LEAF(current_node))
    {
        return FindAndAllocateNextFree(pivot_node, requested_ip, allocated_ip, pivot_depth);
    }
    
    current_node->is_leaf = LEAF_NODE;
    *allocated_ip = requested_ip;
    TRIEUpdateMetadata(path_nodes, path_count);
    return TRIE_SUCCESS;
}

static trie_status_e AllocateAtTarget(trie_node_t* current_node, trie_node_t** path_nodes,
                                     int path_count, unsigned int target_ip, size_t remaining_depth, unsigned int* allocated_ip)
{
    trie_node_t* traverse_node = current_node;
    int bit_position = 0;
    
    for(bit_position = (int)remaining_depth - 1; bit_position >= 0; --bit_position)
    {
        int direction = IsBitSet(target_ip, bit_position);
        
        if(!traverse_node->children[direction])
        {
            traverse_node->children[direction] = TRIECreateNode();
            if(!traverse_node->children[direction])
            {
                return TRIE_ALLOC_FAILURE;
            }
        }
        traverse_node = traverse_node->children[direction];
    }
    
    traverse_node->is_leaf = LEAF_NODE;
    *allocated_ip = target_ip;
    TRIEUpdateMetadata(path_nodes, path_count);
    
    return TRIE_SUCCESS;
}

static trie_status_e FindAndAllocateNextFree(trie_node_t* pivot_node, unsigned int base_ip, 
                                           unsigned int* allocated_ip, size_t pivot_depth)
{
    unsigned int next_ip = 0;
    trie_status_e status = TRIE_SUCCESS;
    
    if(!pivot_node)
    {
        return TRIE_NO_IP_AVAILABLE;
    }
    
    next_ip = base_ip | (BIT_SHIFT_ONE << pivot_depth);
    status = TRIEInsertHelper(pivot_node, allocated_ip, next_ip, pivot_depth + 1);
    
    return (status == TRIE_SUCCESS) ? TRIE_IP_OCCUPIED_ALLOCATED_ANOTHER : status;
}

static trie_node_t* TraversePath(trie_node_t* root, unsigned int ip_address, size_t depth)
{
    trie_node_t* current_node = root;
    int bit_position = 0;
    
    for (bit_position = (int)depth - 1; bit_position >= 0; --bit_position) 
    {
        int direction = IsBitSet(ip_address, bit_position);
        
        if(!current_node->children[direction])
        {
            return NULL;
        }
        current_node = current_node->children[direction];
    }
    
    return current_node;
}

static void TRIEUpdateMetadata(trie_node_t** path_nodes, int path_count)
{
    int level = 0;
    trie_node_t* current_node = NULL;
    
    for(level = path_count - 1; level >= 0; --level)
    {
        current_node = path_nodes[level];
        
        current_node->has_free_in_subtree = !IS_LEAF(current_node) ||
                                           HAS_AVAILABLE_CHILD_SLOT(current_node) ||
                                           HasChildWithFreeSpace(current_node);
    }
}

static int HasChildWithFreeSpace(trie_node_t* node)
{
    return (HAS_LEFT_CHILD(node) && HAS_FREE_IN_SUBTREE(LEFT_CHILD(node))) ||
           (HAS_RIGHT_CHILD(node) && HAS_FREE_IN_SUBTREE(RIGHT_CHILD(node)));
}

