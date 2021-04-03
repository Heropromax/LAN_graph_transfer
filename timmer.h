#pragma once

#include<iostream>
#include <windows.h>
#include "Ctime"
#include <mutex>
#include <thread>

using	namespace std;

class timmer_inS
{

private:
	LONGLONG Time_Start = 0;
	LONG dfFreq = 0;
	mutex timmer_locker;

	LONGLONG copy()
	{
		lock_guard<mutex> lck(timmer_locker);//����
		return Time_Start;
	}

public:
	timmer_inS()
	{
		LARGE_INTEGER Storage;
		QueryPerformanceFrequency(&Storage);
		dfFreq = Storage.QuadPart;// ��ü�������ʱ��Ƶ��
	}
	void T_start()//��ʼ��ʱ
	{
		LARGE_INTEGER Storage;
		QueryPerformanceCounter(&Storage);
		lock_guard<mutex> lck(timmer_locker);//����
		Time_Start = Storage.QuadPart;// ��ó�ʼֵ
	}

	void operator=(timmer_inS& T)//����ʱ�ӳ�ʼֵ����Ҫ��ͬһϵͳ�£�
	{
		lock_guard<mutex> lck(timmer_locker);//����
		Time_Start = T.copy();
		return;
	}
	double T_now()//�����ǰʱ������s��
	{
		LARGE_INTEGER L_now;//
		QueryPerformanceCounter(&L_now);//��ȡʱ��
		return (double)(L_now.QuadPart - Time_Start) / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��

	}





};