// RingBuffer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>

template<class T>
class RingBuffer
{
public:
	RingBuffer(unsigned int InSize):Size(InSize),Front(0),Rear(0)
	{
		Data = new T[Size];
	}
	~RingBuffer()
	{
		delete[]Data;
		Data = nullptr;
	}

	bool IsEmpty()const
	{
		return Front == Rear;
	}

	bool IsFull() const
	{
		return Front == (Rear + 1) % Size;
	}

	bool Push(const T& InValue)
	{
		if (IsFull())
		{
			Expand();
		}
		Data[Rear] = InValue;
		Rear = (Rear + 1) % Size;

		return true;
	}

	bool Pop(T& InValue)
	{
		if (IsEmpty())
		{
			return false;
		}

		InValue = Data[Front];
		Front = (Front + 1) % Size;

		reduce();
		return true;
	}

	unsigned int GetFront() const
	{
		return Front;
	}

	unsigned int GetRear() const
	{
		return Rear;
	}
private:
	void Expand()
	{
		unsigned int PerSize = (Size * 2) < 1024 ? (unsigned int)(Size * 2) : (unsigned int)(Size * 1.25f);
		T* NewData = new T[PerSize];

		for (int i = 0; i < Size; i++)
		{
			NewData[i] = Data[i];
		}

		delete[]Data;
		Data = NewData;

		Size = PerSize;
	}

	void reduce()
	{
		unsigned int PerSize = Size / 2;

		if ((Rear - Front) >= PerSize)
		{
			return;
		}

		Size = PerSize;

		T* NewData = new T[Size];

		int i = 0;

		for (int ii = Front; ii <= Rear; ii++)
		{
			NewData[i] = Data[ii];
			i++;
		}

		Front = 0;
		Rear = i - 1;

		delete[]Data;
		Data = NewData;
	}
private:
	unsigned int Size;
	int Front; // 队头
	int Rear; // 队尾
	T* Data;
};


int main()
{
	int initSize = 256;
	const char* data = "123456789";
	auto buffer = new RingBuffer<char>(initSize);
	srand(1);
	for (int ii = 0; ii < 1000; ii++)
	{
		int pushCount = rand() % 10;

		for (int jj = 0; jj < pushCount; jj++)
		{
			char temp = jj + '0';
			buffer->Push(temp);    // 要支持自动扩容
		}

		char temp = '0';
		int popCount = rand() % 10;
		for (int jj = 0; jj < popCount; jj++) buffer->Pop(temp);   // 要支持自动缩容
	}
}