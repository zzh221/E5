#pragma warning(disable:4996)
#include "client.h"

void client::init()
{
	WSADATA wsaData;
	int port = 1234;						//�˿�
	std::string serverIp;					//������IP

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) //����Windows Sockets��ʼ��WinSock
	{
		printf("Winsock�޷���ʼ��!\n");
		WSACleanup();
		exit(-1);
	}
	printf("�ɹ�����Winsock��\n");
	printf("���ڴ���SOCKET....\n");
	std::cout << "�����������IP:\n";
	std::cin >> serverIp;
	/* ���÷�����IP���˿���Ϣ */
	memset(&serverAddr, 0, sizeof(struct sockaddr));//����0�����
	serverAddr.sin_family = AF_INET;				//AF_INET��IPv4��Internet��ַ���ʽ��
	serverAddr.sin_port = htons(port);				//����server�ļ����˿�
	serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());	//����server�ĵ�ַ
	socket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);			//�������ݱ���ʽ���������ӵ�UDP������׽���
}

client::client()
{
	init();
}

void client::sendToServer(std::string msg)//��������
{
	sendto(socket1,	 //�����׽���
		msg.c_str(), //ָ�����Ҫ���������
		msg.size(),	 //���ݵĳ���
		0,			 //��־
		(struct sockaddr*)&serverAddr,//�������׽��ֵ�ַ
		sizeof(serverAddr));//��ַ����
}

std::string client::receieveFromServer()
{
	while (true) {
		char buffer[128] = "\0";//���ڽ������ݵĻ������ַ���
		sockaddr_in ServerAddr;
		int msgLen=sizeof(ServerAddr);
		if (recvfrom(socket1,		//�����׽���
					buffer,			//���ַ�����������
					sizeof(buffer), //�ַ�����С 
					0,				//��־
					(struct sockaddr*) & ServerAddr,//�������׽��ֵ�ַ
					&msgLen							//��ַ����
					) != SOCKET_ERROR) {//��δ�յ��򷵻ش����ַ�����ѭ�����ܵȴ������յ��򷵻��ֽ�����������һ��
			return std::string(buffer);//�յ���Ϣ�򷵻���Ϣ
		}
	}
}
