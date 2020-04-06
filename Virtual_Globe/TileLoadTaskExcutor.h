#pragma once

#include "CoreMinimal.h"
#include "CoordinateSystem.h"
#include "TileInfo.h"
#include "SceneCulling.h"
#include "TileLoadManager.h"
#include <queue>

//ʵ�ʵ���Ƭ���ء�ж�ص�ִ���࣬����UE���ݽṹ��ֻ������һ����Ƭ��ж��һ����Ƭ
class TileLoadTaskExcutor
{
	//��Ƭ���ݵ�����ϵͳ��Ϣ������ִ�д���Ƭ����ϵͳ��UE����ϵͳ�ı任ת��
	CoordinateSystem tileDataCoordinateSystem;

	//�Ƿ��첽��������
	bool isAsyn;

	TileLoadTaskExcutor(CoordinateSystem tileDataCoordinateSystem);

	//����һ����Ƭ��ʵ�ֺ���
	void LoadOneTile(TileInfo thisTile, bool isAsyn, FVector tilePositionInUE
	,FRotator tileRotationInUE, FVector tileScaleInUE);

	void UnloadOneTile(TileInfo thisTile, bool isAsyn);
};

class LandscapeTileLoadTaskExcutor :TileLoadTaskExcutor
{
public:
	//ά����ǰ�������Ѽ��ص�landscape��Ƭ�Ķ��󼯺ϣ�
	//���ں�������tileinfo�ҵ���Ӧ�ĳ����е�ʵ�ʶ��󣬲�ж��
	//TMap<TileInfo*, LandscapeComponent*> landscapeInScene;

	//���ظú���
	void LoadOneTile(TileInfo thisTile, bool isAsyn, FVector tilePositionInUE
		, FRotator tileRotationInUE, FVector tileScaleInUE);

	void UnloadOneTile(TileInfo thisTile, bool isAsyn);
};

class StaticmeshTileLoadTaskExcutor :TileLoadTaskExcutor
{
public:
	//ά����ǰ�������Ѽ��ص�landscape��Ƭ�Ķ��󼯺ϣ�
	//���ں�������tileinfo�ҵ���Ӧ�ĳ����е�ʵ�ʶ��󣬲�ж��
	//TMap<TileInfo*, StaticmeshCompontent*> staticmeshInScene;

	//���ظú���
	void LoadOneTile(TileInfo thisTile, bool isAsyn, FVector tilePositionInUE
		, FRotator tileRotationInUE, FVector tileScaleInUE);

	void UnloadOneTile(TileInfo thisTile, bool isAsyn);
};

