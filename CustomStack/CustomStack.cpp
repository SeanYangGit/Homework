// CustomStack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <stack>

using namespace std;

class StackData
{
public:
	StackData()
	{
		Data = nullptr;
	}
	~StackData()
	{
		if(Data != nullptr)
		delete Data;

		Data = nullptr;
	}

	template<typename  T>
	void Init(const T& InValue)
	{
		TypeName = typeid(T).name();

		T* NewValue = new T();
		*NewValue = InValue;

		Data = (void*)NewValue;
	}

	template<typename  T>
	bool GetValue(T& OutValue)
	{
		if (std::string(typeid(T).name()) != TypeName)
		{
			return false;
		}

		T* ItemPtr = (T*)Data;
		OutValue = *ItemPtr;

		return true;
	}
private:
	string TypeName;
	void* Data;
};

class StackInfo
{
public:
	StackInfo();
	~StackInfo();

	template<typename  T>
	void push(const T& InValue)
	{
		StackDataInfo.push(StackData());
		StackData& NewData = StackDataInfo.top();
		NewData.Init(InValue);
	}

	template<typename  T>
	bool pop(T& OutValue)
	{
		StackData& TopItem = StackDataInfo.top();
		if (!TopItem.GetValue(OutValue))
		{
			return false;
		}

		StackDataInfo.pop();
		return true;
	}
private:
	stack<StackData> StackDataInfo;
};

StackInfo::StackInfo()
{
}

StackInfo::~StackInfo()
{
}

class Test
{
public:
	int p1 = 1;
	bool p2 = false;
	std::string p3 = "p33333333";

	void testWithStack(StackInfo& stack)
	{
		int v1 = 0;
		std::string v2;
		Test v3;
		Test* v4 = nullptr;

		stack.pop<Test*>(v4);
		stack.pop(v3);
		stack.pop(v2);
		stack.pop(v1);

		std::cout << "testWithStack "
			<< v1 << " "
			<< v2 << " "
			<< v3.p3 << " "
			<< v4->p3 << std::endl;

		stack.push(1);
	}
};

void test2()
{
	Test t1;

	StackInfo stack;  // 实现此类，就俩模板函数，push和pop
	stack.push(111);
	stack.push<string>("asdfasdfasdf");

	Test t2;
	t2.p3 = "t2teststring";
	stack.push(t2);
	stack.push(&t1);

	t1.testWithStack(stack);

	int ret = 0;
	stack.pop(ret);
	std::cout << "testWithStack ret " << ret << std::endl;
}

int main()
{
	test2();
}
