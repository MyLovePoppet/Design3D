
#include "TileInfo.h"

//�ж�����Ƭ�ļ��Ƿ����
bool TileInfo_Grid::IsTileFileExist() { return false; }

//������Ƭ��Ϣ
FString TileInfo_Grid::TileToString() { return FString(); }

//��ȡ����Ƭ��ʵ���������·��
FString TileInfo_Grid::GetTileDiskPath() { return FString(); }

FString TileInfo_Grid::GetBasePath() { return FString(); }

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
bool TileInfo_Grid::Equal(TileInfo_Grid *other)
{
	if (this->LevelNum == other->LevelNum && this->Row == other->Row && this->Col == other->Col)
		return true;
	return false;
}

TileInfo_Grid::TileInfo_Grid(const int32 levelNum, const int32 row, const int32 col)
{
	this->LevelNum = levelNum;
	this->Row = row;
	this->Col = col;

	if (this->LevelNum == -1)
		return;

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

bool TileInfo_Grid::IsPositionIn(FVector testPt)
{
	if (testPt.X < this->DownLeftPt.X || testPt.X > this->UpRightPt.X
		|| testPt.Y < this->DownLeftPt.Y || testPt.Y > this->UpRightPt.Y)
		return false;
	return true;
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
	int rowNum = pow(2, thisLevelNum);
	// ��������㣬ȷ���õ��������кţ�
	int Col = Coordinate.Longitude  * (2 * rowNum) / (2 * PI) + rowNum; // lon
	int Row = Coordinate.Latitude * rowNum / PI + rowNum / 2.0;   // lat
	
	return TileInfo_Grid(thisLevelNum, Row, Col);
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
			if (this->Equal(&all4ChildrenTileFromParent[i]))
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


//��ż  ����  żż  ż��
// *0   0*    01    01
// 12   12    *2    2*
// northwest northeast southwest southeast
TileInfo_Grid::TilePositionInParent TileInfo_Grid::GetTilePositionInParent()
{
	if (this->Row % 2 == 0)
	{
		if (this->Col % 2 == 0)
			return TileInfo_Grid::TilePositionInParent::SouthWest;
		else
			return TileInfo_Grid::TilePositionInParent::SouthEast;
	}
	else
	{
		if (this->Col % 2 == 0)
			return TileInfo_Grid::TilePositionInParent::NorthWest;
		else
			return TileInfo_Grid::TilePositionInParent::NorthEast;
	}	
}

//���ص�ǰ��Ƭ�Ϸ���Ƭ
TileInfo_Grid* TileInfo_Grid::GetUpTile()
{
	//�������������򶥶ˣ��򷵻ر�����һ����Ƭ
	if (this->Row == this->GetTileRowCountInGlobe() - 1)
		return new TileInfo_Grid(this->LevelNum, this->Row, this->GetTileRowCountInGlobe() - 1 - this->Col);
	else
		return new TileInfo_Grid(this->LevelNum, this->Row+1, this->Col);
}

//���ص�ǰ��Ƭ�·���Ƭ
TileInfo_Grid* TileInfo_Grid::GetDownTile()
{
	//��������������׶ˣ��򷵻��ϼ���һ����Ƭ
	if (this->Row == 0)
		return new TileInfo_Grid(this->LevelNum, 0, this->GetTileRowCountInGlobe() - 1 - this->Col);
	else
		return new TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col);
}
//���ص�ǰ��Ƭ�����Ƭ
TileInfo_Grid* TileInfo_Grid::GetLeftTile()
{
	//������������������ˣ��򷵻����Ҷ�һ����Ƭ
	if (this->Col == 0)
		return new TileInfo_Grid(this->LevelNum, this->Row, this->GetTileRowCountInGlobe() - 1);
	else
		return new TileInfo_Grid(this->LevelNum, this->Row, this->Col - 1);
}

//���ص�ǰ��Ƭ�Ҳ���Ƭ
TileInfo_Grid* TileInfo_Grid::GetRightTile()
{
	//�����������������Ҷˣ��򷵻������һ����Ƭ
	if (this->Col == this->GetTileColCountInGlobe() - 1)
		return new TileInfo_Grid(this->LevelNum, this->Row, 0);
	else
		return new TileInfo_Grid(this->LevelNum, this->Row, this->Col + 1);
}

//���ص�ǰ��Ƭ����������Ƭ
TileInfo_Grid* TileInfo_Grid::GetUpUpTile()
{
	//�������������򶥶ˣ��򷵻ر�����һ����Ƭ
	if (this->Row == this->GetTileRowCountInGlobe() - 1)
		return this->GetUpTile()->GetDownTile();
	else
		return this->GetUpTile()->GetUpTile();
}

//���ص�ǰ��Ƭ����������Ƭ
TileInfo_Grid* TileInfo_Grid::GetDownDownTile()
{
	//��������������׶ˣ��򷵻��ϼ���һ����Ƭ
	if (this->Row == 0)
		return this->GetDownTile()->GetUpTile();
	else
		return this->GetDownTile()->GetDownTile();
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
	/*int32 currentTilePositionIn16 = 0;

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
	}*/

	//���ݵ�ǰ��Ƭ����4*4������Ƭ�е�λ�ã���������һ���ĸ�����Ƭ�����ڸ���Ƭ������������Ƭ
	//����ǰ��Ƭ����0��λ�ã��丸��ƬΪ��������Ƭ�����ڸ���Ƭ����������������Ƭ�������������Ƭ
	TArray<TileInfo_Grid> fourUpLevelTiles;
	this->GetParent().GetParent().GetChildren(fourUpLevelTiles);
	
	//TArray<TileInfo_Grid> thisLevelTilesIn16;
	int neighbor_15_index=0;
	for (int i = 0; i < fourUpLevelTiles.Num(); i++)
	{
		TArray<TileInfo_Grid> currentFourTileInThisLevel;
		fourUpLevelTiles[i].GetChildren(currentFourTileInThisLevel);
		for (int j = 0; j < currentFourTileInThisLevel.Num(); j++)
		{
			if (currentFourTileInThisLevel[j].Equal(this))
				continue;
			neighbor_15[neighbor_15_index]=currentFourTileInThisLevel[j];
			neighbor_15_index++;
		}	
	}	
}

void TileInfo_Grid::GetNeighbor_15(TArray<TileInfo_Grid*>& neighbor_15)
{
	if (this->LevelNum < 3)
		return;
	//����seed��Ƭ���������ڸò㼶��������Ƭ���ͣ��������кţ������������丸��Ƭ�е�λ��,������μ�����15����������ֵ���Ƭ
	//      0   1  2  3 
    //      4   *  5  6
	//      7   8  9 10 
	//      11 12 13 14
	if (this->GetTilePositionInParent() == TileInfo_Grid::TilePositionInParent::NorthWest)
	{
		//0
		neighbor_15.Add(this->GetLeftTile()->GetUpTile());
		//1
		neighbor_15.Add(this->GetUpTile());
		//2
		neighbor_15.Add(this->GetRightTile()->GetUpTile());
		//3
		neighbor_15.Add(this->GetRightTile()->GetRightTile()->GetUpTile());
		//4
		neighbor_15.Add(this->GetLeftTile());
		//5
		neighbor_15.Add(this->GetRightTile());
		//6
		neighbor_15.Add(this->GetRightTile()->GetRightTile());
		//7
		neighbor_15.Add(this->GetLeftTile()->GetDownTile());
		//8
		neighbor_15.Add(this->GetDownTile());
		//9
		neighbor_15.Add(this->GetRightTile()->GetDownTile());
		//10
		neighbor_15.Add(this->GetRightTile()->GetRightTile()->GetDownTile());
		//11
		neighbor_15.Add(this->GetLeftTile()->GetDownDownTile());
		//12
		neighbor_15.Add(this->GetDownDownTile());
		//13
		neighbor_15.Add(this->GetRightTile()->GetDownDownTile());
		//14
		neighbor_15.Add(this->GetRightTile()->GetRightTile()->GetDownDownTile());

	}
	// 0   1  2  3   
	// 4   5  *  6    
	// 7   8  9 10   
	// 11 12 13 14   
	if (this->GetTilePositionInParent() == TileInfo_Grid::TilePositionInParent::NorthEast)
	{
		//0
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile()->GetUpTile());
		//1
		neighbor_15.Add(this->GetLeftTile()->GetUpTile());
		//2
		neighbor_15.Add(this->GetUpTile());
		//3
		neighbor_15.Add(this->GetRightTile()->GetUpTile());
		//4
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile());
		//5
		neighbor_15.Add(this->GetLeftTile());
		//6
		neighbor_15.Add(this->GetRightTile());
		//7
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile()->GetDownTile());
		//8
		neighbor_15.Add(this->GetLeftTile()->GetDownTile());
		//9
		neighbor_15.Add(this->GetDownTile());
		//10
		neighbor_15.Add(this->GetRightTile()->GetDownTile());
		//11
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile()->GetDownDownTile());
		//12
		neighbor_15.Add(this->GetLeftTile()->GetDownDownTile());
		//13
		neighbor_15.Add(this->GetDownDownTile());
		//14
		neighbor_15.Add(this->GetRightTile()->GetDownDownTile());
	}

	// 0   1  2  3   
	// 4   5  6  7    
	// 8   *  9 10   
	// 11 12 13 14   
	if (this->GetTilePositionInParent() == TileInfo_Grid::TilePositionInParent::SouthWest)
	{
		//0
		neighbor_15.Add(this->GetLeftTile()->GetUpUpTile());
		//1
		neighbor_15.Add(this->GetUpUpTile());
		//2
		neighbor_15.Add(this->GetRightTile()->GetUpUpTile());
		//3
		neighbor_15.Add(this->GetRightTile()->GetRightTile()->GetUpUpTile());
		//4
		neighbor_15.Add(this->GetLeftTile()->GetUpTile());
		//5
		neighbor_15.Add(this->GetUpTile());
		//6
		neighbor_15.Add(this->GetRightTile()->GetUpTile());
		//7
		neighbor_15.Add(this->GetRightTile()->GetRightTile()->GetUpTile());
		//8
		neighbor_15.Add(this->GetLeftTile());
		//9
		neighbor_15.Add(this->GetRightTile());
		//10
		neighbor_15.Add(this->GetRightTile()->GetRightTile());
		//11
		neighbor_15.Add(this->GetLeftTile()->GetDownTile());
		//12
		neighbor_15.Add(this->GetDownTile());
		//13
		neighbor_15.Add(this->GetRightTile()->GetDownTile());
		//14
		neighbor_15.Add(this->GetRightTile()->GetRightTile()->GetDownTile());
	}

	// 0   1  2  3   
	// 4   5  6  7    
	// 8   9  * 10   
	// 11 12 13 14   
	if (this->GetTilePositionInParent() == TileInfo_Grid::TilePositionInParent::SouthWest)
	{
		//0
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile()->GetUpUpTile());
		//1
		neighbor_15.Add(this->GetLeftTile()->GetUpUpTile());
		//2
		neighbor_15.Add(this->GetUpUpTile());
		//3
		neighbor_15.Add(this->GetRightTile()->GetUpUpTile());
		//4
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile()->GetUpTile());
		//5
		neighbor_15.Add(this->GetLeftTile()->GetUpTile());
		//6
		neighbor_15.Add(this->GetUpTile());
		//7
		neighbor_15.Add(this->GetLeftTile()->GetUpTile());
		//8
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile());
		//9
		neighbor_15.Add(this->GetLeftTile());
		//10
		neighbor_15.Add(this->GetRightTile());
		//11
		neighbor_15.Add(this->GetLeftTile()->GetLeftTile()->GetDownTile());
		//12
		neighbor_15.Add(this->GetLeftTile()->GetDownTile());
		//13
		neighbor_15.Add(this->GetDownTile());
		//14
		neighbor_15.Add(this->GetRightTile()->GetDownTile());
	}
}

TileNode::TileNode(int levelNum, int row, int col)
{	
	this->tileInfo = new TileInfo_Grid(levelNum, row, col);		
}

TileNode::TileNode(int levelNum, int row, int col, TileNode* parent)
{
	this->tileInfo = new TileInfo_Grid(levelNum, row, col);
	this->parent = parent;
}

TileNode::TileNode(TileInfo_Grid inputTileInfo)
{
	this->tileInfo = &inputTileInfo;
}

TileNode::TileNode(TileInfo_Grid * inputTileInfo)
{
	this->tileInfo = inputTileInfo;
}

TileNode::TileNode()
{	
}

void TileNode::CreateSubTileNode()
{
	TileInfo_Grid * thisTileInfo_Grid = (TileInfo_Grid *)(this->tileInfo);
	if (thisTileInfo_Grid != NULL)
	{
		if(this->children.Num() != 0)
			this->children.RemoveAt(0,this->children.Num());

		if (thisTileInfo_Grid->LevelNum == -1)
		{
			this->children.Add(new TileNode(0, 0, 0, this));
			this->children.Add(new TileNode(0, 0, 1, this));
			return;
		}		
		//�������������Ƭ
		this->children.Add(new TileNode(thisTileInfo_Grid->LevelNum + 1, thisTileInfo_Grid->Row * 2 + 1, thisTileInfo_Grid->Col * 2, this));		
		//��Ӷ���������Ƭ
		this->children.Add(new TileNode(thisTileInfo_Grid->LevelNum + 1, thisTileInfo_Grid->Row * 2 + 1, thisTileInfo_Grid->Col * 2 + 1, this));
		//������Ͻ�����Ƭ
		this->children.Add(new TileNode(thisTileInfo_Grid->LevelNum + 1, thisTileInfo_Grid->Row * 2, thisTileInfo_Grid->Col * 2, this));		
		//��Ӷ��Ͻ�����Ƭ
		this->children.Add(new TileNode(thisTileInfo_Grid->LevelNum + 1, thisTileInfo_Grid->Row * 2, thisTileInfo_Grid->Col * 2 + 1, this));
	}
}

