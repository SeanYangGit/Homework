// Octree.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>

struct KPoint
{
	KPoint()
	{
		X = Y = Z = -1;
	}
	KPoint(float InX, float InY, float InZ)
	{
		X = InX;
		Y = InY;
		Z = InZ;
	}
	KPoint(const KPoint& Other)
	{
		X = Other.X;
		Y = Other.Y;
		Z = Other.Z;
	}

	~KPoint() {}

public:
	float GetX() const { return X; }
	float GetY() const { return Y; }
	float GetZ() const { return Z; }

	KPoint operator+(const KPoint& Other) {
		return KPoint(X + Other.X, Y + Other.Y, Z + Other.Z);
	}
	KPoint operator-(const KPoint& Other) {
		return KPoint(X - Other.X, Y - Other.Y, Z - Other.Z);
	}


public:
	bool IsSet = false;

private:
	float X, Y, Z;
};

bool IsBoxOverLap(const KPoint& InMin, const KPoint& InMax, const KPoint& InOtherMin, const KPoint& InOtherMaX) {
	bool X_overlap = !(InMax.GetX() < InOtherMin.GetX() || InOtherMaX.GetX() < InMin.GetX());
	bool Y_overlap = !(InMax.GetY() < InOtherMin.GetY() || InOtherMaX.GetY() < InMin.GetY());
	bool Z_overlap = !(InMax.GetZ() < InOtherMin.GetZ() || InOtherMaX.GetZ() < InMin.GetZ());
	return X_overlap && Y_overlap && Z_overlap;
}



class KOctree
{
public:
	KOctree(const KPoint& InCenter, float InWidth, float InHeight, float InDepth)
	{
		Width = InWidth;
		Height = InHeight;
		Depth = InDepth;
		CenterPoint = InCenter;

		for (int i = 0; i < 8; i++)
		{
			ChildrenNode[i] = nullptr;
		}
	}
	~KOctree(){}

	bool IsHasChildren() { return ChildrenNode[0] != nullptr; }

	bool IsInSlef(const KPoint& point)
	{
		return
			point.GetX() > CenterPoint.GetX() - Depth/2.0f &&
			point.GetX() < CenterPoint.GetX() + Depth/2.0f &&
			point.GetY() > CenterPoint.GetY() - Width/2.0f &&
			point.GetY() < CenterPoint.GetY() + Width/2.0f &&
			point.GetZ() > CenterPoint.GetZ() - Height/2.0f &&
			point.GetZ() < CenterPoint.GetZ() + Height/2.0f;

	}

	void insert(const KPoint& point)
	{
		if (IsHasChildren())
		{
			for (int i = 0; i < 8; i++)
			{
				if (ChildrenNode[i] != nullptr && ChildrenNode[i] - IsInSlef(point))
				{
					ChildrenNode[i]->insert(point);
				}
			}
		}
		else
		{
			if (DataPoint.IsSet)
			{
					float HalfWidth = Width / 2.0f;
					float HalfHeight = Height / 2.0f;
					float HalfDepth = Depth / 2.0f;

					ChildrenNode[0] = new KOctree(
						KPoint(
							CenterPoint.GetX() + HalfDepth / 2.0f,
							CenterPoint.GetY() + HalfWidth / 2.0f,
							CenterPoint.GetZ() + HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					ChildrenNode[1] = new KOctree(
						KPoint(
							CenterPoint.GetX() - HalfDepth / 2.0f,
							CenterPoint.GetY() + HalfWidth / 2.0f,
							CenterPoint.GetZ() + HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					ChildrenNode[2] = new KOctree(
						KPoint(
							CenterPoint.GetX() - HalfDepth / 2.0f,
							CenterPoint.GetY() - HalfWidth / 2.0f,
							CenterPoint.GetZ() + HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					ChildrenNode[3] = new KOctree(
						KPoint(
							CenterPoint.GetX() + HalfDepth / 2.0f,
							CenterPoint.GetY() - HalfWidth / 2.0f,
							CenterPoint.GetZ() + HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					ChildrenNode[4] = new KOctree(
						KPoint(
							CenterPoint.GetX() + HalfDepth / 2.0f,
							CenterPoint.GetY() + HalfWidth / 2.0f,
							CenterPoint.GetZ() - HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					ChildrenNode[5] = new KOctree(
						KPoint(
							CenterPoint.GetX() - HalfDepth / 2.0f,
							CenterPoint.GetY() + HalfWidth / 2.0f,
							CenterPoint.GetZ() - HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					ChildrenNode[6] = new KOctree(
						KPoint(
							CenterPoint.GetX() - HalfDepth / 2.0f,
							CenterPoint.GetY() - HalfWidth / 2.0f,
							CenterPoint.GetZ() - HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					ChildrenNode[7] = new KOctree(
						KPoint(
							CenterPoint.GetX() + HalfDepth / 2.0f,
							CenterPoint.GetY() - HalfWidth / 2.0f,
							CenterPoint.GetZ() - HalfHeight / 2.0f),
						HalfWidth,
						HalfHeight,
						HalfDepth);

					int SearchCount = 0;

					for (int i = 0; i < 8; i++)
					{
						if (2 == SearchCount)
						{
							break;
						}

						if (ChildrenNode[i] != nullptr && ChildrenNode[i] - IsInSlef(point))
						{
							ChildrenNode[i]->insert(point);

							SearchCount++;

							continue;
						}

						if (ChildrenNode[i] != nullptr && ChildrenNode[i] - IsInSlef(DataPoint))
						{
							ChildrenNode[i]->insert(DataPoint);
							SearchCount++;
						}
					}

					DataPoint.IsSet = false;
			}
			else
			{
				DataPoint = point;
				DataPoint.IsSet = true;
			}
		}
	}

	void FindPointInOctree(const KPoint& InMin, const KPoint& InMax, std::vector<KPoint>& OutPoints)
	{
		if (IsHasChildren())
		{
			for (int i = 0; i < 8; i++) {
				KPoint Tmp(
					ChildrenNode[i]->Depth / 2.0f,
					ChildrenNode[i]->Width / 2.0f,
					ChildrenNode[i]->Height / 2.0f);
				KPoint ChildMin(
					ChildrenNode[i]->CenterPoint - Tmp);

				KPoint ChildMax(
					ChildrenNode[i]->CenterPoint + Tmp);

				if (IsBoxOverLap(InMin,InMax,ChildMin,ChildMax))
				{
					ChildrenNode[i]->FindPointInOctree(InMin, InMax, OutPoints);
				}
			}

		}
		else if(DataPoint.IsSet)
		{
			if (DataPoint.GetX() > InMin.GetX() &&
				DataPoint.GetY() > InMin.GetY() &&
				DataPoint.GetZ() > InMin.GetZ() &&
				DataPoint.GetX() < InMax.GetX() &&
				DataPoint.GetY() < InMax.GetY() &&
				DataPoint.GetZ() < InMax.GetZ())
			{
				OutPoints.push_back(DataPoint);
			}
		}
	}
private:
	KOctree* ChildrenNode[8];
	KPoint CenterPoint;
	float Width;
	float Height;
	float Depth;

	KPoint DataPoint;
};

float RandFloatRange(float InMin, float InMax)
{
	return InMin + (InMax - InMin) * ((rand() & RAND_MAX) / (float)RAND_MAX);
}

KPoint RnadPoint(float InMin, float InMax)
{
	return KPoint(RandFloatRange(InMin, InMax), RandFloatRange(InMin, InMax), RandFloatRange(InMin, InMax));
}

bool IsPointInBox(const KPoint& InPoint, const KPoint& InMin, const KPoint& InMax) {
	return
		InPoint.GetX() >= InMin.GetX() && InPoint.GetY() >= InMin.GetY() && InPoint.GetZ() >= InMin.GetZ() && \
		InPoint.GetX() <= InMax.GetX() && InPoint.GetY() <= InMax.GetY() && InPoint.GetZ() <= InMax.GetZ();
}


int main()
{
	std::vector<KPoint> TestPoints;
	for (int i = 0; i < 500; i++)
	{
		TestPoints.push_back(RnadPoint(0.0f,99.0f));
	}

	KPoint MinPoint(RnadPoint(10.0f, 30.0f));
	KPoint MaxPoint(RnadPoint(50.0f, 90.0f));

	KOctree* octree = new KOctree(KPoint(0.0f,0.0f,0.0f),100.0f,100.0f,100.0f);

	int InBoxCount = 0;

	for (int i = 0; i < 500; i++)
	{
		if (IsPointInBox(TestPoints[i], MinPoint, MaxPoint))
		{
			InBoxCount++;
		}

		octree->insert(TestPoints[i]);
	}

	std::vector<KPoint> RetrunPoints;

	octree->FindPointInOctree(MinPoint, MaxPoint, RetrunPoints);
	std::cout << (RetrunPoints.size() == InBoxCount) << std::endl;

}
