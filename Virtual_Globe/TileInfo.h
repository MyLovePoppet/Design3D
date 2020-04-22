#pragma once
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "CoordinateSystem.h"


//��������ʵ�������������࣬�ḻtile�а������ݵľ����������ں�
class TileItem
{};

//���ڶ�����Ƭ������Ϣ�Ľӿڻ���
//�Ĳ�����Ƭ��r����Ƭ��osgb���ݵ�������Ƭ�ȣ�һ����Ƭ���͵�������Ϣ���̳��Ըû���
struct ITileInfo
{
public:
	
	//���ڼ�¼��Ƭ�洢�����·��
	virtual FString GetTilePath() = 0;

	virtual FTransform GetTileTransform() = 0;
		
	//TArray<TileItem> TileItemArray;

	//virtual ~TileInfo();

	//�ж�����Ƭ�ļ��Ƿ����
	virtual bool IsTileFileExist() = 0;

	//������Ƭ��Ϣ���ַ���
	virtual FString TileToString() = 0;

	virtual bool Equal(const ITileInfo* otherTile) = 0;
};

//���ε���Ƭ������Ϣ�࣬�̳���TileInfo����,ȫ����Ƭ
struct TileInfo_Grid :public ITileInfo
{
public:
	//����Ƭ�Ĳ㼶
	int32 LevelNum;
	//����Ƭ���к�
	int32 Row;
	//����Ƭ���к�
	int32 Col;	

	//����Ƭ�����½ǵ㣬��¼��άƽ���ϸ���Ƭ��x_min��y_min�������upRightPt
	FVector2D DownLeftPt;

	//����Ƭ�����Ͻǵ㣬��¼��άƽ���ϸ���Ƭ��x_max��y_max�������downLeftPt
	FVector2D UpRightPt;

	FTransform tileTransform;

	enum TilePositionInParent
	{
		SouthWest,
		SouthEast,
		NorthWest,
		NorthEast,
	};

	//static TileInfo_Grid GetTileByLevelNumAndCoord(const int32 thisLevelNum, const Geographic2D Coordinate);

	TileInfo_Grid();

	TileInfo_Grid(const int32 layerNum, const int32 Row, const int32 Col, const FTransform inTransform);

	TileInfo_Grid(const int32 layerNum, const int32 Row, const int32 Col);
	
	//���ظ���Ƭ��ϵ�£�����ԭ�����꣨-180��-90��
	FVector2D GetGeographicGridOriginPt();

	//���ڼ�¼��Ƭ�洢��·��
	FString GetTilePath();
	
	//�ж�����Ƭ�ļ��Ƿ����
	bool IsTileFileExist();

	//������Ƭ��Ϣ���ַ���
	FString TileToString();

	//�жϸ���Ƭ��������Ƭ����Ĭ�Ϲ���Ŀ���Ƭ
	bool IsTileValid();

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�������ӽڵ���Ƭ������Ϣ
	void GetChildren(TArray<TileInfo_Grid*>& children);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�ĸ��ڵ���Ƭ������Ϣ
	TileInfo_Grid* GetParent();

	//�ж�������Ƭ�Ƿ���ͬһ����Ƭ
	bool Equal(const ITileInfo *other);

	//��ȡ����Ƭ�ĸ����߳�������Ϊ������
	double GetTileGridSize();

	//�����Ĳ���ԭ�����ظ���Ƭ�ڸ��ڵ��еľ���λ�ã������ֿ���
	TilePositionInParent GetTilePositionInParent();

	//�жϾ�γ�������testPt���Ƿ��ڸ���Ƭ��Χ��
	bool IsPositionIn(const FVector testPt);

	//�жϾ�γ�������testPt���Ƿ��ڸ���Ƭ��Χ��
	bool IsPositionIn(const FVector2D testPt);

	//�жϾ�γ�ȵ����깹�ɵĵ���Χ���Ƿ������Ƭ�н���
	bool IsGeoRangeIntersect(const TArray<FVector2D> geoRange);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�İ�������Ƭ��������Ϣ
	//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
	//      012
	//      3*4
	//      567	
	void GetNeighbor_8(TArray<TileInfo_Grid*>& neighbor_8);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ����������Ƭ��������Ϣ
	//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
	//       0    
	//      1*2    
	//       3 	 
	void GetNeighbor_4(TArray<TileInfo_Grid*>& neighbor_4);

	//���ڵ�ǰ��Ƭ�����ؽ��ڵ�������Ƭ�����ҽ���������Ƭ�ͱ���Ƭ��ͬ������һ���ĸ���Ƭ
	//���������*Ϊ����Ƭ
	//   *0       0*       01       01
	//   12       12       *2       2*
	void GetNeighbor_3(TArray<TileInfo_Grid*>& neighbor_3);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ��15������Ƭ��������Ϣ
	//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
	//      0   1  2  3    0   1  2  3    0   1  2  3    0   1  2  3
	//      4   *  5  6    4   5  *  6    4   5  6  7    4   5  6  7      
	//      7   8  9 10    7   8  9 10    8   *  9 10    8   9  * 10
	//      11 12 13 14    11 12 13 14    11 12 13 14    11 12 13 14
	void GetNeighbor_15(TArray<TileInfo_Grid*>& neighbor_15);
	//��ȡ�ò㼶��Ƭ��ȫ���е�������
	int32 GetTileRowCountInGlobe();

	//��ȡ�ò㼶��Ƭ��ȫ���е�������
	int32 GetTileColCountInGlobe();

	FTransform GetTileTransform();


private:	

	//��ȡ����Ƭ�����λ���ھ�
	TileInfo_Grid* GetUpTile();
	//��ȡ����Ƭ�����λ���ھ�
	TileInfo_Grid* GetDownTile();
	//��ȡ����Ƭ�����λ���ھ�
	TileInfo_Grid* GetLeftTile();
	//��ȡ����Ƭ�����λ���ھ�
	TileInfo_Grid* GetRightTile();
	//��ȡ����Ƭ�����λ���ھ�
	TileInfo_Grid* GetUpUpTile();
	//��ȡ����Ƭ�����λ���ھ�
	TileInfo_Grid* GetDownDownTile();

};



class Tile_DEM :TileItem
{
	//�洢����Ƭ������
	int32 row_count;

	//�洢����Ƭ������
	int32 col_count;

	//����Ƭ��dem�ֱ��ʣ�xΪ����ֱ��ʣ�yΪ����ֱ���
	FVector2D dem_resolution;

	//�洢��dem��Ƭ�ĸ̷߳�Χ��xΪheight_min,yΪheight_max��λ��
	FVector2D heightRange;

	//��������Ƭ�ĵ�λ��Ϣ��meterΪ�ף�degreeΪ��
	enum unitType
	{
		METER,
		DEGREE,
	};
};

class Tile_DOM :TileItem
{
	//�洢����Ƭ������
	int32 row_count;

	//�洢����Ƭ������
	int32 col_count;

	//����Ƭ��dem�ֱ��ʣ�xΪ����ֱ��ʣ�yΪ����ֱ���
	FVector2D dem_resolution;
	
	//��������Ƭ�ĵ�λ��Ϣ��meterΪ�ף�degreeΪ��
	enum unitType
	{
		METER,
		DEGREE,
	};
};

struct TileInfo_Rtree :ITileInfo
{};

struct TileInfo_OSGB :ITileInfo
{
public:

	FString id;

	//���ӷ�Χ��Сֵ
	float min;
	//���ӷ�Χ���ֵ
	float max;

	//�ӽڵ�
	TArray<TileInfo_OSGB*> Childlist;

	//�ò㼶osgbƽ�����������
	float triangleSize;

	//�㼶
	int levelNum;

	//BoundingBox
	TArray<FVector2D> boudingBox;

	FTransform tileTransform;

	bool Equal(ITileInfo* otherTile);

	bool IsGeoRangeIntersect(const TArray<FVector2D> geoRange);

	//���ڼ�¼��Ƭ�洢��·��
	FString GetTilePath();

	//�ж�����Ƭ�ļ��Ƿ����
	bool IsTileFileExist();

	//������Ƭ��Ϣ���ַ���
	FString TileToString();

	FTransform GetTileTransform();

	static void readOSGTree(FString txtFilePath, TArray<TileInfo_OSGB> &osgTree);
};

struct TileInfo_3dModel :ITileInfo
{
public:
	bool Equal(ITileInfo* otherTile);
};