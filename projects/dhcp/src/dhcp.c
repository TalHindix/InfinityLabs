/**************************************
Exercise:   Projects - DHCP
Date:       27/8/2025
Developer:  Tal Hindi
Reviewer:   Daniel Gamliel
Status:     
**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "dhcp.h" /* DHCPCreate */
#include "trie.h" /* TRIECreate */

#define IPv4_BITS 32
#define NETWORK_ADDRESS_HOST 0
#define GATEWAY_ADDRESS_HOST 1
#define RESERVED_IP_COUNT 3
#define FIRST_ALLOCATABLE_HOST 2
#define BYTE_MASK 0xFF
#define BITS_PER_BYTE 8
#define BIT_SHIFT_ONE 1
#define MIN_USABLE_DEPTH 2
#define MAX_SHIFT_BITS 31

struct dhcp 
{
    trie_node_t* trie;
    unsigned int network;
    size_t depth;
};

static unsigned int ConvertSubnetToUInt(const unsigned char netip[BYTES_IN_IPV4], size_t mask);
static unsigned int ExtractHostFromIp(const unsigned char ip[BYTES_IN_IPV4], size_t mask);
static void BuildFullIp(unsigned char result[BYTES_IN_IPV4], unsigned int network, unsigned int ip_address);
static int IsReservedHost(unsigned int ip_address, size_t depth);
static dhcp_status_e ConvertTrieStatus(trie_status_e trie_status);
static dhcp_status_e AllocateNextAvailable(dhcp_t* dhcp, unsigned int* allocated_ip_address);
static void CleanupDhcp(dhcp_t* dhcp);

dhcp_t* DHCPCreate(const unsigned char netip[BYTES_IN_IPV4], size_t mask)
{
    dhcp_t* dhcp = NULL;
    unsigned int dummy = 0;

    assert(netip);
    assert(mask <= MAX_SHIFT_BITS);
    assert((IPv4_BITS - mask) >= MIN_USABLE_DEPTH);

    dhcp = (dhcp_t*)malloc(sizeof(dhcp_t));
    if(!dhcp)
    {
        return NULL;
    }

    dhcp->trie = NULL;
    dhcp->network = ConvertSubnetToUInt(netip, mask);
    dhcp->depth = IPv4_BITS - mask;
    
    dhcp->trie = TRIECreate();
    if(!dhcp->trie)
    {
        free(dhcp);
        return NULL;
    }

    if (TRIEInsert(dhcp->trie, &dummy, NETWORK_ADDRESS_HOST, dhcp->depth) != TRIE_SUCCESS)
    {
        CleanupDhcp(dhcp);
        return NULL;
    }
    
    if (TRIEInsert(dhcp->trie, &dummy, GATEWAY_ADDRESS_HOST, dhcp->depth) != TRIE_SUCCESS)
    {
        CleanupDhcp(dhcp);
        return NULL;
    }
    
    if (TRIEInsert(dhcp->trie, &dummy, (BIT_SHIFT_ONE << dhcp->depth) - BIT_SHIFT_ONE, dhcp->depth) != TRIE_SUCCESS)
    {
        CleanupDhcp(dhcp);
        return NULL;
    }

    return dhcp;
}

void DHCPDestroy(dhcp_t* dhcp)
{
    assert(dhcp);
    assert(dhcp->trie);
    
    TRIEDestroy(dhcp->trie);
    free(dhcp);
}

dhcp_status_e DHCPAllocateIP(dhcp_t* dhcp,
                             unsigned char allocated_ip[BYTES_IN_IPV4],
                             const unsigned char requested_ip[BYTES_IN_IPV4])
{
    unsigned int allocated_ip_address = 0;
    unsigned int requested_ip_address = 0;
    dhcp_status_e status = SUCCESS;
    trie_status_e trie_status = TRIE_SUCCESS;
    int ip_zero = 0;

    assert(dhcp);
    assert(dhcp->trie);
    assert(allocated_ip);
    assert(requested_ip);

    ip_zero = (!requested_ip[0] && !requested_ip[1] &&
               !requested_ip[2] && !requested_ip[3]);

    requested_ip_address = ExtractHostFromIp(requested_ip, IPv4_BITS - dhcp->depth);
    
    if (ip_zero || IsReservedHost(requested_ip_address, dhcp->depth))
    {
        status = AllocateNextAvailable(dhcp, &allocated_ip_address);
    }
    else
    {
        trie_status = TRIEInsert(dhcp->trie, &allocated_ip_address, requested_ip_address, dhcp->depth);
        status = ConvertTrieStatus(trie_status);
        
        if (IP_OCCUPIED_ALLOCATED_ANOTHER == status || ALLOC_FAILURE == status)
        {
            status = AllocateNextAvailable(dhcp, &allocated_ip_address);
        }
    }

    if (status == SUCCESS || status == IP_OCCUPIED_ALLOCATED_ANOTHER)
    {
        BuildFullIp(allocated_ip, dhcp->network, allocated_ip_address);
    }

    return status;
}                   

dhcp_status_e DHCPFreeIP(dhcp_t* dhcp, const unsigned char ip[BYTES_IN_IPV4])
{
    unsigned int ip_address = 0;
    trie_status_e trie_status = TRIE_SUCCESS;

    assert(dhcp);
    assert(dhcp->trie);
    assert(ip);

    ip_address = ExtractHostFromIp(ip, IPv4_BITS - dhcp->depth);
    
    if (IsReservedHost(ip_address, dhcp->depth))
    {
        return DOUBLE_FREE;
    }
    
    trie_status = TRIEFree(dhcp->trie, ip_address, dhcp->depth);
    
    return ConvertTrieStatus(trie_status);
}                       

size_t DHCPCountFree(const dhcp_t* dhcp)
{
    size_t trie_free = 0;
    size_t reserved = RESERVED_IP_COUNT;
    
    assert(dhcp);
    assert(dhcp->trie);

    trie_free = TRIECountFree(dhcp->trie, dhcp->depth);
    
    return (trie_free > reserved) ? (trie_free - reserved) : 0;
}

static unsigned int ConvertSubnetToUInt(const unsigned char netip[BYTES_IN_IPV4], size_t mask)
{
    unsigned int full_ip_number = 0;
    unsigned int subnet_mask = 0;
    size_t byte_index = 0;

    /* Step 1: Convert 4 bytes (192.168.1.100) to single number */
    for (byte_index = 0; byte_index < BYTES_IN_IPV4; ++byte_index)
    {
        full_ip_number = (full_ip_number << BITS_PER_BYTE) | netip[byte_index];
    }

    /* Special case: if mask is 0, return 0 */
    if (mask == 0)
    {
        return 0;
    }

    /* Step 2: Create subnet mask (like 255.255.255.0 for /24) */
    subnet_mask = ~0 << (IPv4_BITS - mask);

    /* Step 3: Remove host bits to get network address (192.168.1.0) */
    return full_ip_number & subnet_mask;
}

static unsigned int ExtractHostFromIp(const unsigned char ip[BYTES_IN_IPV4], size_t mask)
{
    size_t host_bits = IPv4_BITS - mask;
    unsigned int host_mask = 0;
    unsigned int ip_address = 0;
    size_t i = 0;
    
    for (i = 0; i < BYTES_IN_IPV4; ++i)
    {
        ip_address = (ip_address << BITS_PER_BYTE) | ip[i];
    }
    
    if (host_bits >= IPv4_BITS)
    {
        return ip_address;
    }
    
    host_mask = (BIT_SHIFT_ONE << host_bits) - BIT_SHIFT_ONE;
    
    return ip_address & host_mask;
}

static void BuildFullIp(unsigned char result[BYTES_IN_IPV4], unsigned int network, unsigned int ip_address)
{
    unsigned int full = network | ip_address;
    size_t i = 0;

    for (i = 0; i < BYTES_IN_IPV4; ++i)
    {
        result[BYTES_IN_IPV4 - 1 - i] = (unsigned char)(full & BYTE_MASK);
        full >>= BITS_PER_BYTE;
    }
}

static int IsReservedHost(unsigned int ip_address, size_t depth)
{
    unsigned int broadcast = (BIT_SHIFT_ONE << depth) - BIT_SHIFT_ONE;
    
    return (ip_address == NETWORK_ADDRESS_HOST || 
            ip_address == GATEWAY_ADDRESS_HOST || 
            ip_address == broadcast);
}

static dhcp_status_e ConvertTrieStatus(trie_status_e trie_status)
{
    switch (trie_status)
    {
        case TRIE_SUCCESS:
            return SUCCESS;
        case TRIE_IP_OCCUPIED_ALLOCATED_ANOTHER:
            return IP_OCCUPIED_ALLOCATED_ANOTHER;
        case TRIE_NO_IP_AVAILABLE:
            return NO_IP_AVAILABLE;
        case TRIE_ALLOC_FAILURE:
            return ALLOC_FAILURE;
        case TRIE_DOUBLE_FREE:
            return DOUBLE_FREE;
        case TRIE_IP_NOT_FOUND:
            return IP_NOT_FOUND;
        default:
            return ALLOC_FAILURE;
    }
}

static dhcp_status_e AllocateNextAvailable(dhcp_t* dhcp, unsigned int* allocated_ip_address)
{
    unsigned int current_ip = FIRST_ALLOCATABLE_HOST;
    unsigned int max_ip = (BIT_SHIFT_ONE << dhcp->depth) - FIRST_ALLOCATABLE_HOST;
    trie_status_e trie_status = TRIE_SUCCESS;
    
    while (current_ip <= max_ip)
    {
        trie_status = TRIEInsert(dhcp->trie, allocated_ip_address, current_ip, dhcp->depth);
        if (trie_status == TRIE_SUCCESS)
        {
            return IP_OCCUPIED_ALLOCATED_ANOTHER;
        }
        current_ip++;
    }
    
    return NO_IP_AVAILABLE;
}

static void CleanupDhcp(dhcp_t* dhcp)
{
    TRIEDestroy(dhcp->trie);
    free(dhcp);
}