// Server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>


using namespace std;
#pragma comment(lib,"ws2_32.lib")

bool MySend(int fd)
{
	cout << "请输入要发送的内容" << endl;
	char  sendbuf[20];
	cin >> sendbuf;
	if (sendbuf == "\0")
	{
		return true;
	}
	send(fd, sendbuf, sizeof(sendbuf), 0);
	return false;
}

int main()
{
	int sockfd;
	WSAData wsdata;
	int err = WSAStartup(MAKEWORD(2, 2), &wsdata);
	if (err < 0)
	{
		cout << "加载2.2版本socket库失败！" << endl;
		return 0;
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == SOCKET_ERROR)
	{
		cout << "socket创建失败！" << endl;
		return 0;
	}
	sockaddr_in sockaddrIn;
	sockaddrIn.sin_family = AF_INET;
	sockaddrIn.sin_port = htons(8090);
	sockaddrIn.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
	if (bind(sockfd, (sockaddr*)&sockaddrIn, sizeof(sockaddrIn)) == SOCKET_ERROR)
	{
		cout << "socket绑定IP和端口号失败！错误码：" << GetLastError() << endl;
		return 0;
	}
	if (listen(sockfd, 5) == SOCKET_ERROR)
	{
		cout << "监听失败！错误码：" << GetLastError()<< endl;
		return 0;
	}

	sockaddr_in clientAddr;
	int len = sizeof(clientAddr);
	int clientfd = accept(sockfd, (sockaddr*)&clientAddr, &len);
	if (clientfd == INVALID_SOCKET)
	{
		cout << "accept函数出错！错误码：" << GetLastError() << endl;
		closesocket(clientfd);
		WSACleanup();
		return 0;
	}
	
	char outBuf[INET_ADDRSTRLEN];
	cout << "接收到一个连接,客户端地址：" << inet_ntop(AF_INET,&clientAddr.sin_addr, outBuf,sizeof(outBuf)) << endl;
	if (MySend(clientfd))
	{
		return 0;
	}
	for (;;)
	{
		while (1)
		{
			char  buffer[1500] = { 0 };
			int size = recv(clientfd, buffer, 1499, 0);
			if (size > 0)
			{
				cout << "收到的数据内容为：" << buffer << endl;
				break;
			}
			else if (size == 0)
			{
				cout << "数据读取完成或客户端下线" << endl;
				closesocket(clientfd);
				break;
			}
			else
			{
				cout << "数据读取错误" << endl;
				break;
			}
		}
		if (MySend(clientfd))
		{
			break;
		}
	}
	

	closesocket(sockfd);
	WSACleanup();

	system("pause");
	return 0;
}

