#include "TileInfo.h"

/////////////////////////////////////////////////////////////TileInfo_Grid/////////////////////////////////////////////////////////////
TileInfo_Grid TileInfo_Grid::GetTileByLevelNumAndCoord(const int32 thisLevelNum, const Geographic2D Coordinate)
{
	int rowNum = pow(2, thisLevelNum);
	// ��������㣬ȷ���õ��������кţ�
	int Col = Coordinate.Longitude  * (2 * rowNum) / (2 * PI) + rowNum; // lon
	int Row = Coordinate.Latitude * rowNum / PI + rowNum / 2.0;   // lat

	return TileInfo_Grid(thisLevelNum, Row, Col, FTransform());
}

//�������Ƭ�Ĺ��캯��
TileInfo_Grid::TileInfo_Grid()
{
	this->LevelNum = -1;
	this->Row = -1;
	this->Col = -1;
}

double TileInfo_Grid::GetTileGridSize() { return 180.0 / FMath::Pow(2.0, LevelNum); }

//������������Χ�ڣ��ò㼶��Ƭ��������
int32 TileInfo_Grid::GetTileRowCountInGlobe() { return FMath::Pow(2.0, LevelNum); }

//������������Χ�ڣ��ò㼶��Ƭ��������
int32 TileInfo_Grid::GetTileColCountInGlobe() { return FMath::Pow(2.0, LevelNum + 1); }

//���ݵ�ǰ��Ƭ�㼶���кţ��кţ���������Ƭ����
TileInfo_Grid::TileInfo_Grid(const int32 levelNum, const int32 row, const int32 col, const FTransform inTransform)
{
	this->LevelNum = levelNum;
	this->Row = row;
	this->Col = col;
	this->tileTransform = inTransform;

	if (this->LevelNum == -1)
		return;

	double tileGridSize = GetTileGridSize();

	FVector2D GeographicGridOriginPt = GetGeographicGridOriginPt();

	this->DownLeftPt = FVector2D(GeographicGridOriginPt.X + col * tileGridSize, GeographicGridOriginPt.Y + row * tileGridSize);

	this->UpRightPt = FVector2D(GeographicGridOriginPt.X + (col + 1)*tileGridSize, GeographicGridOriginPt.Y + (row + 1)*tileGridSize);
}


//���ݵ�ǰ��Ƭ�㼶���кţ��кţ���������Ƭ����
TileInfo_Grid::TileInfo_Grid(const int32 levelNum, const int32 row, const int32 col)
{
	this->LevelNum = levelNum;
	this->Row = row;
	this->Col = col;	

	if (this->LevelNum == -1)
		return;

	double tileGridSize = GetTileGridSize();

	FVector2D GeographicGridOriginPt = GetGeographicGridOriginPt();

	this->DownLeftPt = FVector2D(GeographicGridOriginPt.X + col * tileGridSize, GeographicGridOriginPt.Y + row * tileGridSize);

	this->UpRightPt = FVector2D(GeographicGridOriginPt.X + (col + 1)*tileGridSize, GeographicGridOriginPt.Y + (row + 1)*tileGridSize);
}

FVector2D TileInfo_Grid::GetGeographicGridOriginPt(){	return FVector2D(-180.0, -90.0);}

//���ڼ�¼��Ƭ�洢��·��
FString TileInfo_Grid::GetTilePath(){	return FString();}

//���ص�ǰ��Ƭ������ⲿ����ϵͳ�ı仯����
FTransform TileInfo_Grid::GetTileTransform(){	return this->tileTransform;}

//�ж�����Ƭ�ļ��Ƿ����
bool TileInfo_Grid::IsTileFileExist() { return false; }

//������Ƭ��Ϣ
FString TileInfo_Grid::TileToString() { return FString(); }

//�жϵ�ǰ��Ƭ�Ƿ��ǺϷ���Ƭ
bool TileInfo_Grid::IsTileValid()
{
	if (this->LevelNum == -1 && this->Row == -1 && this->Col == -1)
		return false;
	return true;
}

//���ظ���Ƭ���ĸ�����Ƭ���ɵ�����
// 01
// 23
void TileInfo_Grid::GetChildren(TArray<TileInfo_Grid*>& children)
{
	//����ǰ��ƬΪ�գ�ֱ�ӷ���
	if (this->Row == -1 && this->Col == -1 && this->LevelNum == -1)
		return;

	//�������������Ƭ
	children.Add(new TileInfo_Grid(this->LevelNum + 1, this->Row * 2 + 1, this->Col * 2,this->tileTransform));

	//��Ӷ���������Ƭ
	children.Add(new TileInfo_Grid(this->LevelNum + 1, this->Row * 2 + 1, this->Col * 2 + 1, this->tileTransform));

	//������Ͻ�����Ƭ
	children.Add(new TileInfo_Grid(this->LevelNum + 1, this->Row * 2, this->Col * 2, this->tileTransform));

	//��Ӷ��Ͻ�����Ƭ
	children.Add(new TileInfo_Grid(this->LevelNum + 1, this->Row * 2, this->Col * 2 + 1, this->tileTransform));
}

//���ص�ǰ��Ƭ�ĸ���Ƭ
//����ǰ��ƬΪ0����Ƭ������ƬΪ����Ƭ
TileInfo_Grid* TileInfo_Grid::GetParent()
{
	TileInfo_Grid* result_TileInfo_Grid = new TileInfo_Grid();
	//���ؿ���Ƭ
	if (this->LevelNum == 0)
	{
		return result_TileInfo_Grid;
	}

	result_TileInfo_Grid = new TileInfo_Grid(this->LevelNum - 1, FMath::FloorToInt(this->Row / 2), FMath::FloorToInt(this->Col / 2),this->tileTransform);

	return result_TileInfo_Grid;
}

//�ж�������Ƭ�Ƿ���ͬһ����Ƭ
bool TileInfo_Grid::Equal(const TileInfo_Grid *other)
{
	if (this->LevelNum == other->LevelNum && this->Row == other->Row && this->Col == other->Col)
		return true;
	return false;
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



bool TileInfo_Grid::IsPositionIn(const FVector testPt)
{
	if (testPt.X < this->DownLeftPt.X || testPt.X > this->UpRightPt.X
		|| testPt.Y < this->DownLeftPt.Y || testPt.Y > this->UpRightPt.Y)
		return false;
	return true;
}

bool TileInfo_Grid::IsPositionIn(const FVector2D testPt)
{
	if (testPt.X < this->DownLeftPt.X || testPt.X > this->UpRightPt.X
		|| testPt.Y < this->DownLeftPt.Y || testPt.Y > this->UpRightPt.Y)
		return false;
	return true;
}

//�жϸ�������Χ�����Σ��Ƿ��뱾��Ƭ�ཻ
bool TileInfo_Grid::IsGeoRangeIntersect(const TArray<FVector2D> geoRange)
{	
	float MaxLeft = 0;
	float MaxBottom = 0;
	float MinRight = 0;
	float MinTop = 0;

	//Ӌ��ɾ��ο��ܵ��ཻ���ε�߅��  
	MaxLeft = FMath::Max(this->DownLeftPt.X, geoRange[0].X);
	MaxBottom = FMath::Max(this->DownLeftPt.Y, geoRange[0].Y);
	MinRight = FMath::Min(this->UpRightPt.X, geoRange[2].X);
	MinTop = FMath::Min(this->UpRightPt.Y, geoRange[2].Y);
	// �Д��Ƿ��ཻ  
	if (MaxLeft > MinRight || MaxBottom > MinTop)
	{
		return false;
	}
	else
	{
		return true;
	}
}


//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ�İ�������Ƭ��������Ϣ
//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
//      012
//      3*4
//      567	
void TileInfo_Grid::GetNeighbor_8(TArray<TileInfo_Grid*>& neighbor_8)
{
	neighbor_8.AddDefaulted(8);
	//����ǰtileΪ��tile�����ؿ�����
	if (this->Row == -1 && this->Col == -1 && this->LevelNum == -1)
		return;

	if (this->Row != (GetTileRowCountInGlobe() - 1))
	{
		if (this->Col != 0)
		{
			neighbor_8[0] = new TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col - 1);
			neighbor_8[3] = new TileInfo_Grid(this->LevelNum, this->Row, this->Col - 1);
		}			
		else
		{
			neighbor_8[0] = new TileInfo_Grid(this->LevelNum, this->Row + 1, GetTileColCountInGlobe() - 1);
			neighbor_8[3] = new TileInfo_Grid(this->LevelNum, this->Row, GetTileColCountInGlobe() - 1);
		}

		neighbor_8[1] = new TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col);

		if (this->Col != GetTileColCountInGlobe() - 1)
		{
			neighbor_8[2] = new TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col + 1);
			neighbor_8[4] = new TileInfo_Grid(this->LevelNum, this->Row, this->Col + 1);
		}
		else
		{
			neighbor_8[2] = new TileInfo_Grid(this->LevelNum, this->Row + 1, 0);
			neighbor_8[4] = new TileInfo_Grid(this->LevelNum, this->Row, 0);
		}
	}
	else if (this->Row != 0)
	{
		if (this->Col != 0)
		{
			neighbor_8[5] = new TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col - 1);				
		}
		else
		{
			neighbor_8[5] = new TileInfo_Grid(this->LevelNum, this->Row - 1, GetTileColCountInGlobe() - 1);			
		}

		neighbor_8[6] = new TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col);

		if (this->Col != GetTileColCountInGlobe() - 1)
		{
			neighbor_8[7] = new TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col + 1);			
		}
		else
		{
			neighbor_8[7] = new TileInfo_Grid(this->LevelNum, this->Row - 1, 0);			
		}
	}
}



//���ڵ�ǰ��Ƭ�����ؽ��ڵ�������Ƭ�����ҽ���������Ƭ�ͱ���Ƭ��ͬ������һ���ĸ���Ƭ
//���������*Ϊ����Ƭ
//   *0       0*       01       01
//   12       12       *2       2*
void TileInfo_Grid::GetNeighbor_3(TArray<TileInfo_Grid*>& neighbor_3)
{
	//neighbor_3.AddDefaulted(3);

	TArray<TileInfo_Grid*> all4ChildrenTileFromParent;	
	this->GetParent()->GetChildren(all4ChildrenTileFromParent);
	if (all4ChildrenTileFromParent.Num() == 4)
	{		
		for (TileInfo_Grid* thisNeighbor : all4ChildrenTileFromParent)
		{
			if (!this->Equal(thisNeighbor))
				neighbor_3.Add(thisNeighbor);
		}		
	}	
}


//���ڱ���Ƭ������Ϣ���Ĳ���������򣬷��ر���Ƭ����������Ƭ��������Ϣ
//���������������ͼ��ʾ��*Ϊ����Ƭ��+Ϊ������Ƭ
//       0    
//      1*2    
//       3 	 
void TileInfo_Grid::GetNeighbor_4(TArray<TileInfo_Grid*>& neighbor_4)
{
	neighbor_4.AddDefaulted(4);
	//����ǰtileΪ��tile�����ؿ�����
	if (this->Row == -1 && this->Col == -1 && this->LevelNum == -1)
		return;

	if (this->Row != (GetTileRowCountInGlobe() - 1))
		neighbor_4[0] = new TileInfo_Grid(this->LevelNum, this->Row + 1, this->Col);

	if (this->Col != 0)
		neighbor_4[1] = new TileInfo_Grid(this->LevelNum, this->Row, this->Col - 1);
	else
		neighbor_4[1] = new TileInfo_Grid(this->LevelNum, this->Row, GetTileColCountInGlobe() - 1);

	if (this->Col != GetTileColCountInGlobe() - 1)
		neighbor_4[2] = new TileInfo_Grid(this->LevelNum, this->Row, this->Col + 1);
	else
		neighbor_4[2] = new TileInfo_Grid(this->LevelNum, this->Row, 0);

	if (this->Row != 0)
		neighbor_4[3] = new TileInfo_Grid(this->LevelNum, this->Row - 1, this->Col);
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