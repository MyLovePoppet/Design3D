#pragma once

#include "CoreMinimal.h"
#include "CoordinateSystem.h"
#include "TileInfo.h"
#include "SceneCulling.h"
#include <queue>

class TileLoadManager
{
public:
		
	
	TSet<TileNode*> loadedTileSet;
	//
	TileLoadManager();


	//ʵ����Ƭ���ز��ԣ�

	//1.ͨ��worldCameraManager�������seedtileNode��ȷ��15�������ͬ�㼶�Ĵ�������Ƭ
	//ͨ������tileInfo.GetNeighbor_15(TArray<TileInfo_Quadtree>& neighbor_15)����
	//2.�ں���16����Ƭ�Ļ����ϣ����������Ĵ�һ�����������Ƭ
	//3.ѭ��2���裬ֱ�����������Ƭ����������Ļ�������Ѽ��㵽���һ����Ƭ
	/*void GetTileShouldBeLoaded(TileNode_Quadtree seedTileNode, FVector2D screenResolution
		, TArray<TileSet> &tileNodeShouldBeLoaded);*/
		//4.��ȡ���ϴ�һ����Ԥ�ȼ��ص���Ƭ����
	/*void GetRoughTileShouldBeLoaded(TArray<TileSet> &tileNodeShouldBeLoaded
		, int32 roughingStep, TArray<TileSet> &roughTileNodeShouldBeLoaded);*/

	//5.������Ƭ�ڵ�ļ��غ�ж�ض���,���ڸ���tileNode��tileLoadState�����ж�
	//�Ѽ�������Ҫ���صĴӼ��ض������Ƴ����Ѽ����Ҳ���Ҫ���صģ�����ж�ض���......
	TSet<TileNode*> UpdateLoadingTileArray(TSet<TileNode*> tileShouldBeLoaded);

	TSet<TileNode*> UpdateUnLoadingTileArray(TSet<TileNode*> tileShouldBeLoaded);

	void UpdateLoadedTileArray(TileNode* loadedTileNode);

	

	//6.�ھ���ļ��غ�ж���������У�ִ�о���ļ��غ�ж�ز��裬����UE�ӿ�
	void DoLoadingTasks(TArray<TileNode*> loadingTileQueue, bool isAsyn);

	void DoUnloadingTasks(TArray<TileNode*> unloadingTileQueue, bool isAsyn);
	

	//�����ýڵ�ļ���״̬�����Ƿŵ� tileloadmanager����
	enum tileLoadState
	{
		LOADED,//Ӧ���أ����Ѽ���
		UNLOADED,//Ӧ���أ���δ����		
		REPLACED_BY_CHILDREN,//�Ѽ��أ��ѱ��ӽڵ��滻
	};

};
//������ּ����߼�


