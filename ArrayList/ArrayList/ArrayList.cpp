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

template<class T>
struct LinkNode
{
public:
	T value;
	LinkNode* Per;
	LinkNode* Next;

public:
	LinkNode(){}
	LinkNode(T InValue, LinkNode* InPer, LinkNode* InNext)
	{
		value = InValue;
		Per = InPer;
		Next = InNext
	}
};

template<class T>
class DoubleLink
{
public:
	DoubleLink();
	~DoubleLink();

	int Size();
	int Is_Empty();


	T Get(int InIndex);
	T Get_First();
	T Get_Last();

	LinkNode<T>* Insert(int InIndex, T InValue);
	LinkNode<T>* Insert_First(T InValue);
	LinkNode<T>* Append_Last(T InValue);

	void DeleteItem(int InIndex);
	void DeleteItem_First();
	void DeleteItem_Last();

private:
	int Count;
	LinkNode<T>* HeadPtr = nullptr;

private:
	LinkNode<T>* Get_Node(int InIndex);
};

template<class T>
DoubleLink::DoubleLink():Count(0)
{
	HeadPtr = new LinkNode<T>();
	HeadPtr->Per = HeadPtr->Next = HeadPtr;
}

template<class T>
DoubleLink::~DoubleLink()
{
	LinkNode<T>* TempPtr = nullptr;
	LinkNode<T>* NodePtr = HeadPtr->Next;

	while (NodePtr != HeadPtr)
	{
		TempPtr = NodePtr;
		NodePtr = NodePtr->Next;
		delete TempPtr;
	}

	delete HeadPtr;
	HeadPtr = nullptr;
}

template<class T>
LinkNode<T>* DoubleLink<T>::Get_Node(int InIndex)
{
	if (InIndex < 0 || InIndex >= Count)
	{
		count << "Can not find node by index!!!" << endl;
		return nullptr;
	}

	if (InIndex <= Count / 2)
	{
		int i = 0;
		LinkNode<T>* IndexNodePtr = HeadPtr->Next;
		while (i++ < InIndex)
		{
			IndexNodePtr = IndexNodePtr->Next;
		}

		return IndexNodePtr;
	}

	int i = 0;
	int RIndex = Count - InIndex - 1;
	LinkNode<T>* IndexNodePtr = HeadPtr->Per;
	while (i++ < RIndex)
	{
		IndexNodePtr = IndexNodePtr->Per;
	}

	return IndexNodePtr;
}

template<class T>
void DoubleLink<T>::DeleteItem_Last()
{
	DeleteItem(Count - 1);
}

template<class T>
void DoubleLink<T>::DeleteItem_First()
{
	DeleteItem(0);
}

template<class T>
void DoubleLink<T>::DeleteItem(int InIndex)
{
	LinkNode<T>* IndexNodePtr = Get_Node(InIndex);
	IndexNodePtr->Next->Per = IndexNodePtr->Per;
	IndexNodePtr->Per->Next = IndexNodePtr->Next;
	delete IndexNodePtr;
	Count--;
}

template<class T>
LinkNode<T>* DoubleLink<T>::Append_Last(T InValue)
{
	LinkNode NewNodePtr = new LinkNode<T>(InValue, HeadPtr->Per, HeadPtr);
	HeadPtr->Per->Next = NewNodePtr;
	HeadPtr->Per = NewNodePtr;
	Count++;
	return NewNodePtr;
}

template<class T>
LinkNode<T>* DoubleLink<T>::Insert_First(T InValue)
{
	LinkNode<T>* NewNodePtr = new LinkNode<T>(InValue, HeadPtr, HeadPtr->Next);
	HeadPtr->Next->Per = NewNodePtr;
	HeadPtr->Next = NewNodePtr;
	count++;
	return NewNodePtr;
}

template<class T>
LinkNode<T>* DoubleLink<T>::Insert(int InIndex, T InValue)
{
	if (InIndex == 0)
	{
		return Insert_First(InValue);
	}

	LinkNode<T>* IndexNodePtr = Get_Node(InIndex);
	LinkNode<T>* NewNodePtr = new LinkNode<T>(InValue, IndexNodePtr->Per, IndexNodePtr->Next);

	IndexNodePtr->Per->Next = NewNodePtr;
	IndexNodePtr->Per = NewNodePtr;

	Count++;
	return NewNodePtr;
}

template<class T>
T DoubleLink<T>::Get_Last()
{
	return Get(Count - 1);
}

template<class T>
T DoubleLink<T>::Get_First()
{
	return Get(0);
}

template<class T>
T DoubleLink<T>::Get(int InIndex)
{
	auto NodePtr = Get_Node(InIndex);

	return NodePtr != nullptr ? NodePtr->value : T();
}

template<class T>
int DoubleLink<T>::Is_Empty()
{
	return Count == 0;
}

template<class T>
int DoubleLink<T>::Size()
{
	return Count;
}

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

	auto list1 = new DoubleLink<int>();
	auto node1 = list1->Insert_First(1);
	auto node2 = list1->Insert_First(2);
	auto node3 = list1->Insert(2, 3);
	auto ret = list1->Get(2);
	list1->DeleteItem(2);

}
