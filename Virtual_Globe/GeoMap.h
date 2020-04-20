#pragma once
#include "CoreMinimal.h"
#include "GeoLayer.h"
#include "ViewerCameraController.h"
//#include "UE_World_Camera.h"
class GeoMap
{
private:
	TArray<GeoLayer *> geoLayerArray;
	//AUE_World_Camera * thisUE_World_Camera;
	CameraState lastCameraState;

	AActor* rootActor;

	CoordinateSystem* mapCoordinateSystem;

	bool IsCameraStateChange(const CameraState &lastCameraState, const CameraState &newCameraState);

	AViewerCameraController* pViewerCameraController;

public:
	//���ǰ��֡����Ĳ���
	//����仯�ˣ�֪ͨ����layer�����µ����λ��֪ͨ������layer
	void OnTick(CameraState newCameraState);
	GeoMap(CoordinateSystem* pCoordinateSystem, CameraState initCameraState, AActor* pActor, AViewerCameraController* inViewerCameraController);

	void AddLayer(GeoLayer* addingLayer);
	
};

