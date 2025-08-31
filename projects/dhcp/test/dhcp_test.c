/**************************************
Exercise:   Projects - DHCP
Date:       27/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */
#include <string.h> /* memcmp */

#include "dhcp.h" /* DHCPCreate */

#define PASS "\033[32mPASS\033[0m"
#define FAIL "\033[31mFAIL\033[0m"

static void TestDHCPCreate(void);
static void TestBasicAllocation(void);
static void TestReservedIPs(void);
static void TestFreeAndRealloc(void);
static void TestFullSubnet(void);
static void TestCountFree(void);
static void PrintIp(const unsigned char ip[BYTES_IN_IPV4]);
static void PrintTestResult(const char* test_name, int passed);

int main(void)
{
    printf("=== DHCP Test Suite ===\n\n");
    
    TestDHCPCreate();
    TestBasicAllocation();
    TestReservedIPs();
    TestFreeAndRealloc();
    TestFullSubnet();
    TestCountFree();
    
    printf("\n=== Test Suite Complete ===\n");
    return 0;
}

static void TestDHCPCreate(void)
{
    unsigned char network[BYTES_IN_IPV4] = {192, 168, 1, 0};
    dhcp_t* dhcp = NULL;
    size_t free_count = 0;
    
    printf("Test 1: DHCP Creation\n");
    
    dhcp = DHCPCreate(network, 24);
    
    PrintTestResult("DHCPCreate with /24 subnet", dhcp != NULL);
    
    if (dhcp) {
        free_count = DHCPCountFree(dhcp);
        PrintTestResult("Initial free count > 0", free_count > 0);
        DHCPDestroy(dhcp);
    }
    
    printf("\n");
}

static void TestBasicAllocation(void)
{
    unsigned char network[BYTES_IN_IPV4] = {192, 168, 1, 0};
    unsigned char requested[BYTES_IN_IPV4] = {192, 168, 1, 10};
    unsigned char allocated[BYTES_IN_IPV4];
    dhcp_t* dhcp = NULL;
    dhcp_status_e result = SUCCESS;
    
    printf("Test 2: Basic IP Allocation\n");
    
    dhcp = DHCPCreate(network, 24);
    result = DHCPAllocateIP(dhcp, allocated, requested);
    
    PrintTestResult("Allocate available IP", result == SUCCESS);
    PrintTestResult("Allocated matches requested", 
                   memcmp(allocated, requested, BYTES_IN_IPV4) == 0);
    
    printf("Requested: ");
    PrintIp(requested);
    printf(", Allocated: ");
    PrintIp(allocated);
    printf("\n");
    
    DHCPDestroy(dhcp);
    printf("\n");
}

static void TestReservedIPs(void)
{
    unsigned char network[BYTES_IN_IPV4] = {192, 168, 1, 0};
    unsigned char allocated[BYTES_IN_IPV4];
    unsigned char network_addr[BYTES_IN_IPV4] = {192, 168, 1, 0};
    unsigned char gateway_addr[BYTES_IN_IPV4] = {192, 168, 1, 1};
    unsigned char broadcast_addr[BYTES_IN_IPV4] = {192, 168, 1, 255};
    dhcp_t* dhcp = NULL;
    dhcp_status_e result1 = SUCCESS;
    dhcp_status_e result2 = SUCCESS;
    dhcp_status_e result3 = SUCCESS;
    
    printf("Test 3: Reserved IP Addresses\n");
    
    dhcp = DHCPCreate(network, 24);
    
    result1 = DHCPAllocateIP(dhcp, allocated, network_addr);
    result2 = DHCPAllocateIP(dhcp, allocated, gateway_addr);
    result3 = DHCPAllocateIP(dhcp, allocated, broadcast_addr);
    
    PrintTestResult("Network address reserved", result1 == IP_OCCUPIED_ALLOCATED_ANOTHER);
    PrintTestResult("Gateway address reserved", result2 == IP_OCCUPIED_ALLOCATED_ANOTHER);
    PrintTestResult("Broadcast address reserved", result3 == IP_OCCUPIED_ALLOCATED_ANOTHER);
    
    DHCPDestroy(dhcp);
    printf("\n");
}

static void TestFreeAndRealloc(void)
{
    unsigned char network[BYTES_IN_IPV4] = {192, 168, 1, 0};
    unsigned char test_ip[BYTES_IN_IPV4] = {192, 168, 1, 50};
    unsigned char allocated[BYTES_IN_IPV4];
    dhcp_t* dhcp = NULL;
    dhcp_status_e free_result = SUCCESS;
    dhcp_status_e realloc_result = SUCCESS;
    dhcp_status_e double_free2 = SUCCESS;
    
    printf("Test 4: Free and Reallocate\n");
    
    dhcp = DHCPCreate(network, 24);
    
    DHCPAllocateIP(dhcp, allocated, test_ip);
    
    free_result = DHCPFreeIP(dhcp, test_ip);
    PrintTestResult("Free allocated IP", free_result == SUCCESS);
    
    realloc_result = DHCPAllocateIP(dhcp, allocated, test_ip);
    PrintTestResult("Reallocate freed IP", realloc_result == SUCCESS);
    PrintTestResult("Reallocated matches original", 
                   memcmp(allocated, test_ip, BYTES_IN_IPV4) == 0);
    
    DHCPFreeIP(dhcp, test_ip);
    DHCPFreeIP(dhcp, test_ip);
    double_free2 = DHCPFreeIP(dhcp, test_ip);
    PrintTestResult("Double free detected", double_free2 == DOUBLE_FREE);
    
    DHCPDestroy(dhcp);
    printf("\n");
}

static void TestFullSubnet(void)
{
    unsigned char network[BYTES_IN_IPV4] = {10, 0, 0, 0};
    unsigned char allocated[BYTES_IN_IPV4];
    unsigned char test_ip[BYTES_IN_IPV4] = {10, 0, 0, 2};
    dhcp_t* dhcp = NULL;
    size_t initial_free = 0;
    dhcp_status_e result1 = SUCCESS;
    dhcp_status_e result2 = SUCCESS;
    size_t final_free = 0;
    
    printf("Test 5: Full Subnet (/30)\n");
    
    dhcp = DHCPCreate(network, 30);
    
    initial_free = DHCPCountFree(dhcp);
    printf("Initial free IPs in /30: %lu\n", (unsigned long)initial_free);
    
    result1 = DHCPAllocateIP(dhcp, allocated, test_ip);
    PrintTestResult("Allocate first available", result1 == SUCCESS);
    
    test_ip[3] = 3;
    result2 = DHCPAllocateIP(dhcp, allocated, test_ip);
    PrintTestResult("Allocate second (should fail - subnet full)", result2 == NO_IP_AVAILABLE);
    
    final_free = DHCPCountFree(dhcp);
    PrintTestResult("No free IPs remaining", final_free == 0);
    
    DHCPDestroy(dhcp);
    printf("\n");
}

static void TestCountFree(void)
{
    unsigned char network[BYTES_IN_IPV4] = {172, 16, 0, 0};
    unsigned char allocated[BYTES_IN_IPV4];
    unsigned char test_ip[BYTES_IN_IPV4] = {172, 16, 0, 10};
    dhcp_t* dhcp = NULL;
    size_t initial = 0;
    size_t after_alloc = 0;
    size_t after_free = 0;
    
    printf("Test 6: Count Free IPs\n");
    
    dhcp = DHCPCreate(network, 28);
    
    initial = DHCPCountFree(dhcp);
    printf("Initial free count (/28): %lu\n", (unsigned long)initial);
    
    DHCPAllocateIP(dhcp, allocated, test_ip);
    after_alloc = DHCPCountFree(dhcp);
    PrintTestResult("Free count decreased after allocation", after_alloc == initial - 1);
    
    DHCPFreeIP(dhcp, test_ip);
    after_free = DHCPCountFree(dhcp);
    PrintTestResult("Free count increased after free", after_free == initial);
    
    DHCPDestroy(dhcp);
    printf("\n");
}

static void PrintIp(const unsigned char ip[BYTES_IN_IPV4]) 
{
    printf("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

static void PrintTestResult(const char* test_name, int passed)
{
    printf("  %s: %s\n", test_name, passed ? PASS : FAIL);
}