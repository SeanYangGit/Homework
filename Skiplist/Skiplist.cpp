// Skiplist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>

#define Skip_Level_Prob 0.5f
#define Max_Level 32

struct KSkipNode
{
public:
	KSkipNode(int InValue) :Value(InValue)
	{
		Next.resize(Max_Level, nullptr);
	}

public:
	int Value;

	std::vector<KSkipNode*> Next;
};

class KSkiplist
{
public:
	KSkiplist();
	~KSkiplist();

	bool Search(int InValue);
	void Add(int InValue);
	bool Erase(int InValue);

private:
	void Find(int InValue, std::vector<KSkipNode*>& OutNodes);

private:
	KSkipNode* Hand = nullptr;
};

KSkiplist::KSkiplist()
{
	Hand = new KSkipNode(-99999);
}

KSkiplist::~KSkiplist()
{
	delete Hand;
}

bool KSkiplist::Search(int InValue)
{
	std::vector<KSkipNode*> FindNodes(Max_Level);
	Find(InValue, FindNodes);

	return FindNodes[0]->Next[0]->Value == InValue;
}

void KSkiplist::Add(int InValue)
{
	std::vector<KSkipNode*> FindNodes(Max_Level);
	Find(InValue, FindNodes);

	auto NewNode = new KSkipNode(InValue);

	for (int i = 0; i < Max_Level; i++)
	{
		NewNode->Next[i] = FindNodes[i]->Next[i];
		FindNodes[i]->Next[i] = NewNode;

		if (rand() / RAND_MAX > 0.5f)
		{
			break;
		}
	}
}

bool KSkiplist::Erase(int InValue)
{
	std::vector<KSkipNode*> FindNodes(Max_Level);
	Find(InValue, FindNodes);

	auto FirstFind = FindNodes[0]->Next[0];

	if (FirstFind != nullptr && FirstFind->Value != InValue)
	{
		return false;
	}

	for (int i = 0; i < Max_Level && FindNodes[i]->Next[i] == FirstFind; i++)
	{
		FindNodes[i]->Next[i] = FirstFind->Next[i];
	}

	delete FirstFind;

	return true;
}

void KSkiplist::Find(int InValue, std::vector<KSkipNode*>& OutNodes)
{
	auto CurrentNode = Hand;

	for (int i = Max_Level - 1; i >= 0; i--)
	{
		while (CurrentNode->Next[i] && CurrentNode->Next[i]->Value < InValue)
		{
			CurrentNode = CurrentNode->Next[i];
		}

		OutNodes[i] = CurrentNode;
	}
}

int main()
{
	KSkiplist skiplist;
	skiplist.Add(1);
	skiplist.Add(2);
	skiplist.Add(3);
	std::cout << "Find 0: " << skiplist.Search(0) << std::endl;
	skiplist.Add(4);
	std::cout << "Find 1: " << skiplist.Search(1) << std::endl;
	skiplist.Erase(0);
	skiplist.Erase(1);
	std::cout << "Find 1: " << skiplist.Search(1) << std::endl;
}