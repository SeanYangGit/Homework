#include <iostream>
#include <vector>
#include <string>
#include <map>


using namespace std;

class Command;

enum class ArgType
{
	None,
	Required,
	Option
};

class Command_t
{
public:
	Command_t(const char* InShorArg):ShortArg(InShorArg){};

public:
	const char* Arg = nullptr;
	string ShortArg;
	string FullArg;
	string Desc;
	ArgType RequireArg = ArgType::None;
	Command* CommandPtr = nullptr;

	void (*func)(Command_t*);
};

class Command
{
public:
	Command(string InName, string InVersion)
	{
		ArgMap = new map<string, Command_t*>();
		ParsedArgs = new vector<const char*>();
	};

	static Command* Self(Command_t* pCommand)
	{
		return pCommand->CommandPtr;
	}

	void SetUsage(string InStr)
	{
		Usage = InStr;
	}

	void Option(const char* InShortArg, const char* InFullArg, const char* ArgDesc, void (*InParam)(Command_t*))
	{
		auto Cmd = new Command_t(InShortArg);
		if (strstr(InFullArg, " <"))
		{
			Cmd->RequireArg = ArgType::Required;
			auto Str = string(InFullArg);
			Cmd->FullArg = Str.substr(0, Str.find(" <") - 1);
		}
		else if(strstr(InFullArg," ["))
		{
			Cmd->RequireArg = ArgType::Option;
			auto Str = string(InFullArg);
			Cmd->FullArg = Str.substr(0, Str.find(" [") - 1);
		}
		else
		{
			Cmd->FullArg = InFullArg;
		}

		Cmd->func = InParam;
		Cmd->Desc = ArgDesc;
		Cmd->CommandPtr = this;

		(*ArgMap)[InShortArg] = Cmd;
		(*ArgMap)[Cmd->FullArg.c_str()] = Cmd;
	}

	void Parse(int Argc, char** Argv)
	{
		for (int i = 0; i < Argc; i++)
		{
			auto Command = (*ArgMap)[Argv[i]];
			if (Command)
			{
				ParsedArgs->push_back(Command->ShortArg.c_str());
				switch (Command->RequireArg)
				{
				case ArgType::None:
					Command->func(Command);
					break;
				default:
					if (i + 1 == Argc)
					{
						cerr << "Missing arg" << endl;
						return;
					}
					Command->Arg = Argv[i++];
					Command->func(Command);
					break;
				}
			}
		}
	}

	const vector<const char*> AdditionalArgs()
	{
		return *ParsedArgs;
	}

	void Help()
	{
		cout << Usage << endl;
	}
public:
	map<string, Command_t*>* ArgMap;
	vector<const char*>* ParsedArgs;


private:
	string Name;
	string Version;
	string Usage;
};