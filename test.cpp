#include <iostream>
#include <boost/version.hpp>
#include <boost/config.hpp>
#include <boost/format.hpp>
#include "httplib.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
int main()
{
        std::cout <<"boost version:"<<BOOST_VERSION<<std::endl;
	std::string url;
	boost::format fmt("/g8HF6uCjc2QrWDUiPthnPf/%s/ip:%s\nboost_version:%s");
	fmt%"termux"%"192.168.3.208"%BOOST_VERSION;
	httplib::Client cli("http://api.day.app");
	std::cout<<fmt<<std::endl;
	std::string info = fmt.str();
 	auto res = cli.Get(info.c_str());
	if(res->status)
	{
		std::cout<<"完成"<<std::endl;
	}
	else
	{
		std::cout<<"失败"<<std::endl;
	}
	return 0;
}
