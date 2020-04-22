#include "TileLoadManager.h"

//�ӵ�ǰӦ���ؼ����У������߼�����֤��Ƭ���������ԡ���Ч��
void TileGridLoadRefiningStrategy::GetRefiningLoadingTiles(const TSet<ITileInfo*> &inTilesShouldBeLoaded, TSet<ITileInfo*> &outTilesLoading)
{
	outTilesLoading = inTilesShouldBeLoaded;
	return;
}

//�ӵ�ǰӦ���ؼ����У������߼�����֤��Ƭ���������ԡ���Ч��
void OSGB_GridLoadRefiningStrategy::GetRefiningLoadingTiles(const TSet<ITileInfo*> &inTilesShouldBeLoaded, TSet<ITileInfo*> &outTilesLoading)
{
	outTilesLoading = inTilesShouldBeLoaded;
	return;
}

TileLoadManager::TileLoadManager()
{}

//��Ӧ������Ƭ����A���������Ѿ�������Ƭ����B�У����㵱ǰʵ��Ӧ�ü��ص���Ƭ����C
//��Ӧ������Ƭ����A���������Ѿ�������Ƭ����B�У����㵱ǰʵ��Ӧ��ж�ص���Ƭ����D
//C=A-B
//D=B-A
void TileLoadManager::GetLoadingTiles(const TSet<ITileInfo*> &inTilesShouldBeLoaded, TSet<ITileInfo*> &outTilesLoading, TSet<ITileInfo*> &outTilesUnloading)
{
	//����loading����
	for (ITileInfo* thisTile : inTilesShouldBeLoaded)
	{
		bool thisTileShouldBeLoading = true;	

		for (ITileInfo* tileLoaded : this->loadedTileSet)
		{		
			if (thisTile->Equal(tileLoaded))
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
		for (ITileInfo* tileShouldBeLoading : inTilesShouldBeLoaded)
		{			
			if (tileLoaded->Equal(tileShouldBeLoading))
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
void TileLoadManager::UpdateLoadedTile(ITileInfo* loadedTile)
{
	bool thisTileHasBeenLoaded = false;

	if (this->loadedTileSet.Num() == 0)
	{
		this->loadedTileSet.Add(loadedTile);
		return;
	}	

	for (ITileInfo* tileLoaded : this->loadedTileSet)
	{	
		if (loadedTile->Equal(tileLoaded))
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
void TileLoadManager::UpdateUnloadedTile(ITileInfo* unloadedTile)
{
	for (ITileInfo* tileLoaded : this->loadedTileSet)
	{		
		if (unloadedTile->Equal(tileLoaded))
		{
			this->loadedTileSet.Remove(tileLoaded);
			this->loadedTileSet.CompactStable();
			break;
		}
	}
}