#include<iostream>
#include<cstring>
#ifdef _WIN32

#include<winsock2.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
typedef int SOCKET;

#endif

int main(){
#ifdef _WIN32
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(2, 2), &wsd)){
		std::cout << "WSAStartup Error" << std::endl;
		exit(-1);
	}   
#endif
	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0); 
	sockaddr_in sain;
	sain.sin_family = AF_INET;
	sain.sin_addr.s_addr = htonl(INADDR_ANY);
	sain.sin_port = htons(atoi("50002"));
	
	if(bind(udpSocket, (sockaddr *)&sain, sizeof(sockaddr)) == -1){
		std::cout << "绑定失败" << std::endl;
	}   
	const size_t BufMaxSize = 1000;
	char buf[BufMaxSize] = {}; 
	sockaddr fromAddr;
#ifndef _WIN32
	unsigned
#endif
	int fromAddrLen = sizeof(sockaddr);
	std::cout << "等待接收..." << std::endl;
	while(true){
		if(recvfrom(udpSocket, buf, BufMaxSize, 0, &fromAddr, &fromAddrLen) != -1){
			std::cout << "接收到数据：" << buf << std::endl;
			memset(buf, 0, sizeof(buf));
		}   
		else{
			std::cout << "接收失败或发生错误！" << std::endl;
			return -1; 
		}   
	}   
	//shutdown(udpSocket, SB_BOTH);
#ifdef _WIN32
	WSACleanup();
	closesocket(udpSocket);
#else
	close(udpSocket);
#endif
	
	return 0;
}

