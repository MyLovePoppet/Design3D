#pragma once

#include "SceneCulling.h"
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"


SceneCulling::SceneCulling()
{	
}

SceneCulling_CenterTileStrategy::SceneCulling_CenterTileStrategy(CoordinateSystem coordinateSystem)
{
	this->TileCoordinateSystem = coordinateSystem;
}


//UE���״̬���������״̬
CameraState SceneCulling_CenterTileStrategy::UEToGeoCameraState(CameraState UE_CameraState)
{
	CameraState geoCameraState;
	//geoCameraState.Location = UEToGeoPosition(UE_CameraState.Location);
	geoCameraState.Location = UE_CameraState.Location;
	//geoCameraState.Rotator = UEToGeoRotator(UE_CameraState.Rotator);
	geoCameraState.Rotator = UE_CameraState.Rotator;
	geoCameraState.AspectRatio = UE_CameraState.AspectRatio;
	geoCameraState.FOV = UE_CameraState.FOV;
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
	float distance = Intersect.Z;
	float pixelDegree = distance * abs(tan(cameraState.FOV * PI / 360.0)) / 500;
	int i;
	for (i = 0; i < 20; ++i)
	{
		float degree = 180.0f / (256 * pow(2, i));
		float degreeNext = 180.0f / (256 * pow(2, i + 1));

		if (pixelDegree >= degree)
			break;

		if (pixelDegree < degree && pixelDegree > degreeNext)
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

	return TileInfo_Grid::GetTileByLevelNumAndCoord(i, Geographic2D(Intersect.X, Intersect.Y));
}

//ͨ��������ߣ������������Ľ���
//��ǰ���ü򵥼��㷽ʽ��������������������������ڵ������Ľ���
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
TSet<TileNode> SceneCulling_CenterTileStrategy::GetTileSetBySeedTile(CameraState cameraState, TileInfo_Grid seedTile, int MaxIteration)
{
	TSet<TileNode> resultArray;
	//����ǰ������ϸ��Ƭ�㼶С��3������ȫ���ڶ��㼶��Ƭ
	if (seedTile.LevelNum < 3)
	{
		int totalRow = seedTile.GetTileRowCountInGlobe();
		int totalCol = seedTile.GetTileColCountInGlobe();
		for (int i = 0; i < totalRow; i++)
		{
			for (int j = 0; j < totalCol; j++)
			{
				resultArray.Add(TileNode(seedTile.LevelNum, i, j));
			}
		}
	}//����ǰ�ϸ��Ƭ�㼶����3�����պ���4*4�����������Լ���
	else
	{
		resultArray.Add(seedTile);
		TArray<TileInfo_Grid> Neighbor_15_Tile;
		seedTile.GetNeighbor_15(Neighbor_15_Tile);
		for (int i = 0; i < Neighbor_15_Tile.Num(); i++)
		{
			resultArray.Add(TileNode(Neighbor_15_Tile[i]));
		}
		//�趨������Ļ�ֱ��ʵ�����ֹͣ�ж�����
		int breakScreenSize = FMath::Max(cameraState.screenResolution.X, cameraState.screenResolution.Y);
				
		//��ǰĬ��Ϊ������Ƭ256*256�ֱ��ʣ������ĳ�ͨ�ñ���
		int currentTotalTileSize = 4 * 256;
		int currentIterationIndex = 0;

		TArray<TileInfo_Grid> thisLevelNeighborTiles = Neighbor_15_Tile;
		TArray<TileInfo_Grid> upLevelBoundary12_Tile;

		thisLevelNeighborTiles[0].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		thisLevelNeighborTiles[3].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		thisLevelNeighborTiles[11].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		thisLevelNeighborTiles[14].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
		currentIterationIndex += 1;
		currentTotalTileSize *= 2;
		
		while (currentTotalTileSize < breakScreenSize && currentIterationIndex <= MaxIteration)
		{
			thisLevelNeighborTiles = upLevelBoundary12_Tile;
			for (int i = 0; i < thisLevelNeighborTiles.Num(); i++)
			{
				resultArray.Add(TileInfo_Grid(thisLevelNeighborTiles[i]));
			}			
			upLevelBoundary12_Tile.RemoveAll(upLevelBoundary12_Tile);

			thisLevelNeighborTiles[0].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
			thisLevelNeighborTiles[3].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
			thisLevelNeighborTiles[6].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
			thisLevelNeighborTiles[9].GetParent().GetNeighbor_3(upLevelBoundary12_Tile);
			currentIterationIndex += 1;
			currentTotalTileSize *= 2;			
		}
	}
}


//���㵱ǰӦ�ü��ص���Ƭ
TSet<TileNode> SceneCulling_CenterTileStrategy::GetTilesShouldbeLoaded(CameraState UE_CameraState, FVector2D CurrentScreenResolution)
{
	//1��ͨ���任��ue���״̬תΪ�������״̬
	this->GeoCameraState = UEToGeoCameraState(UE_CameraState);

	//2��ͨ����ǰ������ϵͳ��Ϣ�����ѿ�����������ϵ�е����λ�ã�ת��Ϊ��γ��/�߶ȣ����ں�������������Ƭ
	FVector GeoGlobePosition = this->TileCoordinateSystem.FromUE_CoordinateSystem(GeoCameraState.Location);

	//3, ���ݵ�ǰ�������򽻵�ľ��룬��Ļ�ֱ��ʣ�ȷ�������ϸ��Ƭ�ļ���Ͷ�Ӧ��������Ƭ
	FVector Intersect;
	TileInfo_Grid seedTile = GetTileByCenterIntersect(GeoCameraState, Intersect);

	//4������������Ƭ�����㵱ǰ�ӽ���Ҫ���ص�������Ƭ����
	TSet<TileNode> resultArray = GetTileSetBySeedTile(GeoCameraState, seedTile,4);
		
	return resultArray;
}

