// StringRealization.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<string.h>

class CustomString
{
public:
	CustomString(const char* InStrPtr = nullptr)
	{
		if (nullptr != InStrPtr)
		{
			StrLength = LenChar(InStrPtr);
			StrPtr = new char[StrLength + 1];
			Strcpy(StrPtr, InStrPtr);
		}
		else
		{
			StrPtr = new char[1];
			*StrPtr = '\0';
		}
	}

	CustomString(const CustomString &InStr)
	{
		StrLength = LenChar(InStr.StrPtr);
		StrPtr = new char[StrLength + 1];
		Strcpy(StrPtr, InStr.StrPtr);
	}

	CustomString(const char* InStr1, const char* InStr2)
	{
		int Len1 = LenChar(InStr1);
		int Len2 = LenChar(InStr2);

		StrLength = Len1 + Len2;

		StrPtr = new char[StrLength + 1];

		char* StrCopy = StrPtr;

		const char* Copy1 = InStr1;

		while (Len1--)
		{
			*StrCopy++ = *Copy1++;
		}
		
		const char* Copy2 = InStr2;

		while (Len2--)
		{
			*StrCopy++ = *Copy2++;
		}
	}

	~CustomString()
	{
		delete[]StrPtr;
		StrPtr = nullptr;
	}

	CustomString& operator = (const CustomString& InStr)
	{
		if (this == &InStr)
		{
			return *this;
		}

		delete[] StrPtr;

		StrLength = LenChar(InStr.StrPtr);
		this->StrPtr = new char[StrLength + 1];
		Strcpy(this->StrPtr, InStr.StrPtr);

		return *this;
	}

	CustomString& operator = (const char* InStrPtr)
	{
		if (this->StrPtr == InStrPtr)
		{
			return *this;
		}

		delete[] StrPtr;

		StrLength = LenChar(InStrPtr);
		this->StrPtr = new char[StrLength + 1];
		Strcpy(this->StrPtr, InStrPtr);

		return *this;
	}

	CustomString operator + (const CustomString& InStr)
	{
		CustomString Temp(this->StrPtr, InStr.StrPtr);

		return Temp;
	}

	bool operator == (const CustomString& InStr)
	{
		int Len1 = LenChar(this->StrPtr);
		int Len2 = LenChar(InStr.StrPtr);

		if (Len1 != Len2)
		{
			return false;
		}

		const char* CopyStr1 = StrPtr;
		const char* CopyStr2 = InStr.StrPtr;

		while (Len1--)
		{
			if (*CopyStr1++ != *CopyStr2++)
			{
				return false;
			}
		}

		return true;
	}

	char& operator[](int Index)
	{
		if (Index >= StrLength)
		{
			throw 1;
		}
		return this->StrPtr[Index];
	}

	unsigned int Len()
	{
		return StrLength;
	}

	CustomString Sub(unsigned int Start, unsigned int End)
	{
		if (End < Start || LenChar(StrPtr) < Start)
		{
			return CustomString();
		}

		if (Start == End)
		{
			const char* TempPtr = StrPtr;
			char Temp = '\0';

			while (Start-- && *TempPtr++);

			Temp = *TempPtr;

			return CustomString(&Temp);
		}

		const char* CopyPtr = StrPtr;

		while (Start-- && *CopyPtr++);

		int SelectLen = End - Start;
		char* NewStrPtr = new char[SelectLen + 1];
		char* TempPtr = NewStrPtr;
		while (SelectLen--)
		{
			*TempPtr++ = *CopyPtr++;
		}

		CustomString Temp(NewStrPtr);

		delete[]NewStrPtr;

		return Temp;
	}

	void Append(const CustomString& InStr)
	{
		int Len1 = LenChar(this->StrPtr);
		int Len2 = LenChar(InStr.StrPtr);

		char* TempPtr = StrPtr;
		StrPtr = new char[Len1 + Len2 + 1];

		char* StrCopy = StrPtr;

		const char* Copy1 = TempPtr;

		while (Len1--)
		{
			*StrCopy++ = *Copy1++;
		}

		const char* Copy2 = InStr.StrPtr;

		while (Len2--)
		{
			*StrCopy++ = *Copy2++;
		}

		delete[]TempPtr;
	}

	int Find(const char* InStrPtr)
	{
		int Len1 = LenChar(this->StrPtr);
		int Len2 = LenChar(InStrPtr);

		if (Len2 > Len1)
		{
			return -1;
		}

		for (int i = 0; i < Len1; ++i)
		{
			bool bIsMatch = true;
			for (int ii = 0; ii < Len2; ++ii)
			{
				if (this->StrPtr[i + ii] != InStrPtr[ii])
				{
					bIsMatch = false;
					break;
				}
			}
			if (bIsMatch)
			{
				return i;
			}
		}
		return -1;
	}

	CustomString* Split(const char* InStrPtr)
	{
		if (Find(InStrPtr) < 0)
		{
			return this;
		}

		int Length = LenChar(InStrPtr);

		if (Length <= 0)
		{
			return this;
		}

		const char Search = InStrPtr[0];

		int Size = 0;

		for (int i = 0; i < Len(); i++)
		{
			if (StrPtr[i] == Search)
			{
				Size++;
			}
		}

		CustomString* ReturnStr = new CustomString[Size];
		CustomString* TempStr = ReturnStr;

		for (int i = 0; i < Len(); i++)
		{
			if (StrPtr[i] == Search)
			{
				TempStr++;
			}
			else if(TempStr)
			{
				TempStr->Append(&StrPtr[i]);
			}
		}

		return ReturnStr;
	}

	

private:
	unsigned int LenChar(const char* InStr)
	{
		const char* Temp = InStr;

		while (*Temp++);

		return (Temp - InStr - 1);
	}

	void Strcpy(char* InStr1, const char* InStr2)
	{
		int Len1 = LenChar(InStr1);
		int Len2 = LenChar(InStr2);

		int Temp = Len2;

		if (Len1 < Temp)
		{
			Temp = Len1;
		}

		while (Temp--)
		{
			*InStr1++ = *InStr2++;
		}
		
	}
private:
	char* StrPtr = nullptr;
	int StrLength = 0;
};

int main()
{
	auto str1 = CustomString("test1");

	auto str2 = CustomString("test2", "test3");


	str1 = "test3";

	int len = str1.Len();
	CustomString sub1 = str1.Sub(1, 2);

	for (int i = 0; i < 2; i++)
	{
		std::cout << sub1[i] << std::endl;
	}

	str1.Append("append");
	bool equal = str1 == str2;
	int index = str1.Find("es");
	CustomString* ret = str2.Split(",");

	return 0;
}