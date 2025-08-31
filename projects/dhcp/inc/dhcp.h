/**************************************
Exercise:   Projects - DHCP
Date:       27/8/2025
Developer:  Tal Hindi
Reviewer:   Daniel Gamliel
Status:     
**************************************/

#ifndef __ILRD_DHCP_H__
#define __ILRD_DHCP_H__

#include <stddef.h> /* size_t */

#define BYTES_IN_IPV4 (4)

typedef struct dhcp dhcp_t;

typedef enum status
{
    SUCCESS,                        /* Operation completed successfully */
    ALLOC_FAILURE,                 /* Memory allocation failed */
    IP_OCCUPIED_ALLOCATED_ANOTHER, /* Requested IP taken, allocated alternative */
    NO_IP_AVAILABLE,               /* No free IPs in subnet */
    DOUBLE_FREE,                   /* Attempted to free already free IP */
    IP_NOT_FOUND                   /* IP not found in allocation table */
} dhcp_status_e;

/*============================================================================
                                API FUNCTIONS
============================================================================*/

/*
 * DHCPCreate - Creates and starts a new DHCP manager
 * 
 * Description:
 *   Makes a new DHCP manager for giving out IP addresses.
 *   Some IP addresses are saved for special use.
 * 
 * Parameters:
 *   netip - 4 bytes for the network IP address
 *   mask  - Number of bits for the network part
 * 
 * Returns:
 *   Pointer to DHCP manager on success, NULL if no memory
 * 
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
dhcp_t* DHCPCreate(const unsigned char netip[BYTES_IN_IPV4], size_t mask);

/*
 * DHCPDestroy - Cleans up DHCP manager and frees memory
 * 
 * Description:
 *   Deletes the DHCP manager and frees all memory.
 *   Use this when you are done with the DHCP manager.
 * 
 * Parameters:
 *   dhcp - Pointer to DHCP manager to destroy
 * 
 * Returns:
 *   None
 * 
 * Time Complexity: O(n) where n = number of nodes
 * Space Complexity: O(1)
 */
void DHCPDestroy(dhcp_t* dhcp);

/*
 * DHCPAllocateIP - Give out an IP address
 * 
 * Description:
 *   Tries to give the IP you want. If that IP is taken,
 *   finds the next free IP and gives that instead.
 * 
 * Parameters:
 *   dhcp         - Pointer to DHCP manager
 *   allocated_ip - Output: where to put the IP we give you
 *   requested_ip - Input: the IP you want
 * 
 * Returns:
 *   SUCCESS                        - Got the IP you wanted
 *   IP_OCCUPIED_ALLOCATED_ANOTHER  - IP was taken, gave you another
 *   NO_IP_AVAILABLE               - No free IPs left
 *   ALLOC_FAILURE                 - Memory problem
 * 
 * Time Complexity: O(depth) for search + O(n) for finding alternative
 * Space Complexity: O(1)
 */
dhcp_status_e DHCPAllocateIP(dhcp_t* dhcp, unsigned char allocated_ip[BYTES_IN_IPV4], const unsigned char requested_ip[BYTES_IN_IPV4]);

/*
 * DHCPFreeIP - Give back an IP address
 * 
 * Description:
 *   Returns an IP address so it can be used again.
 *   Makes the IP free for others to use.
 * 
 * Parameters:
 *   dhcp - Pointer to DHCP manager
 *   ip   - IP address to give back
 * 
 * Returns:
 *   SUCCESS      - IP given back successfully
 *   DOUBLE_FREE  - IP was already free
 *   IP_NOT_FOUND - IP not found
 * 
 * Time Complexity: O(depth)
 * Space Complexity: O(1)
 */
dhcp_status_e DHCPFreeIP(dhcp_t* dhcp, const unsigned char ip[BYTES_IN_IPV4]);

/*
 * DHCPCountFree - Count how many IP addresses are free
 * 
 * Description:
 *   Counts how many IP addresses are still free to give out.
 *   Does not count special reserved IP addresses.
 * 
 * Parameters:
 *   dhcp - Pointer to DHCP manager
 * 
 * Returns:
 *   Number of free IP addresses
 * 
 * Time Complexity: O(2^depth) - checks all possible IPs
 * Space Complexity: O(1)
 * 
 * Note: This can be slow for big networks because it checks every IP.
 */
size_t DHCPCountFree(const dhcp_t* dhcp);

#endif /* __ILRD_DHCP_H__ */
