// RingBuffer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#define DOUBLE_EXPAND_SIZE = 1024;

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
			return false;
		}
		Data[Rear] = value;
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
		Size = Size * 2 <= DOUBLE_EXPAND_SIZE ? Size * 2 : Size * 1.25;
		T* NewData = new T[Size];

		for (int i = 0; i < Size; i++)
		{
			NewData[i] = Data[i];
		}

		delete[]Data;
		Data = NewData;
	}

	void reduce()
	{
		if ((Rear - Front) > Size / 2)
		{
			return;
		}
	}
private:
	unsigned int Size;
	int Front; // 队头
	int Rear; // 队尾
	T* Data;
};


int main()
{
    std::cout << "Hello World!\n";
}