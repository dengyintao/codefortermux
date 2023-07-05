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
std::string get_cpu_temperature();
void get_mac(char * mac_a);
bool PushMessagetoIOS(std::string msg);
			     //
int main()
{
	 std::string ip = "";
     float temp_value= 0.0;
    //监控ip变化
 while(1)
 {
 	std::string iptemp = get_ip();
    {
        std::string temperature = get_cpu_temperature();
        if (!temperature.empty())
        {
            temp_value = std::stof(temperature) / 1000.0;
            //std::cout << "当前手机温度：" << temp_value << " 度 Celsius" << std::endl;
        }
        else
        {
             std::cout << "无法获取手机温度信息" << std::endl;
        }
    }

 	if (ip!=iptemp)
 	{
		ip = iptemp;
 		boost::format fmt("当前ip地址:%s，当前温度:%.2f");
 		fmt%iptemp%temp_value;
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

//获取手机温度
std::string get_cpu_temperature() {
    std::ifstream temp_file("/sys/class/thermal/thermal_zone0/temp");
    std::string temperature;
    
    if (temp_file.is_open()) {
        std::getline(temp_file, temperature);
        temp_file.close();
        return temperature;
    }
    
    return "";
}

//获取手机ip        
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
