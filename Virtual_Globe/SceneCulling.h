#pragma once
#include "CoreMinimal.h"
#include "CoordinateSystem.h"
#include "TileInfo.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include <queue>


// ���״̬
struct CameraState {
	//field of view,�����ƣ��Ƕ�
	float FOV;
	//���λ��
	FVector Location;
	//�����ת�Ƕ�
	FRotator Rotator;
	//��Ļ�ӿڵĺ��ݱ�
	float AspectRatio;
	//��Ļ�ֱ���
	FVector2D screenResolution;
};

//��������������ߵ����ݽṹ
struct HalfLine
{
	//�����λ
	FVector BasePoint;
	//������߽Ƕ�
	FRotator Direction;
};

//�����ڵ�ǰ���������ϵͳ�У������ز����Ĺ����࣬�б���UE�Դ��������
//��Ҫְ��Ϊ��ɴ�UE�����ȡ�õ���ز�����ʵ�ֻ����ӵ�λ�õĴ�������Ƭ���ݼ�����
class ISceneCulling
{
public:
	CameraState currentCameraState;
	FVector currentGeoCameraLocation;
	virtual void GetTilesShouldbeLoaded(const CameraState &inCameraState, TSet<ITileInfo*> & outTileSet) = 0;
};

class SceneCulling_CenterTileStrategy :public ISceneCulling
{
public:

	Sphere_CoordinateSystem* TileCoordinateSystem;	

	SceneCulling_CenterTileStrategy(CoordinateSystem* inCoordinateSystem);

	//virtual ~SceneCulling_CenterTileStrategy();

	//������Ƭ���ݼ����ں���Ƭ���ֹ��򣩣����λ�ã���Ļ�ֱ��ʣ�
	//���㵱ǰ�ӿ�Ӧ�ü��ز���ʾ����Ƭ���ݼ�
	//�����������Ĳ�������
	void GetTilesShouldbeLoaded(const CameraState &inCameraState, TSet<ITileInfo*> & outTileSet);	

	//���������λ����Ļ�ֱ��ʼ��㵱ǰ��Ļ�µ�ʵ����Ҫ������ϸ��Ƭ����
	float  GetDegreePerPixelInScreen(const float meterPerDegree, const float pixelPerTile);
private:

	TileInfo_Grid* rootTile;

	int FinestLevelNum = 1;

	TArray<FVector2D>  GeoRangeInScreen;

	FVector screenCenterPt;		

	void GetCameraHalfLine(CameraState cameraState, TArray<HalfLine>& lines);

	FVector GetHalfLineIntersect(HalfLine line);

	TileInfo_Grid GetTileByDistance(CameraState cameraState, FVector xoyIntersect);

	TileInfo_Grid GetTileByCenterIntersect(CameraState cameraState, FVector &intersect);

	void  GetGeoRangeInScreen(const float meterPerDegree, const float pixelPerTile, TArray<FVector2D> &outGeoRange);

	int GetTilesLevelInScreen(const float meterPerDegree, const float pixelPerTile);

	void GetTilesByBFS_Iterations(TSet<ITileInfo*> &outTileSet);	
};


class SceneCulling_CenterTileStrategyOSGB :ISceneCulling
{
public:

	Sphere_CoordinateSystem TileCoordinateSystem;

	SceneCulling_CenterTileStrategyOSGB(const FString txtFilePath);

	int GetLevelbyHeight(float height);

	//����OSG��Ƭ��������ͨ���ⲿtxt�ļ���ȡ�������λ�ã���Ļ�ֱ��ʣ�
	//���㵱ǰ�ӿ�Ӧ�ü��ز���ʾ����Ƭ���ݼ�
	//���������ڶ��������
	void GetTilesShouldbeLoaded(const CameraState &inCameraState, TSet<ITileInfo*> & outTileSet);

	//TArray<FString> GetTileByCameraState(const CameraState &inCameraState, TArray<TileInfo_OSGB> osgTree);

private:

	CameraState currentCameraState;

	TileInfo_Grid* rootTile;

	TArray<TileInfo_OSGB*> osgTree;

	
	TArray<FVector2D>  GeoRangeInScreen;

	FVector screenCenterPt;

	void readOSGTree(const FString txtFilePath, TArray<TileInfo_OSGB*> &osgTreeArray);

	//���������λ����Ļ�ֱ��ʼ��㵱ǰ��Ļ�µ�ʵ����Ҫ������ϸ��Ƭ����
	float GetDegreePerPixelInScreen(const float meterPerDegree, const float pixelPerTile);

	void  GetGeoRangeInScreen(const float meterPerDegree, const float pixelPerTile, TArray<FVector2D> &outGeoRange);
	
};