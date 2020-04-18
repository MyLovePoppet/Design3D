#pragma once

#include "CoreMinimal.h"
#include "CoordinateSystem.h"
#include "TileInfo.h"
#include "SceneCulling.h"
#include "TileLoadManager.h"
#include <queue>

#include "GridTileLoader.h"

#include "GameFramework/Actor.h"
#include "LandscapeMaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

//ʵ�ʵ���Ƭ���ء�ж�ص�ִ���࣬����UE���ݽṹ��ֻ������һ����Ƭ��ж��һ����Ƭ
class ITileLoadTaskExcutor
{
public:
	//����һ����Ƭ��ʵ�ֺ���
	virtual bool LoadOneTile(const ITileInfo* thisTile, bool isAsyn) = 0;

	virtual bool UnloadOneTile(const ITileInfo* thisTile, bool isAsyn) = 0;

	AActor* rootActor;
};

class GridTileLoadTaskExcutor:public ITileLoadTaskExcutor
{
public:

	FString dataSourcePath;	

	//����һ����Ƭ��ʵ�ֺ���
	bool LoadOneTile(const ITileInfo* thisTile, bool isAsyn);

	bool UnloadOneTile(const ITileInfo* thisTile, bool isAsyn);

	GridTileLoadTaskExcutor(FString basePath, AActor* pActor);
};

