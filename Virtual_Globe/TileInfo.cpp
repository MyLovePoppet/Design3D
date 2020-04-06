
#include "TileInfo.h"

//�������Ƭ�Ĺ��캯��
TileInfo_Grid::TileInfo_Grid()
{
	this->LevelNum = -1;
	this->Row = -1;
	this->Col = -1;
}

//�жϵ�ǰ��Ƭ�Ƿ��ǺϷ���Ƭ
bool TileInfo_Grid::IsTileValid()
{
	if (this->LevelNum == -1 && this->Row == -1 && this->Col == -1)
		return false;
	return true;
}

//�ж�������Ƭ�Ƿ���ͬһ����Ƭ
bool TileInfo_Grid::Equal(TileInfo_Grid &other)
{
	if (this->LevelNum == other.LevelNum && this->Row == other.Row && this->Col == other.Col)
		return true;
	return false;
}

TileInfo_Grid::TileInfo_Grid(const int32 levelNum, const int32 row, const int32 col)
{
	this->LevelNum = levelNum;
	this->Row = row;
	this->Col = col;

	double tileGridSize = GetTileGridSize();

	this->DownLeftPt = FVector2D(GeographicGridOriginPt.X + col * tileGridSize, GeographicGridOriginPt.Y + row * tileGridSize);

	this->UpRightPt = FVector2D(GeographicGridOriginPt.X + (col + 1)*tileGridSize, GeographicGridOriginPt.Y + (row + 1)*tileGridSize);
} 

FVector2D TileInfo_Grid::GetTileMin()
{
	return this->DownLeftPt;
}

FVector2D TileInfo_Grid::GetTileMax()
{
	return this->UpRightPt;
}

double TileInfo_Grid::GetTileGridSize()
{
	return 180.0 / FMath::Pow(2.0, LevelNum);
}

//������������Χ�ڣ��ò㼶��Ƭ��������
int32 TileInfo_Grid::GetTileRowCountInGlobe()
{
	return FMath::Pow(2.0, LevelNum);
}

//������������Χ�ڣ��ò㼶��Ƭ��������
int32 TileInfo_Grid::GetTileColCountInGlobe()
{
	return FMath::Pow(2.0, LevelNum + 1);
}

//���ظ���Ƭ���ĸ�����Ƭ���ɵ�����
// 01
// 23
void TileInfo_Grid::GetChildren(TArray<TileInfo_Grid>& children)
{
	//����ǰ��ƬΪ�գ�ֱ�ӷ���
	if (this->Row == -1 && this->Col == -1 && this->LevelNum == -1)
		return;
	
	//�������������Ƭ
	children.Add(TileInfo_Grid(this->LevelNum + 1, this->Row * 2 + 1, this->Col * 2));

	//��Ӷ���������Ƭ
	children.Add(TileInfo_Grid(this->LevelNum + 1, this->Row * 2 + 1, this->Col * 2 + 1));

	//������Ͻ�����Ƭ
	children.Add(TileInfo_Grid(this->LevelNum + 1, this->Row * 2, this->Col * 2));

	//��Ӷ��Ͻ�����Ƭ
	children.Add(TileInfo_Grid(this->LevelNum + 1, this->Row * 2, this->Col * 2 + 1));
}

//���ص�ǰ��Ƭ�ĸ���Ƭ
//����ǰ��ƬΪ0����Ƭ������ƬΪ����Ƭ
TileInfo_Grid TileInfo_Grid::GetParent()
{
	TileInfo_Grid result_TileInfo_Grid = TileInfo_Grid();
	//���ؿ���Ƭ
	if (this->LevelNum == 0)
	{		
		return result_TileInfo_Grid;
	}
		
	result_TileInfo_Grid = TileInfo_Grid(this->LevelNum - 1, FMath::FloorToInt(this->Row / 2), FMath::FloorToInt(this->Col / 2));

	return result_TileInfo_Grid;
}

TileInfo_Grid TileInfo_Grid::GetTileByLevelNumAndCoord(int32 thisLevelNum, Geographic2D Coordinate)
{
	static TileInfo_Grid tile;  
	tile.LevelNum = thisLevelNum;

	int rowNum = pow(2, thisLevelNum);
	// ��������㣬ȷ���õ��������кţ�
	tile.Col = Coordinate.Longitude  * (2 * rowNum) / 360 + rowNum; // lon
	tile.Row = Coordinate.Latitude * rowNum / 180 + rowNum / 2.0;   // lat
	return tile;
}


//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�İ�������Ƭ��������Ϣ
//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
//      012
//      3*4
//      567	
void TileInfo_Grid::GetNeighbor_8(TArray<TileInfo_Grid>& neighbor_8)
{
	neighbor_8.AddDefaulted(8);
	//����ǰtileΪ��tile�����ؿ�����
	if (this->Row == -1 && this->Col == -1 && this->LevelNum == -1)
		return;

	if (this->Row != (GetTileRowCountInGlobe() - 1))
	{
		if (this->Col != 0)
		{
			neighbor_8[0] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col - 1);
			neighbor_8[3] = TileInfo_Grid(this->LevelNum, this->Row, this->Col - 1);
		}			
		else
		{
			neighbor_8[0] = TileInfo_Grid(this->LevelNum, this->Row + 1, GetTileColCountInGlobe() - 1);
			neighbor_8[3] = TileInfo_Grid(this->LevelNum, this->Row, GetTileColCountInGlobe() - 1);
		}

		neighbor_8[1] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col);

		if (this->Col != GetTileColCountInGlobe() - 1)
		{
			neighbor_8[2] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col + 1);
			neighbor_8[4] = TileInfo_Grid(this->LevelNum, this->Row, this->Col + 1);
		}
		else
		{
			neighbor_8[2] = TileInfo_Grid(this->LevelNum, this->Row + 1, 0);
			neighbor_8[4] = TileInfo_Grid(this->LevelNum, this->Row, 0);
		}
	}
	else if (this->Row != 0)
	{
		if (this->Col != 0)
		{
			neighbor_8[5] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col - 1);				
		}
		else
		{
			neighbor_8[5] = TileInfo_Grid(this->LevelNum, this->Row - 1, GetTileColCountInGlobe() - 1);			
		}

		neighbor_8[6] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col);

		if (this->Col != GetTileColCountInGlobe() - 1)
		{
			neighbor_8[7] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col + 1);			
		}
		else
		{
			neighbor_8[7] = TileInfo_Grid(this->LevelNum, this->Row - 1, 0);			
		}
	}
}



//���ڵ�ǰ��Ƭ�����ؽ��ڵ�������Ƭ�����ҽ���������Ƭ�ͱ���Ƭ��ͬ������һ���ĸ���Ƭ
//���������*Ϊ����Ƭ
//   *0       0*       01       01
//   12       12       *2       2*
void TileInfo_Grid::GetNeighbor_3(TArray<TileInfo_Grid>& neighbor_3)
{
	//neighbor_3.AddDefaulted(3);

	TArray<TileInfo_Grid> all4ChildrenTileFromParent;
	this->GetParent().GetChildren(all4ChildrenTileFromParent);
	if (all4ChildrenTileFromParent.Num() == 4)
	{		
		for (int i = 0; i < all4ChildrenTileFromParent.Num(); i++)
		{
			if (this->Equal(all4ChildrenTileFromParent[i]))
				continue;
			else
				neighbor_3.Add(all4ChildrenTileFromParent[i]);
		}
	}	
}


//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ����������Ƭ��������Ϣ
//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
//       0    
//      1*2    
//       3 	 
void TileInfo_Grid::GetNeighbor_4(TArray<TileInfo_Grid>& neighbor_4)
{
	neighbor_4.AddDefaulted(4);
	//����ǰtileΪ��tile�����ؿ�����
	if (this->Row == -1 && this->Col == -1 && this->LevelNum == -1)
		return;

	if (this->Row != (GetTileRowCountInGlobe() - 1))
		neighbor_4[0] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col);

	if (this->Col != 0)
		neighbor_4[1] = TileInfo_Grid(this->LevelNum, this->Row, this->Col - 1);
	else
		neighbor_4[1] = TileInfo_Grid(this->LevelNum, this->Row, GetTileColCountInGlobe() - 1);

	if (this->Col != GetTileColCountInGlobe() - 1)
		neighbor_4[2] = TileInfo_Grid(this->LevelNum, this->Row, this->Col + 1);
	else
		neighbor_4[2] = TileInfo_Grid(this->LevelNum, this->Row, 0);

	if (this->Row != 0)
		neighbor_4[3] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col);	
}


//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ��15������Ƭ��������Ϣ
//���������������ͼ��ʾ��*Ϊ����Ƭ������Ϊ������Ƭ
//      0   1  2  3    0   1  2  3    0   1  2  3    0   1  2  3
//      4   *  5  6    4   5  *  6    4   5  6  7    4   5  6  7      
//      7   8  9 10    7   8  9 10    8   *  9 10    8   9  * 10
//      11 12 13 14    11 12 13 14    11 12 13 14    11 12 13 14
void TileInfo_Grid::GetNeighbor_15(TArray<TileInfo_Grid>& neighbor_15)
{
	neighbor_15.AddDefaulted(15);
	//����ǰtileΪ��tile�����ؿ�����
	if (this->Row == -1 && this->Col == -1 && this->LevelNum == -1)
		return;

	//����ǰtile����̫�ͣ��޷�����15�����ֵ���Ƭ
	if (this->LevelNum <= 3)
		return;

	//1.����ǰ��Ƭ��globe��Ե
	//1.1 ����ǰ��Ƭ��globe����
	if (this->Row == this->GetTileRowCountInGlobe() - 1)
	{
		//1.1.1�ҵ�ǰ��Ƭ��globe����
		if (this->Col == 0)
		{
			neighbor_15[8] = TileInfo_Grid(this->LevelNum, this->Row, this->GetTileColCountInGlobe() - 2);
			neighbor_15[9] = TileInfo_Grid(this->LevelNum, this->Row, this->GetTileColCountInGlobe() - 1);
			neighbor_15[10] = TileInfo_Grid(this->LevelNum, this->Row, 1);
			neighbor_15[11] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->GetTileColCountInGlobe() - 2);
			neighbor_15[12] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->GetTileColCountInGlobe() - 1);
			neighbor_15[13] = TileInfo_Grid(this->LevelNum, this->Row - 1, 0);
			neighbor_15[14] = TileInfo_Grid(this->LevelNum, this->Row - 1, 1);
		}
		//1.1.2�ҵ�ǰ��Ƭ��globeβ��
		else if (this->Col == this->GetTileColCountInGlobe() - 1)
		{
			neighbor_15[8] = TileInfo_Grid(this->LevelNum, this->Row, this->GetTileColCountInGlobe() - 2);
			neighbor_15[9] = TileInfo_Grid(this->LevelNum, this->Row, 0);
			neighbor_15[10] = TileInfo_Grid(this->LevelNum, this->Row, 1);
			neighbor_15[11] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->GetTileColCountInGlobe() - 2);
			neighbor_15[12] = TileInfo_Grid(this->LevelNum, this->Row - 1, this->GetTileColCountInGlobe() - 1);
			neighbor_15[13] = TileInfo_Grid(this->LevelNum, this->Row - 1, 0);
			neighbor_15[14] = TileInfo_Grid(this->LevelNum, this->Row - 1, 1);
		}
		return;
	}
	//1.2 ����ǰ��Ƭ��globe����
	else if (this->Row == 0)
	{
		//1.2.1�ҵ�ǰ��Ƭ��globe����
		if (this->Col == 0)
		{
			neighbor_15[0] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->GetTileColCountInGlobe() - 2);
			neighbor_15[1] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->GetTileColCountInGlobe() - 1);
			neighbor_15[2] = TileInfo_Grid(this->LevelNum, this->Row + 1, 0);
			neighbor_15[3] = TileInfo_Grid(this->LevelNum, this->Row + 1, 1);
			neighbor_15[4] = TileInfo_Grid(this->LevelNum, this->Row, this->GetTileColCountInGlobe() - 2);
			neighbor_15[5] = TileInfo_Grid(this->LevelNum, this->Row, this->GetTileColCountInGlobe() - 1);
			neighbor_15[6] = TileInfo_Grid(this->LevelNum, this->Row, 1);
		}
		//1.2.2�ҵ�ǰ��Ƭ��globeβ��
		else if (this->Col == this->GetTileColCountInGlobe() - 1)
		{
			neighbor_15[0] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->GetTileColCountInGlobe() - 2);
			neighbor_15[1] = TileInfo_Grid(this->LevelNum, this->Row + 1, this->GetTileColCountInGlobe() - 1);
			neighbor_15[2] = TileInfo_Grid(this->LevelNum, this->Row + 1, 0);
			neighbor_15[3] = TileInfo_Grid(this->LevelNum, this->Row + 1, 1);
			neighbor_15[4] = TileInfo_Grid(this->LevelNum, this->Row, this->GetTileColCountInGlobe() - 2);
			neighbor_15[5] = TileInfo_Grid(this->LevelNum, this->Row, 0);
			neighbor_15[6] = TileInfo_Grid(this->LevelNum, this->Row, 1);
		}
	}

	//2.����ǰ��Ƭ����globe��Ե

	//���ݵ�ǰ��Ƭ���кţ��жϵ�ǰ��Ƭ��4*4�����е�λ��,��֤�÷�������һ������������2*2����
	//  ****    ****    ****    ****
	//  *0**    **1*    ****    ****
	//  ****    ****    *2**    **3*
	//  ****    ****    ****    ****
	//�ж����ݣ��������кŴ�0��ʼ
	//0��λ�ã��к�Ϊż�����к�Ϊ������
	//1��λ�ã��к�Ϊż�����к�Ϊż����
	//2��λ�ã��к�Ϊ�������к�Ϊ������
	//3��λ�ã��к�Ϊ�������к�Ϊż����
	int32 currentTilePositionIn16 = 0;

	if (this->Row % 2 == 0)
	{
		if (this->Col % 2 == 1)
			currentTilePositionIn16 = 0;
		else
			currentTilePositionIn16 = 1;
	}
	else
	{
		if (this->Col % 2 == 1)
			currentTilePositionIn16 = 2;
		else
			currentTilePositionIn16 = 3;
	}

	//���ݵ�ǰ��Ƭ����4*4������Ƭ�е�λ�ã���������һ���ĸ�����Ƭ�����ڸ���Ƭ������������Ƭ
	//����ǰ��Ƭ����0��λ�ã��丸��ƬΪ��������Ƭ�����ڸ���Ƭ����������������Ƭ�������������Ƭ
	TArray<TileInfo_Grid> fourUpLevelTiles;
	this->GetParent().GetParent().GetChildren(fourUpLevelTiles);
	
	TArray<TileInfo_Grid> thisLevelTilesIn16;
	for (int i = 0; i < fourUpLevelTiles.Num(); i++)
	{
		TArray<TileInfo_Grid> currentFourTileInThisLevel;
		fourUpLevelTiles[i].GetChildren(currentFourTileInThisLevel);
		for (int j = 0; j < currentFourTileInThisLevel.Num(); j++)
		{
			thisLevelTilesIn16.Add(currentFourTileInThisLevel[j]);		
		}	
	}

	//ȷ����ǰ��Ƭ��4*4�����е�λ��
	int skipIndex = 0;
	if (currentTilePositionIn16 == 0)
		skipIndex = 5;

	else if (currentTilePositionIn16 == 1)
		skipIndex = 6;

	else if (currentTilePositionIn16 == 2)
		skipIndex = 9;

	else if (currentTilePositionIn16 == 3)
		skipIndex = 10;

	int neighborIndex = 0;
	for (int k = 0; k < thisLevelTilesIn16.Num(); k++)
	{
		if (k == skipIndex)
			continue;
		else
		{
			neighbor_15[neighborIndex] = thisLevelTilesIn16[k];
			neighborIndex++;
		}
	}
}

TileNode::TileNode(int levelNum, int row, int col)
{	
	this->tileInfo = TileInfo_Grid(levelNum, row, col);		
}

TileNode::TileNode(TileInfo_Grid inputTileInfo)
{
	this->tileInfo = inputTileInfo;
}

TileNode::TileNode()
{	
}

