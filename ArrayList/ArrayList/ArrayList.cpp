// ArrayList.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

template<class T>
class KCustomArray
{
public:
	KCustomArray(){}
	~KCustomArray()
	{
		if (Arr)
		{
			delete[]Arr;
			Arr = nullptr;

			Length = 0;
			Capacity = 0;
		}
	}
public:
	template<class T>
	void Reserve(int InCapacity)
	{
		if (Capacity > InCapacity)
		{
			return;
		}

		T* NewArr = new T[InCapacity];

		if (Arr)
		{
			for (int i = 0; i < Length; i++)
			{
				NewArr[i] = Arr[i];
			}

			Capacity = InCapacity;

			delete[]Arr;
		}

		Arr = NewArr;
	}

	template<class T>
	void Insert(const T& InValue, unsigned int Pos)
	{
		if (Length == Capacity)
		{
			Reserve<T>(Capacity * 2);
		}

		Length++;

		for (int i = Length; i > Pos; i--)
		{
			Arr[i] = Arr[i - 1];
		}
		Arr[Pos] = InValue;

	}

	template<class T>
	void Push(const T& InValue)
	{
		Insert(InValue, Length);
	}

	template<class T>
	void Remove(unsigned int Pos)
	{
		if (Pos > Length)
		{
			return;
		}

		Length--;

		for (int i = Pos; i < this->Length; i++)
		{
			Arr[i] = Arr[i + 1];
		}

	}

	template<class T>
	void Remove(const T& InValue)
	{
		int Pos = FindIndex(InValue);

		if (Pos >= 0)
		{
			Remove(Pos);
		}
	}

	template<class T>
	T& Pop()
	{
		Length--;
		T& PopValue = Arr[Length];
		return PopValue;

	}

	template<class T>
	void Clear()
	{
		Length = 0;
		Capacity = 0;
	}

	template<class T>
	int FindIndex(const T& InValue)
	{
		int Pos = -1;

		for (int i = 0; i < Length; i++)
		{
			if (Arr[i] == InValue)
			{
				Pos = i;
			}
		}

		return Pos;
	}
private:
	T* Arr = nullptr;
	int Length = 0;
	int Capacity = 0;
};


int main()
{
	auto arr = new KCustomArray<float>();
	arr->Reserve<float>(4);
	arr->Push<float>(1);
	arr->Push<float>(2);
	arr->Insert<float>(1, 3);
	arr->Remove<float>(1.0f);
	arr->Pop<float>();
	arr->Clear<float>();
	int index = arr->FindIndex<float>(1);
}
