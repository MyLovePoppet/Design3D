#pragma once
#include "CoreMinimal.h"
#include "SceneCulling.h"
#include "TileLoadManager.h"
#include "TileLoadTaskExcutor.h"
#include "ViewerCameraController.h"
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

public:
	AViewerCameraController* pViewerCameraController;
};

class GridTileLayer : public GeoLayer
{
public:
	//virtual void OnTick() override;
	GridTileLayer(const FString layerSourcePath, AActor* rootActor, AViewerCameraController* inViewerCameraController, CoordinateSystem* inCoordinateSystem);
};

