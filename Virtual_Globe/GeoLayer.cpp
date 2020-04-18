#include "GeoLayer.h"

//��Ӧ���״̬�ı亯����ʵ��ʵ�ʵ����ݼ����߼�
void GeoLayer::OnCameraStateChange(const CameraState &newCameraState, const CameraState &lastCameraState)
{
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
{}


GridTileLayer::GridTileLayer(const FString layerSourcePath, AActor* pActor)
{
	this->dataSourcePath = layerSourcePath;	
	this->rootActor = pActor;	

	this->sceneCulling = new SceneCulling_CenterTileStrategy();
	this->tileLoadStrategy = new TileGridLoadRefiningStrategy();
	this->loadManager = new TileGridLoadManager();
	this->tileLoadTaskExcutor = new GridTileLoadTaskExcutor(layerSourcePath, pActor);
}