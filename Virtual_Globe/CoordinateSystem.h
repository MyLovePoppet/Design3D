#pragma once
//���ڶ�����ά����ϵͳ������UE�����е���ά����ϵͳ֮��ת����ϵ����

#include "CoreMinimal.h"
#include "Vector3D.h"

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

class CoordinateSystem
{
public:
	//����Origin Rotation UnitScaleInUE��ȷ���ı�����ϵͳ����һ���ⲿ����ϵͳ�ı任
	FTransform Transform;

	//������ϵͳ�е�ԭ������һ���ⲿ����ϵͳ�е�����λ��
	FVector Origin;

	//����ά����ϵͳ�е�����������һ���ⲿ����ϵͳ�е���ת�仯����	
	FRotator Rotation;

	//������ϵͳ�ĸ�����������굥λ���ⲿ����ϵͳ�ĵ�λ֮��Ļ����ϵ
	FVector UnitScaleInUE;

	CoordinateSystem();

	virtual ~CoordinateSystem();
	//������ϵͳתUE����ϵͳ������ת��
	virtual FVector FromGeoCoordinateSystem(const FVector &inPt) = 0;

	//UE����ϵͳת������ϵͳ������ת��
	virtual FVector ToGeoCoordinateSystem(const FVector &inPt) = 0;

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
	Sphere_CoordinateSystem(const FVector &radii,
		const FRotator& InRotation,
		const FVector &InTranslation,
		const FVector &InScale3D);

	FVector FromGeoCoordinateSystem(const FVector &inPt) override;

	FVector ToGeoCoordinateSystem(const FVector &inPt) override;

private:

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
};

//����ƽ������ϵͳ�࣬�̳���CoordinateSystem�����뽫����γ��������չ������ƽ����
class Plane_CoordinateSystem :CoordinateSystem
{
	Plane_CoordinateSystem(const FRotator& InRotation,
		const FVector &InTranslation,
		const FVector &InScale3D);

	FVector FromGeoCoordinateSystem(const FVector &inPt);

	FVector ToGeoCoordinateSystem(const FVector &inPt);
};