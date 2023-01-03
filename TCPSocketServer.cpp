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
		return -1;
	}
#endif
	SOCKET tcpsocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sain;
	sain.sin_family = AF_INET;
	sain.sin_addr.s_addr = htonl(INADDR_ANY);
	sain.sin_port = htons(atoi("50002"));
	if(bind(tcpsocket, (sockaddr*)&sain, sizeof(sockaddr)) == -1){
		std::cout << "bind Error" <<  std::endl;
		return -1;
	}
	listen(tcpsocket, 10);
	std::cout << "等待连接中..." << std::endl;
	sockaddr clientAddr;		//
	
#ifndef _WIN32
	unsigned
#endif
	int clientAddrLen = sizeof(sockaddr);
	SOCKET linkSocket = accept(tcpsocket, &clientAddr, &clientAddrLen);
	std::cout << "连接成功！" << std::endl;
	const size_t BufMaxLen = 1000;
	char buf[BufMaxLen] = {};
	char sendBuf[BufMaxLen] = "服务器成功接收!";
	while(true){
		int ret = recv(linkSocket, buf, BufMaxLen, 0);
		if(ret > 0){
			std::cout << "从客户端接收到数据:" << buf << std::endl;
			memset(buf, 0, BufMaxLen);
			send(linkSocket, sendBuf, strlen(sendBuf), 0);
		}
		else if(ret == 0){
			std::cout << "客户端停止发送数据，准备关闭连接..." << std::endl;
			break;
		}
		else{
			std::cout << "recv发生错误！" << std::endl;
			
		}
	}
	
#ifdef _WIN32
	closesocket(tcpsocket);
	closesocket(linkSocket);
	WSACleanup();
#else
	close(tcpsocket);
	close(linkSocket);
#endif
	std::cout << "已关闭服务器Socket..." << std::endl;
	
	return 0;
}
