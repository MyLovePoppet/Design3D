
#include "CoordinateSystem.h"
#include "Math/Vector.h"


CoordinateSystem::CoordinateSystem()
{}

FVector CoordinateSystem::ToUE_CoordinateSystem(const FVector &inPt)
{
	return inPt;
}

FVector CoordinateSystem::FromUE_CoordinateSystem(const FVector &inPt)
{
	return inPt;
}

CoordinateSystem::~CoordinateSystem()
{}


/////////////////////////////////////Sphere_CoordinateSystem/////////////////////////////////////////////

Sphere_CoordinateSystem::Sphere_CoordinateSystem() {}

//������������ϵ
//Rotation(Pitch,Roll,Yaw)
//Pitch������	Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down)
//Yawƫ���ǣ�������תƽ��Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South.
//Roll����ǣ�������	Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW.

Sphere_CoordinateSystem::Sphere_CoordinateSystem(const FVector &radii,
	const FRotator& InRotation,
	const FVector &InTranslation,
	const FVector &InScale3D)
{
	if ((radii.X <= 0.0) || (radii.Y <= 0.0) || (radii.Z <= 0.0))
	{
		return;
	}

	//������������ϵ��UE����ϵ�ı任
	this->Transform = FTransform(InRotation, InTranslation, InScale3D);

	this->_radii = radii;

	this->_radiiSquared = Vector3D(_radii)*Vector3D(_radii);

	this->_radiiToTheFourth = _radiiSquared * _radiiSquared;	

	this->_oneOverRadiiSquared = Vector3D(1.0/ _radiiSquared.X, 1.0 / _radiiSquared.Y, 1.0 / _radiiSquared.Z);
	
}


FVector Sphere_CoordinateSystem::ToUE_CoordinateSystem(const FVector& inPt)
{
	Geographic3D GeographicCoordinates = Geographic3D(inPt);
	//��γ��ת�ѿ�����������ϵ
	FVector DescartesCoordinates = ToVector3F(GeographicCoordinates);
	//�ѿ������棬������ռ�任��ue ����ϵ
	return this->Transform.TransformPosition(DescartesCoordinates);
}

FVector Sphere_CoordinateSystem::FromUE_CoordinateSystem(const FVector& inPt)
{
	FVector DescartesCoordinates = this->Transform.InverseTransformPosition(inPt);
	
	return ToGeography3F(DescartesCoordinates);
}

Geographic2D Sphere_CoordinateSystem::ToGeographic2D(Vector3D &positionOnEllipsoid)
{
	Vector3D n = GeodgraphicSurfaceNormal(positionOnEllipsoid);
	return  Geographic2D(
		FMath::Atan2(n.Y, n.X),
		FMath::Asin(n.Z / n.Magnitude()));
}

//ͨ������ѿ�������ϵxyz���������Ͼ�γ�ȣ��߶�ֵ����Чλ6λ
FVector Sphere_CoordinateSystem::ToGeography3F(const FVector &inPosition)
{
	Vector3D position = Vector3D(inPosition.X, inPosition.Y, inPosition.Z);

	Vector3D p = ScaleToGeographicSurface(inPosition);
	Vector3D h = position - p;
	double height = FMath::Sign(h.Dot(position)) * h.Magnitude();

	Geographic2D resultGeo2D = ToGeographic2D(p);

	Geographic3D resultGeo3D = Geographic3D(resultGeo2D, height);

	return resultGeo3D.ToFVector();
}



Vector3D Sphere_CoordinateSystem::ScaleToGeographicSurface(const FVector &inPosition)
{
	Vector3D position = Vector3D(inPosition.X, inPosition.Y, inPosition.Z);
	double beta = 1.0 / FMath::Sqrt(
		(position.X * position.X) * _oneOverRadiiSquared.X +
		(position.Y * position.Y) * _oneOverRadiiSquared.Y +
		(position.Z * position.Z) * _oneOverRadiiSquared.Z);
	double n = Vector3D(
		beta * position.X * _oneOverRadiiSquared.X,
		beta * position.Y * _oneOverRadiiSquared.Y,
		beta * position.Z * _oneOverRadiiSquared.Z).Magnitude();
	double alpha = (1.0 - beta) * (position.Magnitude() / n);

	double x2 = position.X * position.X;
	double y2 = position.Y * position.Y;
	double z2 = position.Z * position.Z;

	double da = 0.0;
	double db = 0.0;
	double dc = 0.0;

	double s = 0.0;
	double dSdA = 1.0;

	do
	{
		alpha -= (s / dSdA);

		da = 1.0 + (alpha * _oneOverRadiiSquared.X);
		db = 1.0 + (alpha * _oneOverRadiiSquared.Y);
		dc = 1.0 + (alpha * _oneOverRadiiSquared.Z);

		double da2 = da * da;
		double db2 = db * db;
		double dc2 = dc * dc;

		double da3 = da * da2;
		double db3 = db * db2;
		double dc3 = dc * dc2;

		s = x2 / (_radiiSquared.X * da2) +
			y2 / (_radiiSquared.Y * db2) +
			z2 / (_radiiSquared.Z * dc2) - 1.0;

		dSdA = -2.0 *
			(x2 / (_radiiToTheFourth.X * da3) +
				y2 / (_radiiToTheFourth.Y * db3) +
				z2 / (_radiiToTheFourth.Z * dc3));
	} while (FMath::Abs(s) > 1e-10);

	return Vector3D(
		position.X / da,
		position.Y / db,
		position.Z / dc);
}

//ͨ����γ�ȣ��߶�ֵ����������ѿ�������ϵ��xyzֵ����Чλ6λ
FVector Sphere_CoordinateSystem::ToVector3F(const Geographic3D	&inPosition)
{
	Vector3D n = GeodgraphicSurfaceNormal(inPosition);
	Vector3D k = _radiiSquared.MultiplyComponents(n);
	double gamma = FMath::Sqrt(
		(k.X * n.X) +
		(k.Y * n.Y) +
		(k.Z * n.Z));

	Vector3D rSurface = k / gamma;
	Vector3D result3D = rSurface + (n *inPosition.Height);
	return result3D.ToVector3F();
}

Vector3D Sphere_CoordinateSystem::GeodgraphicSurfaceNormal(const FVector &inPosition) 
{	
	double cosLatitude = FMath::Cos(inPosition.Y);

	return Vector3D(
		cosLatitude * FMath::Cos(inPosition.X),
		cosLatitude * FMath::Sin(inPosition.X),
		FMath::Sin(inPosition.Y));
}

Vector3D Sphere_CoordinateSystem::GeodgraphicSurfaceNormal(const Vector3D &inPosition)
{
	//Vector3D ors = this->_oneOverRadiiSquared;
	Vector3D positionOnEllipsoid = Vector3D(inPosition.X, inPosition.Y, inPosition.Z);
	return (positionOnEllipsoid.MultiplyComponents(_oneOverRadiiSquared)).Normalize();
}

Vector3D Sphere_CoordinateSystem::GeodgraphicSurfaceNormal(const Geographic3D &inPosition)
{
	
	double cosLatitude = FMath::Cos(inPosition.Latitude);

	return Vector3D(
		cosLatitude * FMath::Cos(inPosition.Longitude),
		cosLatitude * FMath::Sin(inPosition.Longitude),
		FMath::Sin(inPosition.Latitude));
}

//ͨ����γ�ȣ���������ѿ�������ϵ��xyzֵ���߶�ֵȡ�ر�0����Чλ6λ
FVector Sphere_CoordinateSystem::ToVector3F(const Geographic2D &inPosition)
{
	return ToVector3F(Geographic3D(inPosition.Longitude, inPosition.Latitude, 0.0));
}

/////////////////////////////////////Plane_CoordinateSystem/////////////////////////////////////////////

Plane_CoordinateSystem::Plane_CoordinateSystem(const FRotator& InRotation,
	const FVector &InTranslation,
	const FVector &InScale3D)
{	
	//����ƽ������ϵ��UE����ϵ�ı任
	this->Transform = FTransform(InRotation, InTranslation, InScale3D);
}


FVector Plane_CoordinateSystem::ToUE_CoordinateSystem(const FVector& inPt)
{	
	return this->Transform.TransformPosition(inPt);
}

FVector Plane_CoordinateSystem::FromUE_CoordinateSystem(const FVector& inPt)
{
	return this->Transform.InverseTransformPosition(inPt);
}
