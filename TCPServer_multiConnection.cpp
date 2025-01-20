#include<iostream>
#include<cstring>
#include<thread>
#include<vector>
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

void linkClientThread(SOCKET linkSocket, unsigned int linkId){
	printf("客户端(id:%d) 已连接！\n", linkId);
	const size_t BufMaxLen = 1000;
	char buf[BufMaxLen] = {};
	char sendBuf[BufMaxLen] = "服务器成功接收!";
	while(true){
		int ret = recv(linkSocket, buf, BufMaxLen, 0);
		if(ret > 0){
			printf("从客户端(id:%d)接收到数据:%s\n", linkId, buf);
			memset(buf, 0, BufMaxLen);
			send(linkSocket, sendBuf, strlen(sendBuf), 0);
		}
		else if(ret == 0){
			printf("客户端(id:%d)停止发送数据，关闭连接...\n", linkId);
			break;
		}
		else{
			printf("recv发生错误！\n");
			break;
		}
	}
	
#ifdef _WIN32
	closesocket(linkSocket);
#else
	close(linkSocket);
#endif
	
}

int main(){
    printf("ProjectPath=[%s]\n", __FILE__);
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
	
	unsigned int linkId = 0;
	sockaddr clientAddr;		//
#ifndef _WIN32
	unsigned
#endif
	int clientAddrLen = sizeof(sockaddr);
	while(true){
		SOCKET linkSocket = accept(tcpsocket, &clientAddr, &clientAddrLen);
		std::thread linkThread(linkClientThread, linkSocket, ++linkId);
		linkThread.detach();
	}

	
#ifdef _WIN32
	closesocket(tcpsocket);
	WSACleanup();
#else
	close(tcpsocket);
#endif
	std::cout << "已关闭服务器Socket..." << std::endl;
	
	
	return 0;
}
