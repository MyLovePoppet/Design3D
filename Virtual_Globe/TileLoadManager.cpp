#include "TileLoadManager.h"

//�ӵ�ǰӦ���ؼ����У������߼�����֤��Ƭ���������ԡ���Ч��
void TileGridLoadRefiningStrategy::GetRefiningLoadingTiles(const TSet<ITileInfo*> &inTilesShouldBeLoaded, TSet<ITileInfo*> &outTilesLoading)
{
	outTilesLoading = inTilesShouldBeLoaded;
	return;
}

TileGridLoadManager::TileGridLoadManager()
{}

//��Ӧ������Ƭ����A���������Ѿ�������Ƭ����B�У����㵱ǰʵ��Ӧ�ü��ص���Ƭ����C
//��Ӧ������Ƭ����A���������Ѿ�������Ƭ����B�У����㵱ǰʵ��Ӧ��ж�ص���Ƭ����D
//C=A-B
//D=B-A
void TileGridLoadManager::GetLoadingTiles(const TSet<ITileInfo*> &inTilesShouldBeLoaded, TSet<ITileInfo*> &outTilesLoading, TSet<ITileInfo*> &outTilesUnloading)
{
	//����loading����
	for (ITileInfo* thisTile : inTilesShouldBeLoaded)
	{
		bool thisTileShouldBeLoading = true;
		TileInfo_Grid* thisTileInfo_Grid = (TileInfo_Grid*)thisTile;
		if (thisTileInfo_Grid == NULL)
			continue;

		for (ITileInfo* tileLoaded : this->loadedTileSet)
		{
			TileInfo_Grid* thatTileInfo_Grid = (TileInfo_Grid*)tileLoaded;
			if (thatTileInfo_Grid == NULL)
				continue;

			if (thisTileInfo_Grid->Equal(thatTileInfo_Grid))
			{
				thisTileShouldBeLoading = false;
				break;
			}
		}
		if (thisTileShouldBeLoading == true)
			outTilesLoading.Add(thisTile);
	}

	//����unloading����
	for (ITileInfo* tileLoaded : this->loadedTileSet)
	{
		bool thisTileShouldBeUnloading = true;
		TileInfo_Grid* thisTileInfo_Grid = (TileInfo_Grid*)tileLoaded;
		if (thisTileInfo_Grid == NULL)
			continue;

		for (ITileInfo* tileShouldBeLoading : inTilesShouldBeLoaded)
		{
			TileInfo_Grid* thatTileInfo_Grid = (TileInfo_Grid*)tileShouldBeLoading;
			if (thatTileInfo_Grid == NULL)
				continue;
			if (thisTileInfo_Grid->Equal(thatTileInfo_Grid))
			{
				thisTileShouldBeUnloading = false;
				break;
			}
		}
		if (thisTileShouldBeUnloading == true)
			outTilesUnloading.Add(tileLoaded);
	}
}

//���ʵ���ض���Ƭ���غ��״̬���£������Ѽ�����Ƭ���ϣ�loadedTileSet��
//��loadedTile����loadedTileSet
void TileGridLoadManager::UpdateLoadedTile(ITileInfo* loadedTile)
{
	bool thisTileHasBeenLoaded = false;

	if (this->loadedTileSet.Num() == 0)
	{
		this->loadedTileSet.Add(loadedTile);
		return;
	}
	TileInfo_Grid* thisLoadedTileInfo_Grid = (TileInfo_Grid*)loadedTile;
	if (thisLoadedTileInfo_Grid == NULL)
		return;

	for (ITileInfo* tileLoaded : this->loadedTileSet)
	{
		TileInfo_Grid* thisTileInfo_Grid = (TileInfo_Grid*)tileLoaded;
		if (thisTileInfo_Grid == NULL)
			continue;

		if (thisLoadedTileInfo_Grid->Equal(thisTileInfo_Grid))
		{
			thisTileHasBeenLoaded = true;
			break;
		}
	}
	if (thisTileHasBeenLoaded == false)
		this->loadedTileSet.Add(loadedTile);
}

//���ʵ���ض���Ƭж�غ��״̬���£������Ѽ�����Ƭ���ϣ�loadedTileSet��
//��unloadedTile��loadedTileSet���Ƴ�
void TileGridLoadManager::UpdateUnloadedTile(ITileInfo* unloadedTile)
{
	TileInfo_Grid* thisUnloadedTileInfo_Grid = (TileInfo_Grid*)unloadedTile;
	if (thisUnloadedTileInfo_Grid == NULL)
		return;

	for (ITileInfo* tileLoaded : this->loadedTileSet)
	{
		TileInfo_Grid* thisTileInfo_Grid = (TileInfo_Grid*)tileLoaded;
		if (thisTileInfo_Grid == NULL)
			continue;
		if (thisUnloadedTileInfo_Grid->Equal(thisTileInfo_Grid))
		{
			this->loadedTileSet.Remove(tileLoaded);
			this->loadedTileSet.CompactStable();
			break;
		}
	}
}