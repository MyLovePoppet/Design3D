#pragma once
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "CoordinateSystem.h"


//���������
class TileItem
{};

//���ڶ�����Ƭ������Ϣ�Ļ���
//�Ĳ�����Ƭ��r����Ƭ��osgb���ݵ�������Ƭ�ȣ�һ����Ƭ���͵�������Ϣ���̳��Ըû���
struct TileInfo
{
public:
	//���ڼ�¼��Ƭ�洢�ĸ�Ŀ¼·��
	//FString BasePath;

	//���ڼ�¼��Ƭ�洢�����·��
	FString TilePath;
		
	TArray<TileItem> TileItemArray;

	//�ж�����Ƭ�ļ��Ƿ����
	bool IsTileFileExist();

	//������Ƭ��Ϣ
	FString TileToString();

	//ͨ����infoString������tileInfo
	//TileInfo();

	//��ȡ����Ƭ��ʵ���������·��
	FString GetTileDiskPath();

	FString GetBasePath();
};

//���ε���Ƭ������Ϣ�࣬�̳���TileInfo����,ȫ����Ƭ
struct TileInfo_Grid :TileInfo
{
public:
	//����Ƭ�Ĳ㼶
	int32 LevelNum;
	//����Ƭ���к�
	int32 Row;
	//����Ƭ���к�
	int32 Col;	

	FVector2D GeographicGridOriginPt = FVector2D(-180.0, -90.0);

	//����Ƭ�����½ǵ㣬��¼��άƽ���ϸ���Ƭ��x_min��y_min�������upRightPt
	FVector2D DownLeftPt;

	//����Ƭ�����Ͻǵ㣬��¼��άƽ���ϸ���Ƭ��x_max��y_max�������downLeftPt
	FVector2D UpRightPt;

	TileInfo_Grid();

	TileInfo_Grid(const int32 layerNum, const int32 Row, const int32 Col);

	bool IsTileValid();

	double GetTileGridSize();

	int32 GetTileRowCountInGlobe();

	int32 GetTileColCountInGlobe();

	FVector2D GetTileMin();

	FVector2D GetTileMax();

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�İ�������Ƭ��������Ϣ
	//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
	//      012
	//      3*4
	//      567	
	void GetNeighbor_8(TArray<TileInfo_Grid>& neighbor_8);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ����������Ƭ��������Ϣ
	//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
	//       0    
	//      1*2    
	//       3 	 
	void GetNeighbor_4(TArray<TileInfo_Grid>& neighbor_4);

	//���ڵ�ǰ��Ƭ�����ؽ��ڵ�������Ƭ�����ҽ���������Ƭ�ͱ���Ƭ��ͬ������һ���ĸ���Ƭ
	//���������*Ϊ����Ƭ
	//   *0       0*       01       01
	//   12       12       *2       2*
	void GetNeighbor_3(TArray<TileInfo_Grid>& neighbor_3);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ��15������Ƭ��������Ϣ
	//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
	//      0   1  2  3    0   1  2  3    0   1  2  3    0   1  2  3
	//      4   *  5  6    4   5  *  6    4   5  6  7    4   5  6  7      
	//      7   8  9 10    7   8  9 10    8   *  9 10    8   9  * 10
	//      11 12 13 14    11 12 13 14    11 12 13 14    11 12 13 14
	void GetNeighbor_15(TArray<TileInfo_Grid>& neighbor_15);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�������ӽڵ���Ƭ������Ϣ
	void GetChildren(TArray<TileInfo_Grid>& children);

	//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�ĸ��ڵ���Ƭ������Ϣ
	TileInfo_Grid GetParent();

	//�ж�������Ƭ�Ƿ���ͬһ����Ƭ
	bool Equal(TileInfo_Grid &other);

	static TileInfo_Grid GetTileByLevelNumAndCoord(int32 thisLevelNum, Geographic2D Coordinate);
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

class TileInfo_Rtree :TileInfo
{};

class TileInfo_OSGB :TileInfo
{};


//Ӧ�����������Ĳ�����R����OSGB������ṹ����Ƭ�ڵ����ݽṹ
struct TileNode
{
public:
	//����Ƭ�ڵ����Ƭ������Ϣ
	TileInfo tileInfo;
	//����Ƭ�ڵ�ĺ��ӽڵ㼯��	
	TArray<TileNode*> children;
	//����Ƭ�ڵ�ĸ��ڵ�
	TileNode *parent;
	//�����ض������������ж��ýڵ�ĺ��ӽڵ���Ƭ�Ƿ������
	bool IsAllChildrenExist();

	TileNode(int levelNum, int row, int col);

	TileNode(TileInfo_Grid inputTileInfo);

};

//�Ĳ�����Ƭ�ڵ����ݽṹ������IsAllChildrenExist����
//��children�����㶨Ϊ4
//struct TileNode_Quadtree:TileNode
//{
//
//
//};

//R����Ƭ�ڵ����ݽṹ������IsAllChildrenExist����
//��children������������仯
//struct TileNode_Rtree :TileNode
//{
//
//};

//һ�㻯�ķ����ṹ�洢����Ƭ����
struct TileSet
{
	//ͨ��һά����洢������Ƭ����
	void GetTileNodeSet(TArray<TileNode>& tileNodeArray);
};

struct TreeTileSet :TileSet
{
	TileNode* root;
};

struct ArrayTileSet :TileSet
{
	TArray<TileNode> tileNodeArray;
};

//������Ҫ�õ�ʱ���ٶ��������
class wholeQuadtree
{
public:
	//���Ĳ����ܲ���
	int32 layerCount;
	
	//���Ĳ�����������Ƭ�ڵ�ļ���
	//���ղ�ͬ�㼶�洢���Ĳ�����Ƭ�ڵ����ݼ�
	//ÿһ��tileset��Ӧ���Ĳ����е�һ��������Ƭ�ڵ�
	//quadtreeTileNodeSet�ĳ���Ϊ��ǰ�Ĳ������ܲ���
	TArray<TileSet> quadtreeTileNodeSet;

	//��wholeQuadtree��Ϣд��Ԫ����
	void ToMetadata(FString metaDataPath);

	//��ȡԪ���ݣ����ɸ�wholeQuadtree
	wholeQuadtree(FString metaDataPath); 

};


