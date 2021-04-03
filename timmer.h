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
		lock_guard<mutex> lck(timmer_locker);//保护
		return Time_Start;
	}

public:
	timmer_inS()
	{
		LARGE_INTEGER Storage;
		QueryPerformanceFrequency(&Storage);
		dfFreq = Storage.QuadPart;// 获得计数器的时钟频率
	}
	void T_start()//开始计时
	{
		LARGE_INTEGER Storage;
		QueryPerformanceCounter(&Storage);
		lock_guard<mutex> lck(timmer_locker);//保护
		Time_Start = Storage.QuadPart;// 获得初始值
	}

	void operator=(timmer_inS& T)//复制时钟初始值（需要在同一系统下）
	{
		lock_guard<mutex> lck(timmer_locker);//保护
		Time_Start = T.copy();
		return;
	}
	double T_now()//输出当前时间间隔（s）
	{
		LARGE_INTEGER L_now;//
		QueryPerformanceCounter(&L_now);//获取时间
		return (double)(L_now.QuadPart - Time_Start) / dfFreq;// 获得对应的时间值，单位为秒

	}





};