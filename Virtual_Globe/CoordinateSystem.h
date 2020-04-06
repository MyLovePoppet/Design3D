#pragma once
//���ڶ�����ά����ϵͳ������UE�����е���ά����ϵͳ֮��ת����ϵ����

#include "CoreMinimal.h"
#include "Vector3D.h"
class CoordinateSystem
{

public:
	//����Origin Rotation UnitScaleInUE��ȷ���ı�����ϵͳ��UE����ϵͳ�ı任
	FTransform Transform;


	//����ά����ϵͳ�е�ԭ����UE�����е�����λ�ã�
	//������ϵ�е�ÿһ��������UE�����е�λ��ͨ����ԭ�����õ�
	FVector Origin;

	//����ά����ϵͳ�е���������UE�����е���ת�仯������
	//������ϵ�е�ÿһ��������UE�����е�λ��ͨ������ת�任����+����ԭ�㹲ͬ����õ�
	FRotator Rotation;

	//������ϵͳ�ĸ�����������굥λ��UE�еĵ�λ֮��Ļ����ϵ��������ϵͳĬ�ϵ�λΪ���ס�
	FVector UnitScaleInUE;

	CoordinateSystem();

	virtual ~CoordinateSystem();
	//������ϵͳתUE����ϵͳ������ת��
	virtual FVector ToUE_CoordinateSystem(const FVector &inPt);

	//UE����ϵͳת������ϵͳ������ת��
	virtual FVector FromUE_CoordinateSystem(const FVector &inPt);

};


//����������γ������Ķ�ά���ݽṹ
struct Geographic2D
{
public:
	double Longitude;
	double Latitude;

public:
	Geographic2D(double longitude, double latitude)
	{
		Longitude = longitude;
		Latitude = latitude;
	}

	//Geographic2D(Geographic3D geographic3D)
	//{
	//	Longitude = geographic3D.Longitude;
	//	Latitude = geographic3D.Latitude;
	//}
};

//����������γ������+�߶ȵ���ά���ݽṹ
struct Geographic3D
{
public:

	double Longitude;
	double Latitude;
	double Height;

public:
	Geographic3D(double longitude, double latitude, double height)
	{
		Longitude = longitude;
		Latitude = latitude;
		Height = height;
	}

	Geographic3D(double longitude, double latitude)
	{
		Longitude = longitude;
		Latitude = latitude;
		Height = 0;
	}

	Geographic3D(Geographic2D geographic2D, double height)
	{
		Longitude = geographic2D.Longitude;
		Latitude = geographic2D.Latitude;
		Height = height;
	}

	Geographic3D(const FVector & inPt)
	{
		Longitude = inPt.X;
		Latitude = inPt.Y;
		Height = inPt.Z;
	}

	FVector ToFVector()
	{
		return FVector(this->Longitude, this->Latitude, this->Height);
	}

};

//������������ϵͳ�࣬�̳���CoordinateSystem
class Sphere_CoordinateSystem :public CoordinateSystem
{
private:
	//��������ϵ������������İ뾶���ȣ�Ĭ�����xoyƽ��Ϊ���ƽ��
	//xozƽ��Ϊ���������ߣ�0�Ⱦ��ߣ��и����ƽ�棬yozƽ��Ϊ������90���и����ƽ��
	FVector _radii;

	//Ӧ���ڲ�ͬ����ϵ�任����м��������ֵΪx y z��뾶ƽ��
	Vector3D _radiiSquared;

	//Ӧ���ڲ�ͬ����ϵ�任����м��������ֵΪ_radiiSquared x y zֵ��ƽ��
	Vector3D _radiiToTheFourth;	

	//Ӧ���ڲ�ͬ����ϵ�任����м��������ֵΪ_radiiSquared x y zֵ�ĵ���
	Vector3D _oneOverRadiiSquared;
	

public:

	Sphere_CoordinateSystem();
	//��ͬ��ʽ���캯������������������᳤��
	//��Ϊwgs84���򣬻�cgcs2000�����������ֵ�ֱ�Ϊ
	//x=6378130.0��y=6378130.0��z=6356750.0�����ǵ�float��Чλ��6λ��ȡֵ������׼ȷֵ
	Sphere_CoordinateSystem(float x, float y, float z,
		const FRotator& InRotation,
		const FVector &InTranslation,
		const FVector &InScale3D);

	Sphere_CoordinateSystem(const FVector &radii, 
		const FRotator& InRotation,
		const FVector &InTranslation,
		const FVector &InScale3D);

	FVector ToUE_CoordinateSystem(const FVector &inPt) override;

	FVector FromUE_CoordinateSystem(const FVector &inPt) override;

	//ͨ������ѿ�������ϵxyz���������Ͼ�γ�ȣ��߶�ֵ����Чλ6λ
	FVector ToGeography3F(const FVector &inPosition);

	Vector3D ScaleToGeographicSurface(const FVector &inPosition);

	//ͨ����γ�ȣ��߶�ֵ����������ѿ�������ϵ��xyzֵ����Чλ6λ
	FVector ToVector3F(const Geographic3D &inPosition);

	//ͨ����γ�����꣬����õ㴦��������淨��
	Vector3D GeodgraphicSurfaceNormal(const FVector &inPosition);

	Vector3D GeodgraphicSurfaceNormal(const Vector3D &inPosition);

	Vector3D GeodgraphicSurfaceNormal(const Geographic3D &inPosition);

	//ͨ����γ�ȣ���������ѿ�������ϵ��xyzֵ���߶�ֵȡ�ر�0����Чλ6λ
	FVector ToVector3F(const Geographic2D &inPosition);

	Geographic2D ToGeographic2D(Vector3D &positionOnEllipsoid);
	

	//0. mesh�е�xyz�����ĸ�buffer�У�
	//1. landscape   convertto staticmesh vertexBuffer �Ӹ��ӱ�ż���ʵ�ʸ������������
	//2. �߶ȴӱ���ת��Ϊʵ�ʵ����꣬����0.2��
	//3. vertexBuffer ת���󣬶���ķ��߷������¼���
	//4. ת�����ٴ��ȥ
};






//����ƽ������ϵͳ�࣬�̳���CoordinateSystem�����뽫����γ��������չ������ƽ����
class Plane_CoordinateSystem :CoordinateSystem
{
	Plane_CoordinateSystem(const FRotator& InRotation,
		const FVector &InTranslation,
		const FVector &InScale3D);

	FVector ToUE_CoordinateSystem(const FVector &inPt);

	FVector FromUE_CoordinateSystem(const FVector &inPt);
};


