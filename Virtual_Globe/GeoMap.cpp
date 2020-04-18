#include "GeoMap.h"

GeoMap::GeoMap(CoordinateSystem* pCoordinateSystem, CameraState initCameraState, AActor* pActor)
{
	this->lastCameraState = initCameraState;
	this->mapCoordinateSystem = pCoordinateSystem;
	this->rootActor = pActor;
}

void GeoMap::AddLayer(GeoLayer* addingLayer)
{
	if (addingLayer )
	{
		this->geoLayerArray.Add(addingLayer);		
	}
}

bool GeoMap::IsCameraStateChange(const CameraState &lastCameraState, const CameraState &newCameraState)
{
	if (lastCameraState.AspectRatio != newCameraState.AspectRatio)
		return true;
	if (lastCameraState.FOV != newCameraState.FOV)
		return true;
	if (!lastCameraState.screenResolution.Equals(newCameraState.screenResolution))
		return true;

	//���״̬�ı���ж�������֮����Ը���ʵ�����ϸ��
	if (!lastCameraState.Location.Equals(newCameraState.Location))
		return true;
	if (!lastCameraState.Rotator.Equals(newCameraState.Rotator))
		return true;

	return false;
}

void GeoMap::OnTick(CameraState newCameraState)
{	//�ж��������	
	bool cameraChanged = IsCameraStateChange(lastCameraState, newCameraState);

	if (cameraChanged)
	{
		for (GeoLayer * currentLayer : geoLayerArray)
		{
			currentLayer->OnCameraStateChange(newCameraState, lastCameraState);
		}
	}
	lastCameraState = newCameraState;

	for (GeoLayer * currentLayer : geoLayerArray)
	{
		currentLayer->OnTick();
	}
}