#pragma once
#include "CoreMinimal.h"
#include "CoordinateSystem.h"
#include "TileInfo.h"


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

struct HalfLine
{
	FVector BasePoint;
	FRotator Direction;
};

//�����ڵ�ǰ���������ϵͳ�У������ز����Ĺ����࣬�б���UE�Դ��������
//��Ҫְ��Ϊ��ɴ�UE�����ȡ�õ���ز�����ʵ�ֻ����ӵ�λ�õĴ�������Ƭ���ݼ�����
class SceneCulling
{
public:

	//���������״̬
	CameraState GeoCameraState;

	//��ǰ��Ƭ������ϵͳ
	//CoordinateSystem TileCoordinateSystem;

	//���캯��
	SceneCulling();		
};

class SceneCulling_CenterTileStrategy :SceneCulling
{
public:

	Sphere_CoordinateSystem TileCoordinateSystem;

	SceneCulling_CenterTileStrategy(Sphere_CoordinateSystem coordinateSystem);
	//������Ƭ���ݼ����ں���Ƭ���ֹ��򣩣����λ�ã���Ļ�ֱ��ʣ�
	//���㵱ǰ�ӿ�Ӧ�ü��ز���ʾ����Ƭ���ݼ�
	//�����������Ĳ�������
	TSet<TileNode*> GetTilesShouldbeLoaded(CameraState UE_CameraState, FVector2D CurrentScreenResolution);

private:

	//��ue���״̬��ת��Ϊ��ǰ�����������״̬
	CameraState UEToGeoCameraState(CameraState UE_CameraState);

	void GetCameraHalfLine(CameraState cameraState, TArray<HalfLine>& lines);

	FVector GetHalfLineIntersect(HalfLine line);

	TileInfo_Grid GetTileByDistance(CameraState cameraState, FVector xoyIntersect);

	TileInfo_Grid GetTileByCenterIntersect(CameraState cameraState, FVector &intersect);

	TSet<TileNode*> GetTileSetBySeedTile(CameraState cameraState, TileInfo_Grid seedTile, int MaxIteration);
};




