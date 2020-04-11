#pragma once

#include "SceneCulling.h"
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"


SceneCulling::SceneCulling()
{
}

SceneCulling_CenterTileStrategy::SceneCulling_CenterTileStrategy()
{}

SceneCulling_CenterTileStrategy::SceneCulling_CenterTileStrategy(Sphere_CoordinateSystem coordinateSystem, CameraState UE_CameraState)
{
	this->TileCoordinateSystem = coordinateSystem;

	this->rootNode = new TileNode(-1, 0, 0, NULL);

	this->GeoCameraState = UEToGeoCameraState(UE_CameraState);
}




//UE���״̬���������״̬
CameraState SceneCulling_CenterTileStrategy::UEToGeoCameraState(CameraState UE_CameraState)
{
	CameraState geoCameraState;
	//geoCameraState.Location = UEToGeoPosition(UE_CameraState.Location);
	geoCameraState.Location = this->TileCoordinateSystem.FromUE_CoordinateSystem(UE_CameraState.Location);
	//geoCameraState.Rotator = UEToGeoRotator(UE_CameraState.Rotator);
	geoCameraState.Rotator = UE_CameraState.Rotator;
	geoCameraState.AspectRatio = UE_CameraState.AspectRatio;
	geoCameraState.FOV = UE_CameraState.FOV;
	geoCameraState.screenResolution = UE_CameraState.screenResolution;
	return geoCameraState;
}

//�������״̬����ȡ�䳯�������
void SceneCulling_CenterTileStrategy::GetCameraHalfLine(CameraState cameraState, TArray<HalfLine>& lines)
{
	HalfLine centerHalfLine;
	centerHalfLine.BasePoint = cameraState.Location;
	centerHalfLine.Direction = cameraState.Rotator;

	lines.Add(centerHalfLine);
}

//�������λ�á���λ�����㵱ǰ��������ӵ����������Ľ���
//xoyIntersect �� lon��x�� lat��y,height��z
TileInfo_Grid SceneCulling_CenterTileStrategy::GetTileByDistance(CameraState cameraState, FVector Intersect)
{
	//ʹ�������и����Ĳ�����Ϊ�ο������������޸ģ�������һ�Ⱦ��ȶ�Ӧ�ڶ����ף���ͬγ�ȣ�ֵ��һ�£��˴�ȡ���ڸ�������ֵ
	//ʵ������ʱ���ò���Ϊγ�ȵĺ���
	float meterPerDegree = 111318.0;
	float pixelPerTile = 256;

	float distance = Intersect.Z;
	float groundLengthInMeter = distance * FMath::Abs(FMath::Tan(cameraState.FOV * PI / 360.0));
	float degreePerPixel = groundLengthInMeter / (meterPerDegree* pixelPerTile);

	int level_i;
	for (level_i = 0; level_i < 20; ++level_i)
	{
		float degreePerPixelInLevle_i = 180.0f / (pixelPerTile * FMath::Pow(2, level_i));
		float degreePerPixelInNextLevle = 180.0f / (pixelPerTile * FMath::Pow(2, level_i + 1));

		if (degreePerPixel >= degreePerPixelInLevle_i)
			break;

		if (degreePerPixel < degreePerPixelInLevle_i && degreePerPixel > degreePerPixelInNextLevle)
			break;
	}

	//����γ�Ȼ���ת�Ƕ�
	FVector2D IntersectInDegree = FVector2D((Intersect.X / PI)*180.0, (Intersect.Y / PI)*180.0);

	if (IntersectInDegree.Y < -90 || IntersectInDegree.Y > 90 || IntersectInDegree.X < -180 || IntersectInDegree.X > 180)
	{
		TileInfo_Grid tile;
		tile.LevelNum = 1;
		tile.Row = 1;

		tile.Col = 2;
		return tile;
	}

	return TileInfo_Grid::GetTileByLevelNumAndCoord(level_i, Geographic2D(Intersect.X, Intersect.Y));
}

//ͨ��������ߣ������������Ľ���
//��ǰ���ü򵥼��㷽ʽ��������������������������ڵ������Ľ���,��γ�ȣ�������
FVector SceneCulling_CenterTileStrategy::GetHalfLineIntersect(HalfLine line)
{
	//FQuat quat = line.Direction.Quaternion();
	//quat.Normalize();
	//FVector endPoint = quat.Vector() + line.BasePoint;

	//FVector intersect;
	////if (endPoint.Z >= line.BasePoint.Z)
	////{
	////	intersect.X = INFINITY; 
	////	intersect.Y = INFINITY;		 
	////}
	////else
	//{
	//	intersect.Z = 0.0f;
	//	intersect.X = (endPoint.X - line.BasePoint.X) * (intersect.Z - line.BasePoint.Z) / (endPoint.Z - line.BasePoint.Z) + line.BasePoint.X;
	//	intersect.Y = (endPoint.Y - line.BasePoint.Y) * (intersect.Z - line.BasePoint.Z) / (endPoint.Z - line.BasePoint.Z) + line.BasePoint.Y;
	//}

	//return FVector2D(intersect);

	//����ֵ��lon��x��lat��y	
	FVector intersect = this->TileCoordinateSystem.FromUE_CoordinateSystem(line.BasePoint);
	return intersect;
}

//ͨ�����������������򽻵㣬�������Ľ���Ӧ�ü��ص��ϸ��Ƭ
TileInfo_Grid SceneCulling_CenterTileStrategy::GetTileByCenterIntersect(CameraState GeoCameraState, FVector &IntersectPt)
{
	TArray<HalfLine> lines;

	GetCameraHalfLine(GeoCameraState, lines);
	HalfLine centerLine = lines[0];

	IntersectPt = GetHalfLineIntersect(centerLine);

	TileInfo_Grid tile = GetTileByDistance(GeoCameraState, IntersectPt);
	return tile;
}

//����������Ƭ������Ӧ�ü��ص�����������Ƭ�����뼯��
TSet<TileNode*> SceneCulling_CenterTileStrategy::GetTileSetBySeedTile(CameraState cameraState, TileInfo_Grid seedTile, int MaxIteration)
{
	TSet<TileNode*> resultArray;
	//����ǰ������ϸ��Ƭ�㼶С��3������ȫ����ǰ�㼶��Ƭ
	if (seedTile.LevelNum < 3)
	{
		int totalRow = seedTile.GetTileRowCountInGlobe();
		int totalCol = seedTile.GetTileColCountInGlobe();
		for (int i = 0; i < totalRow; i++)
		{
			for (int j = 0; j < totalCol; j++)
			{
				TileNode currentTileNode = TileNode(seedTile.LevelNum, i, j);
				resultArray.Add(&currentTileNode);
			}
		}
	}//����ǰ�ϸ��Ƭ�㼶����3�����պ���4*4�����������ԣ��ݹ��㷨����
	else
	{
		//1.����������Ƭ������15������Ƭ,����������
		TileNode seedTileNode = TileNode(seedTile);
		resultArray.Add(&seedTileNode);

		TArray<TileInfo_Grid*> neighbor_15_tiles;
		seedTile.GetNeighbor_15(neighbor_15_tiles);

		for (TileInfo_Grid* val : neighbor_15_tiles)
			resultArray.Add(new TileNode(val));

		//�趨������Ļ�ֱ��ʵ�����ֹͣ�ж�����
		int breakScreenSize = FMath::Max(cameraState.screenResolution.X, cameraState.screenResolution.Y);
		//��ǰĬ��Ϊ������Ƭ256*256�ֱ��ʣ������ĳ�ͨ�ñ���
		int currentTotalTileSize = 4 * 256;
		int currentIterationIndex = 0;






		//currentIterationIndex += 1;
		//currentTotalTileSize *= 2;

		//while (currentTotalTileSize < breakScreenSize && currentIterationIndex <= MaxIteration)
		//{
		//	thisLevelNeighborTiles = upLevelBoundary12_Tile;
		//	for (int i = 0; i < thisLevelNeighborTiles.Num(); i++)
		//	{				
		//		TileNode currentTileNode = TileNode(thisLevelNeighborTiles[i]);
		//		resultArray.Add(&currentTileNode);
		//	}			
		//	upLevelBoundary12_Tile.RemoveAt(0, upLevelBoundary12_Tile.Num());

		//	thisLevelNeighborTiles[0].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		//	thisLevelNeighborTiles[3].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		//	thisLevelNeighborTiles[6].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		//	thisLevelNeighborTiles[9].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		//	currentIterationIndex += 1;
		//	currentTotalTileSize *= 2;			
		//}
	}
	return resultArray;
}

//�ж�����Ϊ��ǰ��Ļ��ʵ�ʾ�γ�������Ƿ��뵱ǰ��Ƭ�ཻ������ཻ�����ȡ������Ƭ����������
TSet<TileNode*> SceneCulling_CenterTileStrategy::GetTilesByBFS_Iterations2()
{
	//FVector screenCenterPtInDegree = FVector(FMath::RadiansToDegrees(this->screenCenterPt.X), FMath::RadiansToDegrees(this->screenCenterPt.Y), screenCenterPt.Z);
	this->GeoRangeInScreen = GetGeoRangeInScreen();
	
		TSet<TileNode*> resultTileSet;
	std::queue<TileNode *> BFS_Iteration_Queue;
	if (this->rootNode != NULL)
	{
		rootNode->CreateSubTileNode();
		for (TileNode* child : rootNode->children)
		{
			//0��������Ƭ�����أ��ʼ����1����Ƭ
			//BFS_Iteration_Queue.push(child);
			child->CreateSubTileNode();
			for (TileNode* childChild : child->children)
			{
				BFS_Iteration_Queue.push(childChild);
			}
		}

		while (!BFS_Iteration_Queue.empty())
		{
			TileNode *tileNode = BFS_Iteration_Queue.front();
			TileInfo_Grid * thisTileInfo_Grid = (TileInfo_Grid *)(tileNode->tileInfo);

			BFS_Iteration_Queue.pop();
			if (tileNode == NULL || thisTileInfo_Grid == NULL)
				continue;

			if (thisTileInfo_Grid->LevelNum > this->FinestLevelNum)
				continue;

			if (!thisTileInfo_Grid->IsGeoRangeIntersect(this->GeoRangeInScreen))
			{
				resultTileSet.Add(tileNode);
			}
			else
			{
				//����ǰ���ز㼶����Ҫ���ص��ϸ�㼶��ͬ������Ļ���ĵ��ڸ���Ƭ�ڣ�ֻ���ظ���Ƭ�������ظ���Ƭ����Ƭ
				if (thisTileInfo_Grid->LevelNum == this->FinestLevelNum)
				{
					resultTileSet.Add(tileNode);
					continue;
				}
				tileNode->CreateSubTileNode();
				for (TileNode* tileNode_Child : tileNode->children)
				{
					BFS_Iteration_Queue.push(tileNode_Child);
				}
			}
		}
	}
	return resultTileSet;
}

//�ж�����Ϊ��Ļ���ĵ��Ƿ����ڵ�ǰ��Ƭ�У�������ڵ�ǰ��Ƭ�У������������Ƭ
TSet<TileNode*> SceneCulling_CenterTileStrategy::GetTilesByBFS_Iterations()
{
	FVector screenCenterPtInDegree = FVector(FMath::RadiansToDegrees(this->screenCenterPt.X), FMath::RadiansToDegrees(this->screenCenterPt.Y), screenCenterPt.Z);

	TSet<TileNode*> resultTileSet;
	std::queue<TileNode *> BFS_Iteration_Queue;
	if (this->rootNode != NULL)
	{
		rootNode->CreateSubTileNode();
		for (TileNode* child : rootNode->children)
		{
			//0��������Ƭ�����أ��ʼ����1����Ƭ
			//BFS_Iteration_Queue.push(child);
			child->CreateSubTileNode();
			for (TileNode* childChild : child->children)
			{
				BFS_Iteration_Queue.push(childChild);
			}
		}

		while (!BFS_Iteration_Queue.empty())
		{
			TileNode *tileNode = BFS_Iteration_Queue.front();
			TileInfo_Grid * thisTileInfo_Grid = (TileInfo_Grid *)(tileNode->tileInfo);

			BFS_Iteration_Queue.pop();
			if (tileNode == NULL || thisTileInfo_Grid == NULL)
				continue;

			if (thisTileInfo_Grid->LevelNum > this->FinestLevelNum)
				continue;

			if (!thisTileInfo_Grid->IsPositionIn(screenCenterPtInDegree))
			{
				resultTileSet.Add(tileNode);
			}
			else
			{
				//����ǰ���ز㼶����Ҫ���ص��ϸ�㼶��ͬ������Ļ���ĵ��ڸ���Ƭ�ڣ�ֻ���ظ���Ƭ�������ظ���Ƭ����Ƭ
				if (thisTileInfo_Grid->LevelNum == this->FinestLevelNum)
				{
					resultTileSet.Add(tileNode);
					continue;
				}
				tileNode->CreateSubTileNode();
				for (TileNode* tileNode_Child : tileNode->children)
				{
					BFS_Iteration_Queue.push(tileNode_Child);
				}
			}
		}
	}
	return resultTileSet;
}

//���������λ����Ļ�ֱ��ʼ��㵱ǰ��Ļ�µ�ʵ����Ҫ������ϸ��Ƭ����
int  SceneCulling_CenterTileStrategy::GetTilesLevelInScreen()
{
	//ʹ�������и����Ĳ�����Ϊ�ο������������޸ģ�������һ�Ⱦ��ȶ�Ӧ�ڶ����ף���ͬγ�ȣ�ֵ��һ�£��˴�ȡ���ڸ�������ֵ
	//ʵ������ʱ���ò���Ϊγ�ȵĺ���
	float meterPerDegree = 111318.0;
	//tile��pixel����
	float pixelPerTile = 256;

	//Ŀǰ���������λ����Ϊ�����������潻�㣬�������潻��ľ���������߶ȴ��棬�������������ķ�λ����������
	float distance = this->GeoCameraState.Location.Z;

	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ף�
	float groundLengthInMeter = distance * FMath::Abs(FMath::Tan(this->GeoCameraState.FOV * PI / 360.0)) * 2;
	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ȣ�
	float groundLengthInDegree = groundLengthInMeter / meterPerDegree;

	//������Ļ�ֱ��ʣ�ȷ����Ļ�������ض�Ӧ�ڵ�����루��λ���ȣ�
	float degreePerPixelInScreen = groundLengthInDegree / (FMath::Max(this->GeoCameraState.screenResolution.X, this->GeoCameraState.screenResolution.Y));

	int level_i;
	for (level_i = 0; level_i < 20; ++level_i)
	{
		float degreePerPixelInLevle_i = 180.0f / (pixelPerTile * FMath::Pow(2, level_i));
		float degreePerPixelInNextLevle = 180.0f / (pixelPerTile * FMath::Pow(2, level_i + 1));

		if (degreePerPixelInScreen >= degreePerPixelInLevle_i)
			break;
		if (degreePerPixelInScreen < degreePerPixelInLevle_i && degreePerPixelInScreen > degreePerPixelInNextLevle)
			break;
	}

	return level_i;
}

//���������λ����Ļ�ֱ��ʼ��㵱ǰ��Ļ�µ�ʵ����Ҫ������ϸ��Ƭ����,���ڴ˼��㵱ǰ��Ļʵ�ʵľ�γ�ȷ�Χ
//���ؽ��Ϊ�ĸ���pt0(minX,minY) pt1(maxX,minY) pt2(maxX,maxY) pt3(minX,maxY)
TArray<FVector2D>  SceneCulling_CenterTileStrategy::GetGeoRangeInScreen()
{
	TArray<FVector2D> result;
	FVector screenCenterPtInDegree = FVector(FMath::RadiansToDegrees(this->screenCenterPt.X), FMath::RadiansToDegrees(this->screenCenterPt.Y), screenCenterPt.Z);

	float pixelLengthInDegree = GetDegreePerPixelInScreen();

	if (pixelLengthInDegree == 0||this->GeoCameraState.screenResolution.X ==0|| this->GeoCameraState.screenResolution.Y == 0)
		return result;

	FVector2D minPt = FVector2D(screenCenterPtInDegree.X - (this->GeoCameraState.screenResolution.X * pixelLengthInDegree) / 2
		, screenCenterPtInDegree.Y - (this->GeoCameraState.screenResolution.Y * pixelLengthInDegree) / 2);
	if (minPt.X < -180.0)
		minPt.X += 360.0;
	if (minPt.Y < -90.0)
		minPt.Y = (minPt.Y + 180.0)*-1.0;
	result.Add(minPt);


	FVector2D maxPt = FVector2D(screenCenterPtInDegree.X + (this->GeoCameraState.screenResolution.X * pixelLengthInDegree) / 2
		, screenCenterPtInDegree.Y + (this->GeoCameraState.screenResolution.Y * pixelLengthInDegree) / 2);
	if (maxPt.X > 180.0)
		maxPt.X -= 360.0;
	if (maxPt.Y > 90.0)
		maxPt.Y = 180.0 - maxPt.Y;	

	result.Add(FVector2D(maxPt.X,minPt.Y));
	result.Add(maxPt);
	result.Add(FVector2D(minPt.X, maxPt.Y));

	return result;
}

//���������λ����Ļ�ֱ��ʼ��㵱ǰ��Ļ�µ�ʵ����Ҫ������ϸ��Ƭ����
float  SceneCulling_CenterTileStrategy::GetDegreePerPixelInScreen()
{
	//ʹ�������и����Ĳ�����Ϊ�ο������������޸ģ�������һ�Ⱦ��ȶ�Ӧ�ڶ����ף���ͬγ�ȣ�ֵ��һ�£��˴�ȡ���ڸ�������ֵ
	//ʵ������ʱ���ò���Ϊγ�ȵĺ���
	float meterPerDegree = 111318.0;
	//tile��pixel����
	float pixelPerTile = 256;

	//Ŀǰ���������λ����Ϊ�����������潻�㣬�������潻��ľ���������߶ȴ��棬�������������ķ�λ����������
	float distance = this->GeoCameraState.Location.Z;

	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ף�
	float groundLengthInMeter = distance * FMath::Abs(FMath::Tan(this->GeoCameraState.FOV * PI / 360.0)) * 2;
	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ȣ�
	float groundLengthInDegree = groundLengthInMeter / meterPerDegree;

	//������Ļ�ֱ��ʣ�ȷ����Ļ�������ض�Ӧ�ڵ�����루��λ���ȣ�
	float degreePerPixelInScreen = groundLengthInDegree / (FMath::Max(this->GeoCameraState.screenResolution.X, this->GeoCameraState.screenResolution.Y));

	return degreePerPixelInScreen;
}

//���㵱ǰӦ�ü��ص���Ƭ
TSet<TileNode*> SceneCulling_CenterTileStrategy::GetTilesShouldbeLoaded(CameraState UE_CameraState, FVector2D CurrentScreenResolution)
{
	//1��ͨ���任��ue���״̬תΪ�������״̬
	this->GeoCameraState = UEToGeoCameraState(UE_CameraState);
	//FString Message = "X = " + FString::SanitizeFloat(FMath::RadiansToDegrees(this->GeoCameraState.Location.X)) + "; Y = " + FString::SanitizeFloat(FMath::RadiansToDegrees(this->GeoCameraState.Location.Y)) + "; Z = " + FString::SanitizeFloat(this->GeoCameraState.Location.Z);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);

	//2.����UE���λ�ã��ǶȲ�����������������������潻��ľ�γ������
	HalfLine CameraShootToEarthSurface;
	CameraShootToEarthSurface.BasePoint = UE_CameraState.Location;
	CameraShootToEarthSurface.Direction = UE_CameraState.Rotator;

	this->screenCenterPt = GetHalfLineIntersect(CameraShootToEarthSurface);

	//3.���������λ��FOV�����㵱ǰ֡��Ļ����Ҫ���ص��ϸ��Ƭ�Ĳ㼶
	this->FinestLevelNum = GetTilesLevelInScreen();

	//4.��ȡ��ǰ֡����Ļ����Χ
	this->GeoRangeInScreen = GetGeoRangeInScreen();

	TSet<TileNode*> resultArray = GetTilesByBFS_Iterations2();

	return resultArray;
}

void SceneCulling_CenterTileStrategy::UpdateGeoCameraLocation(FVector &inLocation)
{
	this->GeoCameraState.Location = inLocation;
}