/**************************************
Exercise: 	DS - TRIE
Date:	    27/8/2025
Developer:	Tal Hindi
Reviewer: 	Daniel Gamliel
Status:		
**************************************/

#ifndef __ILRD__TRIE_H__
#define __ILRD__TRIE_H__

#include <stddef.h> /* size_t */

typedef struct trie_node trie_node_t;

typedef enum trie_status
{
    TRIE_SUCCESS,                        /* Operation completed successfully */
    TRIE_ALLOC_FAILURE,                 /* Memory allocation failed */
    TRIE_IP_OCCUPIED_ALLOCATED_ANOTHER, /* Requested Ip taken, allocated next ip */
    TRIE_NO_IP_AVAILABLE,               /* No free ip available */
    TRIE_DOUBLE_FREE,                   /* Attempted to free already free ip */
    TRIE_IP_NOT_FOUND                   /* ip not found in trie */
} trie_status_e;

/*============================================================================
                                API FUNCTIONS
============================================================================*/

/*
 * TRIECreate - Creates and initializes a new trie
 * 
 * Description:
 *   Makes a new empty trie. This creates the root node that you use
 *   for all other operations on the trie.
 * 
 * Parameters:
 *   None
 * 
 * Returns:
 *   Pointer to trie root on success, NULL if memory allocation fails
 * 
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
trie_node_t* TRIECreate(void);

/*
 * TRIEInsert - Inserts a Ip into the trie
 * 
 * Description:
 *   Tries to add the requested ip. If that Ip is already taken,
 *   finds the next available ip and uses that instead.
 *   Automatically cleans up on allocation failures.
 *  
 * Parameters:
 *   root         - Pointer to trie root node
 *   allocated_ip - Output: pointer to store the actually allocated ip
 *   requested_ip - Input: the ip you want to insert
 *   depth        - Number of bits to use (tree depth)
 * 
 * Returns:
 *   TRIE_SUCCESS                        - Got the requested ip
 *   TRIE_IP_OCCUPIED_ALLOCATED_ANOTHER  - Used a different available ip
 *   TRIE_NO_IP_AVAILABLE               - Trie is full, no ips left
 *   TRIE_ALLOC_FAILURE                 - Memory allocation failed 
 * 
 * Time Complexity: O(depth) for insertion + O(2^depth) for alternative search
 * Space Complexity: O(depth) for path creation
 * 
 */
trie_status_e TRIEInsert(trie_node_t* root, unsigned int* allocated_ip, unsigned int requested_ip, size_t depth);

/*
 * TRIESearch - Searches for a ip in the trie
 * 
 * Description:
 *   Checks if a ip is stored in the trie. Follows the path
 *   created by the ip's bits to see if it exists.
 * 
 * Parameters:
 *   root  - Pointer to trie root node
 *   ip_address  - The ip to look for
 *   depth - Number of bits to use for the path
 * 
 * Returns:
 *   1 if the ip is found and stored
 *   0 if the ip is not found or not stored
 * 
 * Time Complexity: O(depth)
 * Space Complexity: O(1)
 * 
 */
int TRIESearch(trie_node_t* root, unsigned int ip_address, size_t depth);

/*
 * TRIEFree - Frees an allocated IP from the trie
 * 
 * Description:
 *   Removes a IP from the trie so it can be used again.
 *   Marks the IP as no longer stored.
 * 
 * Parameters:
 *   root     - Pointer to trie root node
 *   ip_address  - The IP to remove
 *   depth    - Numbers of bits used for the path
 * 
 * Returns:
 *   TRIE_SUCCESS      - IP successfully removed
 *   TRIE_DOUBLE_FREE  - IP was already removed
 *   TRIE_IP_NOT_FOUND - IP not found in trie
 * 
 * Time Complexity: O(depth)
 * Space Complexity: O(1)
 * 
 */
trie_status_e TRIEFree(trie_node_t* root, unsigned int ip_address, size_t depth);

/*
 * TRIEDestroy - Destroys trie and frees all memory
 * 
 * Description:
 *   Deletes the entire trie and frees all memory.
 *   Use this when you are done with the trie.
 * 
 * Parameters:
 *   root - Pointer to trie root node
 * 
 * Returns:
 *   None
 * 
 * Time Complexity: O(n) where n = IP of allocated nodes
 * Space Complexity: O(depth) for recursion stack
 * 
 */
void TRIEDestroy(trie_node_t* root);

/*
 * TRIECountFree - Counts available IP'S in the trie
 * 
 * Description:
 *   Counts how many IPs are still available to be allocated.
 *   Checks all possible IPs and counts the free ones.
 * 
 * Parameters:
 *   root  - Pointer to trie root node
 *   depth - Number of bits that define the range of IPs
 * 
 * Returns:
 *   Number of available IPs that can still be allocated
 * 
 * Time Complexity: O(2^depth) - must check every possible value
 * Space Complexity: O(1)
 * 
 * Note: This can be slow for large depths because it checks every ip.
 */
size_t TRIECountFree(trie_node_t* root, size_t depth);


#endif /* __ILRD_TRIE__H__ */

