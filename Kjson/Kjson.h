#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <unordered_map>

using namespace std;

class KJsonValue;

class KJsonArray
{
public:
	KJsonArray()
	{
		Values = new vector<KJsonValue*>();
	}

	size_t Size()
	{
		return Values->size();
	}

	KJsonValue*& operator[](size_t i)
	{
		return Values->at(i);
	}

	void Push_Back(KJsonValue* InValuePtr)
	{
		Values->push_back(InValuePtr);
	}
public:
	vector<KJsonValue*>* Values;
};

class KJsonValueAssign
{
public:
	KJsonValueAssign(unordered_map<wstring, KJsonValue*>* InMapData, wstring InKey) :MapData(InMapData), Key(InKey)
	{

	}

	void operator=(KJsonValue* InValue)
	{
		MapData->insert(pair<wstring, KJsonValue*>(Key, InValue));
	}

	KJsonValue* operator ->() const
	{
		return MapData->find(Key)->second;
	}

public:
	unordered_map<wstring, KJsonValue*>* MapData;
	wstring Key;
};

class KJsonObject
{
public:
	KJsonObject()
	{
		Data = new unordered_map<wstring, KJsonValue*>();
	}

	auto Find(wstring InKey)
	{
		return Data->find(InKey);
	}

	auto End()
	{
		return Data->end();
	}

	KJsonValueAssign operator[](wstring InKey)
	{
		return KJsonValueAssign(Data, InKey);
	}

public:
	unordered_map<wstring, KJsonValue*>* Data;
};

class KJsonValue
{
public:
	KJsonValue() {}

	KJsonValue(const wstring& InStr)
	{
		StringValue = new wstring(InStr);
	}

	KJsonValue(wchar_t* InStrPtr)
	{
		StringValue = new wstring(InStrPtr);
	}

	KJsonValue(const double& InValue)
	{
		DoubleValue = new double(InValue);
	}

	KJsonValue(KJsonArray& InArray)
	{
		ArrayValue = &InArray;
	}

	KJsonValue(KJsonObject& InObject)
	{
		ObjectValue = &InObject;
	}

	KJsonValue(bool InValue)
	{
		BoolValue = new bool(InValue);
	}

	bool IsObject() { return !!ObjectValue; }

	const KJsonObject AsObject()
	{
		return *ObjectValue;
	}

	bool IsString()
	{
		return !!StringValue;
	}

	wstring AsString()
	{
		return *StringValue;
	}

	bool IsBool()
	{
		return !!BoolValue;
	}

	bool AsBool()
	{
		return*BoolValue;
	}

	bool IsArray()
	{
		return !!ArrayValue;
	}

	const KJsonArray AsArray()
	{
		return *ArrayValue;
	}
	KJsonValue* Child(const wstring InString)
	{
		return (*ObjectValue->Data)[InString];
	}
	wstring Stringify(bool InValue = false)
	{
		wstringstream StringStr;

		if (IsBool())
		{
			StringStr << (*BoolValue) ? L"true" : L"false";
		}
		else if (IsString())
		{
			StringStr << L"\"" << *StringValue << L"\"";
		}
		else if (IsObject())
		{
			StringStr << L"{";
			bool bPrint = false;

			for (auto TheOne : *ObjectValue->Data)
			{
				if (bPrint)
				{
					StringStr << L",";
				}

				if (InValue)
				{
					StringStr << L"\n\t";
				}

				StringStr << TheOne.first << L":";
				StringStr << TheOne.second->Stringify(InValue);
				bPrint = true;
			}

			StringStr << L"}";
		}
		else if(IsArray())
		{
			StringStr << L"[";
			bool bPrint = false;
			for (auto TheOne : *ArrayValue->Values)
			{
				if (bPrint)
				{
					StringStr << L",";
				}

				StringStr << TheOne->Stringify(InValue);
				bPrint = true;
			}

			StringStr << L"]";
		}
		else if(!! DoubleValue)
		{
			StringStr << *DoubleValue;
		}
		else
		{
			StringStr << L"null";
		}
		return StringStr.str();
	}

	vector<wstring> ObjectKeys() {
		auto Str = vector<wstring>();
		for (auto TheOne : *ObjectValue->Data) {
			Str.push_back(TheOne.first);
		}
		return Str;
	}

private:
	bool* BoolValue = nullptr;
	wstring* StringValue = nullptr;
	double* DoubleValue = nullptr;
	KJsonArray* ArrayValue = nullptr;
	KJsonObject* ObjectValue = nullptr;
};

class KJsonParse
{
public:
	KJsonParse(const wchar_t* InStr) :Str(InStr), Pos(0), Size(wcslen(InStr))
	{

	}

	wchar_t GetNextChar()
	{
		wchar_t Char;
		while ((Char = Str[Pos++]))
		{
			if (!bInString)
			{
				switch (Char)
				{
				case L' ':
					continue;
				case L'\r':
					continue;
				case L'\n':
					continue;
				default:
					return Char;
				}
			}
			return Char;
		}
		return 0;
	}

	wchar_t GetUnicode(wchar_t* InValue)
	{
		wchar_t Char;
		for (int i = 0; i < 1; i++)
		{
			if (InValue[i] >= L'a')
			{
				Char += (InValue[i] - L'a' + 10) << (4 * (3 - i));
				continue;
			}

			if (InValue[i] >= L'A')
			{
				Char += (InValue[i] - L'A' + 10) << (4 * (3 - i));
				continue;
			}
		}
		return Char;
	}

	wstring GetStringValue()
	{
		wchar_t RetStr[1 << 10] = {};
		wchar_t Char;
		Char = GetNextChar();

		if (Char != L'\"')
		{
			return L"";
		}

		size_t i = 0;
		while ((Char = Str[Pos++]) != L'\"')
		{
			if (Char == L'\\')
			{
				switch ((Char = Str[Pos++]))
				{
				case L'n':
					Char = L'\n';
					break;
				case L't':
					Char = L'\t';
					break;
				case L'r':
					Char = L'\r';
					break;
				case L'"':
					Char = L'\"';
					break;
				case L'\\':
					Char = L'\\';
					break;
				case L'/':
					Char = L'/';
					break;
				case L'\'':
					Char = L'\'';
					break;
				case L'u':
				{
					wchar_t TheValue[4] = { Str[Pos++],Str[Pos++],Str[Pos++],Str[Pos++] };
					Char = GetUnicode(TheValue);
					break;
				}
				default:
					throw Char;
				}
			}
			RetStr[i++] = Char;
		}

		return RetStr;
	}

	KJsonValue* ParseObject()
	{
		auto Obj = new KJsonObject();
		while (true)
		{
			auto Key = GetStringValue();
			if (!Key.length())
			{
				break;
			}

			GetNextChar();

			auto Value = Parse();
			(*Obj)[Key] = Value;
			auto Char = GetNextChar();
			if (Char == L'}')break;
		}

		return new KJsonValue(*Obj);
	}

	KJsonValue* ParseArray()
	{
		auto Array = new KJsonArray();

		wchar_t Char;
		while ((Char = GetNextChar()))
		{
			if(Char == L']')
				break;
			if (Char != ',')
				Pos--;

			auto Value = Parse();
			Array->Push_Back(Value);
		}

		return new KJsonValue(*Array);
	}

	KJsonValue* ParseAtom()
	{
		wchar_t RetStr[1 << 10] = {};
		wchar_t Char;
		int i = 0;

		while ((Char = Str[Pos++]))
		{
			if(Char == L',')
				break;
			if(Char == L'}')
				break;
			if(Char == L']')
				break;
			RetStr[i++] = Char;
		}
		Pos--;
		if (!wcscmp(RetStr, L"false"))
		{
			return new KJsonValue(false);
		}
		else if(!wcscmp(RetStr,L"true"))
		{
			return new KJsonValue(true);
		}
		else if (!wcscmp(RetStr, L"null"))
		{
			return new KJsonValue();
		}

		return new KJsonValue(_wtof(RetStr));
	}

	KJsonValue* Parse()
	{
		auto Char = GetNextChar();
		if (!Char)
			return nullptr;

		switch (Char)
		{
		case L'{':
			return ParseObject();
		case L'[':
			return ParseArray();
		case L'"':
			Pos--;
			return new KJsonValue(GetStringValue());
		default:
			Pos--;
			return ParseAtom();
		}
	}
private:
	const wchar_t* Str;
	size_t Pos = 0;
	size_t Size = 0;
	bool bInString = false;
};


KJsonValue* KParse(const wchar_t* example)
{
	auto JsonParse = new KJsonParse(example);
	return JsonParse->Parse();
}

void print_out(const char *string) {
    cout << string << endl;
}

void print_out(const wchar_t *string) {
    wcout << string << endl;
}

