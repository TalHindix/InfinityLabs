/**************************************
Exercise: 	DS - TRIE
Date:	    27/8/2025
Developer:	Tal Hindi
Reviewer: 	Daniel Gamliel
Status:		
**************************************/

#include <stdlib.h> /* malloc */

#include "trie.h" /* TRIECreate */

#define BIN 2
#define RIGHT 1
#define LEFT 0

#define MAX_DEPTH 32
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
    trie_node_t* children[BIN];
    int is_leaf;
    int has_free_in_subtree;
};

/* ======================================= PROTOTYPES ======================================= */
static int IsBitSet(unsigned int value, size_t bit_index);
static trie_node_t* TRIECreateNode(void);
static trie_status_e TRIECreatePath(trie_node_t* root, unsigned int ip_address, size_t depth);
static int TRIEFindNextFree(trie_node_t* root, unsigned int start_ip, 
                           unsigned int* found_ip, size_t depth);
static void TRIEUpdateMetadata(trie_node_t* root, unsigned int ip_address, size_t depth);
static trie_node_t* TraversePath(trie_node_t* root, unsigned int ip_address, size_t depth);
static trie_status_e InsertAndAllocate(trie_node_t* root, unsigned int* allocated_ip, unsigned int ip_address, size_t depth);
static int HasChildWithFreeSpace(trie_node_t* node);                           

/* ======================================= API IMP ======================================= */
trie_node_t* TRIECreate()
{
    trie_node_t* root = NULL;
    size_t i = 0;
    
    root = (trie_node_t*)malloc(sizeof(trie_node_t));
    if(!root)
    {
        return NULL;
    }

    root->is_leaf = NON_LEAF_NODE;
    root->has_free_in_subtree = HAS_FREE_SPACE;

    for(i = 0; i < BIN; ++i)
    {
        root->children[i] = NULL;
    }


    return root;
}

trie_status_e TRIEInsert(trie_node_t* root, unsigned int* allocated_ip, unsigned int requested_ip, size_t depth)
{
    unsigned int found_ip = 0;
    trie_status_e status = TRIE_SUCCESS;
    
    if(!root)
    {
        return TRIE_ALLOC_FAILURE;
    }
    
    if(!HAS_FREE_IN_SUBTREE(root))
    {
        return TRIE_NO_IP_AVAILABLE;
    }
    
    if(!TRIESearch(root, requested_ip, depth))
    {
        return InsertAndAllocate(root, allocated_ip, requested_ip, depth);
    }
    
    if(TRIEFindNextFree(root, requested_ip + 1, &found_ip, depth))
    {
        status = InsertAndAllocate(root, allocated_ip, found_ip, depth);
        return (status == TRIE_SUCCESS) ? TRIE_IP_OCCUPIED_ALLOCATED_ANOTHER : status;
    }
    
    return TRIE_NO_IP_AVAILABLE;
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
    trie_node_t* target = TraversePath(root, ip_address, depth);
    
    if(!target)
    {
        return TRIE_IP_NOT_FOUND;
    }
    
    if(!IS_LEAF(target))
    {
        return TRIE_DOUBLE_FREE;
    }
    
    target->is_leaf = NON_LEAF_NODE;
    TRIEUpdateMetadata(root, ip_address, depth);
    
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
    size_t free_count = 0;
    unsigned int max_ip = (BIT_SHIFT_ONE << depth) - BIT_SHIFT_ONE;
    unsigned int ip_address = 0;
    
    for (ip_address = 0; ip_address <= max_ip; ip_address++) 
    {
        if (!TRIESearch(root, ip_address, depth))
        {
            free_count++;
        }
    }
    
    return free_count;
}

static int IsBitSet(unsigned int value, size_t bit_index)
{
	return (value & (BIT_SHIFT_ONE << bit_index)) ? BIT_SHIFT_ONE : NO_FREE_SPACE;
}

static trie_node_t* TRIECreateNode(void)
{
    trie_node_t* new_node = (trie_node_t*)malloc(sizeof(trie_node_t));
    if(!new_node)
    {
        return NULL;
    } 

    RIGHT_CHILD(new_node) = NULL;
    LEFT_CHILD(new_node) = NULL;
    new_node->is_leaf = NON_LEAF_NODE;
    new_node->has_free_in_subtree = HAS_FREE_SPACE;

    return new_node;
}

static trie_status_e TRIECreatePath(trie_node_t* root, unsigned int ip_address, size_t depth)
{
    trie_node_t* current_node = root;
    trie_node_t* path[MAX_PATH_LENGTH];
    int path_length = 0;
    int bit_position = 0;
    
    for (bit_position = (int)depth - 1; bit_position >= 0; --bit_position) 
    {
        path[path_length++] = current_node;
        
        /* Go Right */
        if(IsBitSet(ip_address,bit_position))
        {
            if(!HAS_RIGHT_CHILD(current_node))
            {
                RIGHT_CHILD(current_node) = TRIECreateNode();
                if(!HAS_RIGHT_CHILD(current_node))
                {
                    /* Rollback: free any nodes created in this call */
                    while(--path_length > 0)
                    {
                        trie_node_t* parent = path[path_length];
                        if(IsBitSet(ip_address, (int)depth - 1 - path_length))
                        {
                            free(RIGHT_CHILD(parent));
                            RIGHT_CHILD(parent) = NULL;
                        }
                        else
                        {
                            free(LEFT_CHILD(parent));
                            LEFT_CHILD(parent) = NULL;
                        }
                    }
                    return TRIE_ALLOC_FAILURE;
                }

            }
            current_node = RIGHT_CHILD(current_node);
        }
        else /* Go Left*/
        {
            if(!HAS_LEFT_CHILD(current_node))
            {
                LEFT_CHILD(current_node) = TRIECreateNode();
                if(!HAS_LEFT_CHILD(current_node))
                {
                    /* Rollback: free any nodes created in this call */
                    while(--path_length > 0)
                    {
                        trie_node_t* parent = path[path_length];
                        if(IsBitSet(ip_address, (int)depth - 1 - path_length))
                        {
                            free(RIGHT_CHILD(parent));
                            RIGHT_CHILD(parent) = NULL;
                        }
                        else
                        {
                            free(LEFT_CHILD(parent));
                            LEFT_CHILD(parent) = NULL;
                        }
                    }
                    return TRIE_ALLOC_FAILURE;
                }
            }
            current_node = LEFT_CHILD(current_node);
        }
    }

    current_node->is_leaf = LEAF_NODE;

    return TRIE_SUCCESS;
}

static int TRIEFindNextFree(trie_node_t* root, unsigned int start_ip, 
                           unsigned int* found_ip, size_t depth)
{
    trie_node_t* current_node = root;
    unsigned int current_ip = start_ip;
    unsigned int max_ip = (BIT_SHIFT_ONE << depth) - BIT_SHIFT_ONE;
    int bit_position = 0;
    
    if(NULL == root || !HAS_FREE_IN_SUBTREE(root))
    {
        return NO_FREE_SPACE;
    }
    
    while(current_ip <= max_ip)
    {
        current_node = root;
        
        for(bit_position = (int)depth - 1; bit_position >= 0; --bit_position)
        {
            if(IsBitSet(current_ip, bit_position))
            {
                if(!HAS_RIGHT_CHILD(current_node))
                {
                    break;
                }
                current_node = RIGHT_CHILD(current_node);
            }
            else
            {
                if(!HAS_LEFT_CHILD(current_node))
                {
                    break;
                }
                current_node = LEFT_CHILD(current_node);
            }
        }
        
        if(bit_position < 0 && !IS_LEAF(current_node))
        {
            *found_ip = current_ip;
            return BIT_SHIFT_ONE;
        }
        
        if(current_ip == max_ip)
        {
            break;
        }
        current_ip++;
    }
    
    return NO_FREE_SPACE;
}

static void TRIEUpdateMetadata(trie_node_t* root, unsigned int ip_address, size_t depth)
{
    trie_node_t* current_node = root;
    trie_node_t* path[MAX_PATH_LENGTH];
    int path_length = 0;
    int bit_position = 0;
    int level = 0;
    
    if(NULL == root)
    {
        return;
    }
    
    for(bit_position = (int)depth - 1; bit_position >= 0; --bit_position)
    {
        path[path_length++] = current_node;
        
        if(IsBitSet(ip_address, bit_position))
        {
            if(!HAS_RIGHT_CHILD(current_node))
            {
                return;
            }
            current_node = RIGHT_CHILD(current_node);
        }
        else
        {
            if(!HAS_LEFT_CHILD(current_node))
            {
                return;
            }
            current_node = LEFT_CHILD(current_node);
        }
    }
    
    for(level = path_length - 1; level >= 0; --level)
    {
        current_node = path[level];
        
        current_node->has_free_in_subtree = !IS_LEAF(current_node) ||
                                           HAS_AVAILABLE_CHILD_SLOT(current_node) ||
                                           HasChildWithFreeSpace(current_node);
    }
}

static trie_node_t* TraversePath(trie_node_t* root, unsigned int ip_address, size_t depth)
{
    trie_node_t* current_node = root;
    int bit_position = 0;
    
    for (bit_position = (int)depth - 1; bit_position >= 0; --bit_position) 
    {
        /* BIT IS 1 - GO RIGHT*/
        if(IsBitSet(ip_address,bit_position))
        {
            if(!HAS_RIGHT_CHILD(current_node))
            {
                return NULL;
            }
            current_node = RIGHT_CHILD(current_node);
        }
        else /* Bit IS 0 - Go LEFT */
        {
            if(!HAS_LEFT_CHILD(current_node))
            {
                return NULL;
            }
            current_node = LEFT_CHILD(current_node);
        }
    }
    
    return current_node;
}

static trie_status_e InsertAndAllocate(trie_node_t* root, unsigned int* allocated_ip, unsigned int ip_address, size_t depth)
{
    trie_status_e status = TRIE_SUCCESS;
    
    status = TRIECreatePath(root, ip_address, depth);
    if(status != TRIE_SUCCESS)
    {
        return status;
    }
    
    *allocated_ip = ip_address;
    TRIEUpdateMetadata(root, ip_address, depth);
    
    return TRIE_SUCCESS;
}

static int HasChildWithFreeSpace(trie_node_t* node)
{
    return (HAS_LEFT_CHILD(node) && HAS_FREE_IN_SUBTREE(LEFT_CHILD(node))) ||
           (HAS_RIGHT_CHILD(node) && HAS_FREE_IN_SUBTREE(RIGHT_CHILD(node)));
}

