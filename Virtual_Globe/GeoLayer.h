#pragma once
#include "CoreMinimal.h"
#include "SceneCulling.h"
#include "TileLoadManager.h"
#include "TileLoadTaskExcutor.h"
class GeoLayer
{
public:
	ISceneCulling* sceneCulling;
	ITileLoadRefiningStrategy* tileLoadStrategy;
	ITileLoadManager* loadManager;
	ITileLoadTaskExcutor* tileLoadTaskExcutor;

	//��ǰͼ������Դ·��������·��������·����Ŀ¼
	FString dataSourcePath;
	//�Ƿ���ʾ��ͼ���ʶ
	bool showLayer;

	AActor* rootActor;

	//ʵ�ʵ���Ƭ�����߼�
	void OnTick();
	
	void OnCameraStateChange(const CameraState &newCameraState, const CameraState &lastCameraState);	

};

class GridTileLayer :GeoLayer
{
public:
	GridTileLayer(const FString layerSourcePath, AActor* rootActor);
};

