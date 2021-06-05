#include"public.h"
#include"client.h"
client myClient;

bool login(std::string username,std::string seqNum) {
	//向服务器发送许可证
	myClient.sendToServer("login " + username + " " + seqNum);
	std::string reply = myClient.receieveFromServer();
	if (reply == "permit")
		return true;
	else return false;
}

void check() {
	//每经过一段时间对服务器发一个check
}

int main()
{	

	std::string username, seqNum;
	printf("请输入[用户名]\n");
	std::cin >> username;
	printf("请输入[序列号]\n");
	std::cin >> seqNum;
	while (!login(username, seqNum)) {
		printf("登陆失败,许可证无效!请重新输入!\n");
		printf("请输入[用户名]\n");
		std::cin >> username;
		printf("请输入[序列号]\n");
		std::cin >> seqNum;
	}
	printf("登录成功\n");
	//std::thread check_thread(check);
	while (true) {
		printf("输入quit退出\n");
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