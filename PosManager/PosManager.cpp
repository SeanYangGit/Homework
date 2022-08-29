// PosManager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <map>
#include <vector>

using namespace std;
struct PosNode 
{
    PosNode():ID(0),X(0),Y(0)
    {
    }

    PosNode(int InID, int InX, int InY)
    {
        ID = InID;
        X = InX;
        Y = InY;
    }
public:
    int ID;
    int X, Y;
};

class PosManager
{
public:
    PosManager(int InBlockSize, int InMapSize);
	~PosManager();

    void Add(int InID, int InX, int InY);

    vector<int> Find(int ID);
    void Update(int InID, int InX, int InY);
    void Remove(int InID);

private:
    int GetBlockIdByPos(int InX, int InY);
    vector<int> GetAllSurroundingBlock(int InBolckId);
    bool IsValidPos(int InX, int InY);

private:
    map<int, map<int, PosNode>> IDs;
    map<int, int> PosToBlock;
    int BlockSize;
    int MapSize;
};


PosManager::PosManager(int InBlockSize, int InMapSize)
{
    BlockSize = InBlockSize;
    MapSize = InMapSize;
}

PosManager::~PosManager()
{
}

void PosManager::Add(int InID, int InX, int InY)
{
    if (!IsValidPos(InX, InY))
    {
        return;
    }

    if (PosToBlock.count(InID) > 0)
    {
        return;
    }

    int BlockID = GetBlockIdByPos(InX, InY);

    IDs[BlockID][InID] = PosNode(InID, InX, InY);

    PosToBlock[InID] = BlockID;
}

vector<int> PosManager::Find(int ID)
{
    vector<int> FindPos;
    if (PosToBlock.count(ID) == 0)
    {
        return FindPos;
    }

    int CurrentID = PosToBlock.at(ID);
    vector<int> SurroundingBlocks = GetAllSurroundingBlock(CurrentID);

    for (int TheID : SurroundingBlocks)
    {
        if (IDs.count(TheID) == 0)
        {
            continue;
        }

        for (const auto& LocalData : IDs.at(TheID))
        {
            FindPos.emplace_back(LocalData.first);
        }
    }

    return FindPos;
}

void PosManager::Update(int InID, int InX, int InY)
{
    if (!IsValidPos(InX, InY))
    {
        return;
    }

    if (PosToBlock.count(InID) == 0)
    {
        return;
    }

    int BlockID = PosToBlock.at(InID);
    PosNode* PosNodePtr = &IDs[BlockID][InID];
    PosNodePtr->X = InX;
    PosNodePtr->Y = InY;

    int LocalBlockID = GetBlockIdByPos(InX, InY);

    if (BlockID != LocalBlockID)
    {
        IDs[BlockID].erase(InID);
        IDs[LocalBlockID][InID] = *PosNodePtr;
    }
}

void PosManager::Remove(int InID)
{
    if (PosToBlock.count(InID) == 0)
    {
        return;
    }

    int LocalBlockID = PosToBlock[InID];
    IDs[LocalBlockID].erase(InID);
    PosToBlock.erase(InID);
}

int PosManager::GetBlockIdByPos(int InX, int InY)
{
    return (InX / BlockSize) * (MapSize / BlockSize) + (InY / BlockSize);
}

vector<int> PosManager::GetAllSurroundingBlock(int InBolckId)
{
    vector<int> AllIDs;

    AllIDs.push_back(InBolckId - MapSize / BlockSize - 1);
    AllIDs.push_back(InBolckId - MapSize / BlockSize);
    AllIDs.push_back(InBolckId - MapSize / BlockSize + 1);
    AllIDs.push_back(InBolckId - 1);
    AllIDs.push_back(InBolckId);
    AllIDs.push_back(InBolckId + 1);
    AllIDs.push_back(InBolckId + MapSize / BlockSize - 1);
    AllIDs.push_back(InBolckId + MapSize / BlockSize);
    AllIDs.push_back(InBolckId + MapSize / BlockSize + 1);

    return AllIDs;
}

bool PosManager::IsValidPos(int InX, int InY)
{
    return (InX >= 0) && (InX < MapSize) && (InY >= 0) && (InY < MapSize);
}

int main()
{
	int blockSize = 100;
	int mapSize = 10000;
    auto m = new PosManager(blockSize, mapSize);

	int poseId1 = 1;
	int poseId2 = 2;
	m->Add(poseId1, 100, 200);  // 100是横坐标，200纵坐标
	m->Add(poseId2, 200, 300);
	vector<int> poseIds = m->Find(poseId1);    // 找到poseid1脚下以及周围8格的所有id
	m->Update(poseId1, 300, 400);
	m->Remove(poseId2);
}
