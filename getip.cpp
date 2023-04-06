#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#define ETH_NAME    "wlan0"  //如果要获取其他网卡的地址，将这个换为其他网卡名称，比如eth0
			     //
	std::string get_ip();
	void get_mac(char * mac_a);
			     //
	int main()
	{
	    //ip
	    std::string ip = get_ip();
	    printf("current_ip: %s\n", ip.c_str());
	    return 0;
	 }
        
         std::string get_ip()
	{
	  int                 sockfd;
	  struct sockaddr_in  sin;
	  struct ifreq        ifr;
	  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	  if (sockfd == -1) 
	  {
	        perror("socket error");
	        exit(1);
	  }
	  strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);      //Interface name
	  if (ioctl(sockfd, SIOCGIFADDR, &ifr) == 0) 
	  {    //SIOCGIFADDR 获取interface address
	       memcpy(&sin, &ifr.ifr_addr, sizeof(ifr.ifr_addr));
	       return inet_ntoa(sin.sin_addr);
	  }
	 }
