#include"public.h"
#include"client.h"
client myClient;

bool login(std::string username,std::string seqNum) {
	//��������������֤
	myClient.sendToServer("login " + username + " " + seqNum);
	std::string reply = myClient.receieveFromServer();
	if (reply == "permit")
		return true;
	else return false;
}

void check() {
	//ÿ����һ��ʱ��Է�������һ��check
}

int main()
{	

	std::string username, seqNum;
	printf("������[�û���]\n");
	std::cin >> username;
	printf("������[���к�]\n");
	std::cin >> seqNum;
	while (!login(username, seqNum)) {
		printf("��½ʧ��,���֤��Ч!����������!\n");
		printf("������[�û���]\n");
		std::cin >> username;
		printf("������[���к�]\n");
		std::cin >> seqNum;
	}
	printf("��¼�ɹ�\n");
	//std::thread check_thread(check);
	while (true) {
		printf("����quit�˳�\n");
		std::string INS;
		std::cin >> INS;
		if (INS == "quit") {
			myClient.sendToServer(INS);
			break;
		}
	}

	system("pause");
	return 0;
}