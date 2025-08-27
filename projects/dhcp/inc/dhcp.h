/**************************************
Exercise:   Projects - DHCP
Date:       27/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

typedef struct dhcp
{
    ip_t* ip,
    subnet_t* mask
} dhcp_t


dhcp_t* DHCPCreate(const char* netid , unsigned int mask);
void DHCPDestroy(dhcp_t* server);
dhcp_status_t DHCPAllocateIp(dhcp_t* dhcp, char* ip);
void DHCPFreeIp(dhcp_t* dhcp, char* ip);
size_t DHCPCountFree(const dhcp_t* dhcp);
