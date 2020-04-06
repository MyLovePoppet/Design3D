#include "TileLoadManager.h"

TileLoadManager::TileLoadManager()
{}

TSet<TileNode*> TileLoadManager::UpdateLoadingTileArray(TSet<TileNode*> tileShouldBeLoaded)
{
	TSet<TileNode*> loadingTileArray;
	if (tileShouldBeLoaded.Num() == 0 && this->loadedTileSet.Num() == 0)
		return loadingTileArray;

	loadingTileArray = tileShouldBeLoaded.Difference(this->loadedTileSet);

	//���µ���Ҫ���ص���Ƭ���ϸ�ֵ��Loadedset������loadedset��������Ҫʵ���Ѿ���ɼ��ص���Ƭ�߼���ά��
	loadedTileSet = loadingTileArray;

	return loadingTileArray;
}

TSet<TileNode*> TileLoadManager::UpdateUnLoadingTileArray(TSet<TileNode*> tileShouldBeLoaded)
{
	TSet<TileNode*> loadingTileArray;
	if (tileShouldBeLoaded.Num() == 0 && this->loadedTileSet.Num() == 0)
		return loadingTileArray;

	loadingTileArray = this->loadedTileSet.Difference(tileShouldBeLoaded);

	return loadingTileArray;
}