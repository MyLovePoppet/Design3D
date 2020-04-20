#pragma once

#include "SceneCulling.h"
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"


SceneCulling_CenterTileStrategy::SceneCulling_CenterTileStrategy(CoordinateSystem* inCoordinateSystem)
{
	this->TileCoordinateSystem = (Sphere_CoordinateSystem*)inCoordinateSystem;
}



//���������λ����Ļ�ֱ��ʼ��㵱ǰ��Ļ�µ�ʵ����Ҫ������ϸ��Ƭ����
float  SceneCulling_CenterTileStrategy::GetDegreePerPixelInScreen(const float meterPerDegree,const float pixelPerTile)
{
	//ʹ�������и����Ĳ�����Ϊ�ο������������޸ģ�������һ�Ⱦ��ȶ�Ӧ�ڶ����ף���ͬγ�ȣ�ֵ��һ�£��˴�ȡ���ڸ�������ֵ
	//ʵ������ʱ���ò���Ϊγ�ȵĺ���
	//float meterPerDegree = 111318.0;
	//tile��pixel����
	//float pixelPerTile = 256;
	
	//Ŀǰ���������λ����Ϊ�����������潻�㣬�������潻��ľ���������߶ȴ��棬�������������ķ�λ����������
	//FVector geoCameraLocation = TileCoordinateSystem->ToGeoCoordinateSystem(this->currentCameraState.Location);
	float distance = this->currentGeoCameraLocation.Z;


	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ף�
	float groundLengthInMeter = distance * FMath::Abs(FMath::Tan(this->currentCameraState.FOV * PI / 360.0)) * 2;
	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ȣ�
	float groundLengthInDegree = groundLengthInMeter / meterPerDegree;

	//������Ļ�ֱ��ʣ�ȷ����Ļ�������ض�Ӧ�ڵ�����루��λ���ȣ�
	float degreePerPixelInScreen = groundLengthInDegree / (FMath::Max(this->currentCameraState.screenResolution.X, this->currentCameraState.screenResolution.Y));

	return degreePerPixelInScreen;
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
	return line.BasePoint;
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

//�ж�����Ϊ��ǰ��Ļ��ʵ�ʾ�γ�������Ƿ��뵱ǰ��Ƭ�ཻ������ཻ�����ȡ������Ƭ����������
void SceneCulling_CenterTileStrategy::GetTilesByBFS_Iterations(TSet<ITileInfo*> & outTiles)
{
	//FVector screenCenterPtInDegree = FVector(FMath::RadiansToDegrees(this->screenCenterPt.X), FMath::RadiansToDegrees(this->screenCenterPt.Y), screenCenterPt.Z);
	
	if (this->GeoRangeInScreen.Num() != 4)
		return;

	//����0��������Ƭ����Ϊ��ʼ������Ƭ
	TArray<TileInfo_Grid*> rootTiles;
	rootTiles.Add(new TileInfo_Grid(0, 0, 0));
	rootTiles.Add(new TileInfo_Grid(0, 0, 1));

	std::queue<TileInfo_Grid * > BFS_Iteration_Queue;
	if (rootTiles.Num() > 0)
	{		
		for (TileInfo_Grid* level0Tile : rootTiles)
		{
			//0��������Ƭ�����أ��ʼ����1����Ƭ			
			TArray<TileInfo_Grid*> level0Children;
			level0Tile->GetChildren(level0Children);

			for (TileInfo_Grid* level1Tile : level0Children)
			{
				BFS_Iteration_Queue.push(level1Tile);
			}
		}

		//��������������
		while (!BFS_Iteration_Queue.empty())
		{
			//��ȡ��������λ��Ƭ
			TileInfo_Grid * currentTile = BFS_Iteration_Queue.front();		
			BFS_Iteration_Queue.pop();

			//������ƬΪ�գ������Ƭ�㼶���ڵ�ǰӦ�ü��ص��ϸһ����Ƭ�㼶��������
			if (currentTile == NULL)
				continue;
			if (currentTile->LevelNum > this->FinestLevelNum)
				continue;

			//������Ƭ�ڵ�ǰ��Ļ�в��ɼ������ظ���Ƭ��������������Ƭ
			if (!currentTile->IsGeoRangeIntersect(this->GeoRangeInScreen))
			{
				outTiles.Add(currentTile);
			}
			//������Ƭ�ڵ�ǰ��Ļ��Χ�пɼ�������ظ���Ƭ��һ����Ƭ�������ظ���Ƭ
			else
			{
				//����ǰ��Ƭ��ΪӦ�ü��ص��ϸһ����Ƭ,��ֻ���ظ���Ƭ�������ظ���Ƭ����Ƭ
				if (currentTile->LevelNum == this->FinestLevelNum)
				{
					outTiles.Add(currentTile);
					continue;
				}

				//��ȡ����Ƭ����Ƭ���������������
				TArray<TileInfo_Grid*> currentTileChildren;
				currentTile->GetChildren(currentTileChildren);
				
				for (TileInfo_Grid* currentTile_Child : currentTileChildren)
				{
					BFS_Iteration_Queue.push(currentTile_Child);
				}
			}
		}
	}	
}

//���������λ����Ļ�ֱ��ʼ��㵱ǰ��Ļ�µ�ʵ����Ҫ������ϸ��Ƭ����
int  SceneCulling_CenterTileStrategy::GetTilesLevelInScreen(const float meterPerDegree, const float pixelPerTile)
{
	//Ŀǰ���������λ����Ϊ�����������潻�㣬�������潻��ľ���������߶ȴ��棬�������������ķ�λ����������
	float distance = this->currentGeoCameraLocation.Z;

	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ף�
	float groundLengthInMeter = distance * FMath::Abs(FMath::Tan(this->currentCameraState.FOV * PI / 360.0)) * 2;
	//��Ļ���/���ȶ�Ӧ�ĵ�����루��λ���ȣ�
	float groundLengthInDegree = groundLengthInMeter / meterPerDegree;

	//������Ļ�ֱ��ʣ�ȷ����Ļ�������ض�Ӧ�ڵ�����루��λ���ȣ�
	float degreePerPixelInScreen = groundLengthInDegree / (FMath::Max(this->currentCameraState.screenResolution.X, this->currentCameraState.screenResolution.Y));

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
void SceneCulling_CenterTileStrategy::GetGeoRangeInScreen(const float meterPerDegree, const float pixelPerTile, TArray<FVector2D> &outGeoRange)
{
	//�ϸ����Ƭռ������Ļ��Χ����
	float finestRatio = 1.0;

	FVector screenCenterPtInDegree = FVector(FMath::RadiansToDegrees(this->screenCenterPt.X), FMath::RadiansToDegrees(this->screenCenterPt.Y), screenCenterPt.Z);

	float pixelLengthInDegree = GetDegreePerPixelInScreen(meterPerDegree, pixelPerTile);

	if (pixelLengthInDegree == 0 || this->currentCameraState.screenResolution.X == 0 || this->currentCameraState.screenResolution.Y == 0)
		return;

	FVector2D minPt = FVector2D(screenCenterPtInDegree.X - (this->currentCameraState.screenResolution.X * pixelLengthInDegree) * finestRatio*0.5
		, screenCenterPtInDegree.Y - (this->currentCameraState.screenResolution.Y * pixelLengthInDegree) * finestRatio*0.5);
	//������Ⱦ��ߡ��ϱ����������
	if (minPt.X < -180.0)
		minPt.X += 360.0;
	if (minPt.Y < -90.0)
		minPt.Y = (minPt.Y + 180.0)*-1.0;
	outGeoRange.Add(minPt);


	FVector2D maxPt = FVector2D(screenCenterPtInDegree.X + (this->currentCameraState.screenResolution.X * pixelLengthInDegree) * finestRatio*0.5
		, screenCenterPtInDegree.Y + (this->currentCameraState.screenResolution.Y * pixelLengthInDegree) * finestRatio*0.5);
	//������Ⱦ��ߡ��ϱ����������
	if (maxPt.X > 180.0)
		maxPt.X -= 360.0;
	if (maxPt.Y > 90.0)
		maxPt.Y = 180.0 - maxPt.Y;

	outGeoRange.Add(FVector2D(maxPt.X, minPt.Y));
	outGeoRange.Add(maxPt);
	outGeoRange.Add(FVector2D(minPt.X, maxPt.Y));
}



//���㵱ǰӦ�ü��ص���Ƭ
void SceneCulling_CenterTileStrategy::GetTilesShouldbeLoaded(const CameraState &inCameraState, TSet<ITileInfo*> & outTileSet)
{
	//1��ͨ���任��ue���״̬תΪ�������״̬
	this->currentCameraState = inCameraState;
	this->currentGeoCameraLocation = this->TileCoordinateSystem->ToGeoCoordinateSystem(inCameraState.Location);
	//FString Message = "X = " + FString::SanitizeFloat(FMath::RadiansToDegrees(this->GeoCameraState.Location.X)) + "; Y = " + FString::SanitizeFloat(FMath::RadiansToDegrees(this->GeoCameraState.Location.Y)) + "; Z = " + FString::SanitizeFloat(this->GeoCameraState.Location.Z);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);

	//2.����UE���λ�ã��ǶȲ�����������������������潻��ľ�γ������
	HalfLine CameraShootToEarthSurface;
	CameraShootToEarthSurface.BasePoint = this->currentGeoCameraLocation;
	CameraShootToEarthSurface.Direction = currentCameraState.Rotator;

	//ʹ�������и����Ĳ�����Ϊ�ο������������޸ģ�������һ�Ⱦ��ȶ�Ӧ�ڶ����ף���ͬγ�ȣ�ֵ��һ�£��˴�ȡ���ڸ�������ֵ
	//ʵ������ʱ���ò���Ϊγ�ȵĺ���
	float meterPerDegree = 111318.0;
	//tile��pixel����
	float pixelPerTile = 256;

	this->screenCenterPt = GetHalfLineIntersect(CameraShootToEarthSurface);

	//3.���������λ��FOV�����㵱ǰ֡��Ļ����Ҫ���ص��ϸ��Ƭ�Ĳ㼶
	this->FinestLevelNum = GetTilesLevelInScreen(meterPerDegree, pixelPerTile);

	//4.��ȡ��ǰ֡����Ļ����Χ
	TArray<FVector2D> currentGeoRange;
	GetGeoRangeInScreen(meterPerDegree, pixelPerTile, currentGeoRange);
	if (currentGeoRange.Num() == 4)
		this->GeoRangeInScreen = currentGeoRange;

	GetTilesByBFS_Iterations(outTileSet);
}

////�ж�����Ϊ��Ļ���ĵ��Ƿ����ڵ�ǰ��Ƭ�У�������ڵ�ǰ��Ƭ�У������������Ƭ
//TSet<TileNode*> SceneCulling_CenterTileStrategy::GetTilesByBFS_Iterations()
//{
//	FVector screenCenterPtInDegree = FVector(FMath::RadiansToDegrees(this->screenCenterPt.X), FMath::RadiansToDegrees(this->screenCenterPt.Y), screenCenterPt.Z);
//
//	TSet<TileNode*> resultTileSet;
//	std::queue<TileNode *> BFS_Iteration_Queue;
//	if (this->rootNode != NULL)
//	{
//		rootNode->CreateSubTileNode();
//		for (TileNode* child : rootNode->children)
//		{
//			//0��������Ƭ�����أ��ʼ����1����Ƭ
//			//BFS_Iteration_Queue.push(child);
//			child->CreateSubTileNode();
//			for (TileNode* childChild : child->children)
//			{
//				BFS_Iteration_Queue.push(childChild);
//			}
//		}
//
//		while (!BFS_Iteration_Queue.empty())
//		{
//			TileNode *tileNode = BFS_Iteration_Queue.front();
//			TileInfo_Grid * thisTileInfo_Grid = (TileInfo_Grid *)(tileNode->tileInfo);
//
//			BFS_Iteration_Queue.pop();
//			if (tileNode == NULL || thisTileInfo_Grid == NULL)
//				continue;
//
//			if (thisTileInfo_Grid->LevelNum > this->FinestLevelNum)
//				continue;
//
//			if (!thisTileInfo_Grid->IsPositionIn(screenCenterPtInDegree))
//			{
//				resultTileSet.Add(tileNode);
//			}
//			else
//			{
//				//����ǰ���ز㼶����Ҫ���ص��ϸ�㼶��ͬ������Ļ���ĵ��ڸ���Ƭ�ڣ�ֻ���ظ���Ƭ�������ظ���Ƭ����Ƭ
//				if (thisTileInfo_Grid->LevelNum == this->FinestLevelNum)
//				{
//					resultTileSet.Add(tileNode);
//					continue;
//				}
//				tileNode->CreateSubTileNode();
//				for (TileNode* tileNode_Child : tileNode->children)
//				{
//					BFS_Iteration_Queue.push(tileNode_Child);
//				}
//			}
//		}
//	}
//	return resultTileSet;
//}

