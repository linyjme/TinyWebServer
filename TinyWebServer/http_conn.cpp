#include<WINSOCK2.H>
#include"http_conn.h"
#include<STDIO.H>
#include<iostream>
#include<cstring>
using namespace std;
#pragma comment(lib, "ws2_32.lib")


Http_conn::Http_conn()
{
}

Http_conn::~Http_conn()
{
}

void Http_conn::client_main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return;
	}
	while (true) {
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sclient == INVALID_SOCKET)
		{
			printf("invalid socket!");
			return;
		}

		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{  //����ʧ�� 
			printf("connect error !");
			closesocket(sclient);
			return;
		}

		string data;
		cin >> data;
		const char* sendData;
		sendData = data.c_str();   //stringתconst char* 
		//char * sendData = "��ã�TCP����ˣ����ǿͻ���\n";
		send(sclient, sendData, strlen(sendData), 0);
		//send()������������ָ����socket�����Է�����
		//int send(int s, const void * msg, int len, unsigned int flags)
		//sΪ�ѽ��������ӵ�socket��msgָ���������ݣ�len��Ϊ���ݳ��ȣ�����flagsһ����0
		//�ɹ��򷵻�ʵ�ʴ��ͳ�ȥ���ַ�����ʧ�ܷ���-1������ԭ�����error 

		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0) {
			recData[ret] = 0x00;
			printf(recData);
		}
		closesocket(sclient);
	}


	WSACleanup();
	return;
}

void Http_conn::server_main()
{
    //��ʼ��WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return;
    }

    //�����׽���  
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        return;
    }

    //��IP�Ͷ˿�  
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }

    //��ʼ����  
    if (listen(slisten, 5) == SOCKET_ERROR)
    {
        printf("listen error !");
        return;
    }

    //ѭ����������  
    SOCKET sClient;
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
    char revData[255];
    while (true)
    {
        printf("�ȴ�����...\n");
        sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
        if (sClient == INVALID_SOCKET)
        {
            printf("accept error !");
            continue;
        }
        printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

        //��������  
        int ret = recv(sClient, revData, 255, 0);
        if (ret > 0)
        {
            revData[ret] = 0x00;
            printf(revData);
        }

        //��������  
        const char* sendData = "��ã�TCP�ͻ��ˣ�\n";
        send(sClient, sendData, strlen(sendData), 0);
        closesocket(sClient);
    }

    closesocket(slisten);
    WSACleanup();
    return;
}