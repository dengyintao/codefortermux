#include <iostream>
#include <unistd.h>
#include <string>

#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <boost/version.hpp>
#include <boost/config.hpp>
#include <boost/format.hpp>

#include "./http/httplib.h"

#define ETH_NAME "wlan0"  //如果要获取其他网卡的地址，将这个换为其他网卡名称，比如eth0
#define CPPHTTPLIB_OPENSSL_SUPPORT
	
std::string get_ip();
void get_mac(char * mac_a);
bool PushMessagetoIOS(std::string msg);
			     //
int main()
{
	 std::string ip = "";
    //监控ip变化
 while(1)
 {
 	std::string iptemp = get_ip();
 	if (ip!=iptemp)
 	{
		ip = iptemp;
 		boost::format fmt("当前ip地址:%s");
 		fmt%iptemp;
 		std::string msg = fmt.str();
 		bool bSend = false;
 		while (!bSend)
 		{
 			bSend = PushMessagetoIOS(msg);
 			if (!bSend)
 			{
 				printf("current_ip: %s 发送失败\n", iptemp.c_str());
 			}	
 		}
 	}		
 	sleep(10); 
 }
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
     return "wifi已断开";
  }

  strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);      //Interface name
  if (ioctl(sockfd, SIOCGIFADDR, &ifr) == 0) 
  {   //SIOCGIFADDR 获取interface address
     memcpy(&sin, &ifr.ifr_addr, sizeof(ifr.ifr_addr));
     return inet_ntoa(sin.sin_addr);
  }
  return "不可用";
 }

 bool PushMessagetoIOS(std::string msg)
 {
	bool bRet = false;
	std::string url;
	boost::format fmt("/g8HF6uCjc2QrWDUiPthnPf/%s/%s");
	fmt%"termux"%msg;
	httplib::Client cli("http://api.day.app");
	std::string info = fmt.str();
	auto res = cli.Get(info.c_str());
	if(res->status)
		bRet = true;
	else
		bRet = false;
  return bRet;
 }
