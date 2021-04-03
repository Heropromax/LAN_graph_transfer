
//#include <stdafx.h>
#include "thread"
#include <windows.h>
#include<opencv2\opencv.hpp>
#include<iostream>
#include"timmer.h"


#pragma comment(lib,"ws2_32.lib")

using namespace cv;
using namespace std;










vector<char> recive_buff(65546);



class lock
{
private:
	bool locker = 0;
	mutex a;
public:
	void update(bool b)
	{
		a.lock();
		locker = b;
		a.unlock();
	}
	bool state()
	{
		a.lock();
		bool b = locker;
		a.unlock();
		return b;
	}
} lock1;






void show()
{
	Mat fa;
	vector<char> recive_buff2(65546);


	while (1)
	{
		while (!lock1.state()) Sleep(1);
		recive_buff2 = recive_buff;
		lock1.update(0);
		fa = imdecode(recive_buff2, -1);
		imshow("reciver", fa);
		waitKey(2);
	}
}

int Video_reciver()
{




	char ip[60];
	string s ;
	


	ifstream portData;
	portData.open("Addr.txt");
	//string portAddr;
	int portNum;
	portData >> s;
	portData >> portNum;

	strcpy_s(ip, s.c_str());

	cout << "this is grond\n\n";
	cout << "本地ip :" << ip << endl;
//	printf("输入本机ipIP\n");
//	scanf_s("%s", ip);
//	printf("本机IP:%s\n", ip);
	WSADATA wsaData;
	int port = portNum;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("加载套接字失败\n");
		return 1;
	}
	SOCKADDR_IN addrRec;
	addrRec.sin_family = AF_INET;
	addrRec.sin_port = ::htons(port);
	addrRec.sin_addr.s_addr = ::inet_addr(ip);
	SOCKET sockClient = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (sockClient == SOCKET_ERROR)
	{
		printf("套接字创建失败\n");
		return 1;
	}


	if (::bind(sockClient, (LPSOCKADDR)& addrRec, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("套接字绑定失败:%d\n", ::WSAGetLastError());
		return 1;
	}
	SOCKADDR_IN addrSend;
	int nLen = sizeof(SOCKADDR);
	Mat frame, rsframe;
	//VideoCapture capture(0);
	//capture >> frame;
	//capture.release();
	//resize(frame, frame, Size(), 0.25, 0.25);
	//_int64 datasize = frame.dataend - frame.datastart;

	//char buff[65546] = {0};

	
	//192.168.0.100



	thread a(show);
	a.detach();

	//Mat Fa;


	timmer_inS t1;
	while (1)
	{
		t1.T_start();
		if (SOCKET_ERROR != ::recvfrom(sockClient, &recive_buff[0], 65546, 0, (LPSOCKADDR)& addrRec, &nLen))
		{
			printf("收到Data!\n");
			lock1.update(1);
			//Fa = imdecode(recive_buff, -1);
			//imshow("reciver", Fa);
			//waitKey(1);

		}
		else cout << "等待数据\n";
		waitKey(1);
		cout << "fps : " << 1 / t1.T_now() << endl;
	}
	WSACleanup();
	return 0;

}
int Video_Sender(int camera)
{
	cout << "this is sky\n\n";

	char ip[60];
	string ip_s;

	ifstream portData;
	portData.open("Addr.txt");
	//string portAddr;
	int portNum;
	portData >> ip_s;
	portData >> portNum;
	strcpy_s(ip, ip_s.c_str());
	

	cout << "目标ip :" << ip << endl;
	//printf("输入目标IP\n");
	//scanf_s("%s", &ip);
	//printf("目标IP%s\n", ip);
	WSADATA wsaData;
	int port = portNum;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "加载套接字失败" << endl;
		return 1;
	}
	SOCKADDR_IN addrRec;
	addrRec.sin_family = AF_INET;
	addrRec.sin_port = htons(port);
	addrRec.sin_addr.s_addr = inet_addr(ip);
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockClient == SOCKET_ERROR)
	{
		cout << "创建套接字失败" << endl;
		return 1;
	}


	int nLen = sizeof(SOCKADDR_IN);


	VideoCapture capture(camera);
	Mat capframe, frame;
	capture >> capframe;
	int framesize = sizeof(frame);
	_int64 datasize = capframe.dataend - capframe.datastart;
	_int64 shrinksize = datasize / 16;


	std::vector<int> params;
	params.push_back(IMWRITE_JPEG_QUALITY);
	int quality = 20;
	params.push_back(quality);

	std::vector<uchar> buff;

	timmer_inS t1;
	while (1)
	{
		t1.T_start();
		capture >> capframe;

		imencode(".jpg", capframe, buff, params);

		
		sendto(sockClient, (char*)& buff[0], buff.size(), 0, (struct sockaddr*) & addrRec, nLen);
		waitKey(7);
		cout << "fps : " << 1 / t1.T_now() << endl;
		//time
		//Mat frame = imdecode(buff, -1);
		//imshow("hh", frame);
		//cout << buff.size()<<endl;
		//waitKey(2);
	}
	//waitKey();
	WSACleanup();
	return 0;
}




int main()
{
	ifstream type;
	type.open("sky_ground.txt");
	string name;
	type >> name;
	if (name == (string)"ground") Video_reciver();
	else if (name == (string)"sky")
	{
		cout << "相机号\n";
		int camNum;
		type >> camNum;
		Video_Sender(camNum);
	}
	

	else cout << "invalid cammand, type sky or ground at the doc" << endl;
	type.close();






	//此模板摘自github
	//char ip[20];
	//printf("输入本机ipIP\n");
	//scanf_s("%s", ip);
	//printf("本机IP:%s\n", ip);
	//WSADATA wsaData;
	//int port = 5099;
	//if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	//{
	//	printf("加载套接字失败\n");
	//	return 1;
	//}
	//SOCKADDR_IN addrRec;
	//addrRec.sin_family = AF_INET;
	//addrRec.sin_port = htons(port);
	//addrRec.sin_addr.s_addr = inet_addr(ip);
	//SOCKET sockClient = ::socket(AF_INET, SOCK_DGRAM, 0);
	//if (sockClient == SOCKET_ERROR)
	//{
	//	printf("套接字创建失败\n");
	//	return 1;
	//}
	//if (::bind(sockClient, (LPSOCKADDR)& addrRec, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	//{
	//	printf("套接字绑定失败:%d\n", WSAGetLastError());
	//	return 1;
	//}
	//SOCKADDR_IN addrSend;
	//int nLen = sizeof(SOCKADDR);
	//Mat frame, rsframe;
	////VideoCapture capture(0);
	////capture >> frame;
	////capture.release();
	////resize(frame, frame, Size(), 0.25, 0.25);
	////_int64 datasize = frame.dataend - frame.datastart;

	////char buff[65546] = {0};
	//std::vector<char> buff;

	////192.168.0.100

	//Mat Fa;


	//while (1)
	//{

	//	if (SOCKET_ERROR != recvfrom(sockClient, &buff[0], 65545, 0, (LPSOCKADDR)& addrRec, &nLen))
	//	{
	//		printf("收到Data!\n");
	//		Fa=imdecode(buff,-1);
	//		imshow("reciver", Fa);
	//	}
	//	if (waitKey(5) == 27)//27是键盘摁下esc时，计算机接收到的ascii码值  
	//	{
	//		break;
	//	}
	//}
	//WSACleanup();
	//return 0;
}






