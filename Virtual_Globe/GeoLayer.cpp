#include "GeoLayer.h"

//��Ӧ���״̬�ı亯����ʵ��ʵ�ʵ����ݼ����߼�
void GeoLayer::OnCameraStateChange(const CameraState &newCameraState, const CameraState &lastCameraState)
{
	this->sceneCulling->currentCameraState = newCameraState;

	//���� �ж�����if()

	TSet<ITileInfo *> tilesShouldBeLoaded;
	sceneCulling->GetTilesShouldbeLoaded(newCameraState, tilesShouldBeLoaded);


	TSet<ITileInfo *> refinedTilesShouldBeLoaded;
	tileLoadStrategy->GetRefiningLoadingTiles(tilesShouldBeLoaded, refinedTilesShouldBeLoaded);

	TSet<ITileInfo *> tileLoading;
	TSet<ITileInfo *> tileUnloading;
	loadManager->GetLoadingTiles(refinedTilesShouldBeLoaded, tileLoading, tileUnloading);

	for (ITileInfo * currentTile : tileLoading)
	{
		bool loadedSuccess = tileLoadTaskExcutor->LoadOneTile(currentTile, true);
		if (loadedSuccess)
		{
			loadManager->UpdateLoadedTile(currentTile);
		}
	}

	for (ITileInfo * currentTile : tileUnloading)
	{
		bool unloadedSuccess = tileLoadTaskExcutor->UnloadOneTile(currentTile, true);
		if (unloadedSuccess)
		{
			loadManager->UpdateUnloadedTile(currentTile);
		}
	}
}

//ʵ�ֵ�ǰͼ���������仯����Ҫ�����������������tick�߼�
void GeoLayer::OnTick()
{
	//SceneCulling_CenterTileStrategy* pSceneCulling_CenterTileStrategy = (SceneCulling_CenterTileStrategy*)this->sceneCulling;
	//if (pSceneCulling_CenterTileStrategy)
	//{
	//	this->pViewerCameraController->degreePerPixelInScreen = pSceneCulling_CenterTileStrategy->GetDegreePerPixelInScreen(111318, 256);
	//}
}

//void GridTileLayer::OnTick()
//{	
//	this->pViewerCameraController->degreePerPixelInScreen = ((SceneCulling_CenterTileStrategy*)this->sceneCulling)->GetDegreePerPixelInScreen(111318, 256);
//}


GridTileLayer::GridTileLayer(const FString layerSourcePath, AActor* pActor, AViewerCameraController* inViewerCameraController, CoordinateSystem* inCoordinateSystem)
{
	this->dataSourcePath = layerSourcePath;	
	this->rootActor = pActor;	

	this->pViewerCameraController = inViewerCameraController;
	this->sceneCulling = new SceneCulling_CenterTileStrategy(inCoordinateSystem);
	this->tileLoadStrategy = new TileGridLoadRefiningStrategy();
	this->loadManager = new TileGridLoadManager();
	this->tileLoadTaskExcutor = new GridTileLoadTaskExcutor(layerSourcePath, pActor);

	inViewerCameraController->pISceneCulling = this->sceneCulling;
}