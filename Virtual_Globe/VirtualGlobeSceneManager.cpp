// Fill out your copyright notice in the Description page of Project Settings.


#include "VirtualGlobeSceneManager.h"




bool AVirtualGlobeSceneManager::SetViewerCameraControllerObject()
{
	//��ȡ�����е����AViewerCameraController����
	UWorld* world = GetWorld();
	check(world);
	TArray<AActor*> _actor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AViewerCameraController::StaticClass(), _actor);
	if (_actor.Num() > 0)
	{
		AViewerCameraController* pViewerCameraController = (AViewerCameraController*)_actor[0];

		if (pViewerCameraController)
		{
			this->UE_CameraController = pViewerCameraController;
			return true;
		}
	}
	return false;
}

//������ά����Ŀռ����ó�ʼ����ͳ�ʼ���λ��
void AVirtualGlobeSceneManager::InitializeVirtualGlobeSphere()
{
	//����������������ϵͳ
	FVector radii = FVector(6378137.0, 6378137.0, 6356752.314245);
	FRotator InRotation = FRotator(0.0, 0.0, 0.0);
	FVector InTranslation = FVector(0.0, 0.0, 0.0);
	FVector InScale3D = FVector(1.0, 1.0, 1.0);

	Sphere_CoordinateSystem* earth3d_CoordinateSystem = new Sphere_CoordinateSystem(radii,
		InRotation,
		InTranslation,
		InScale3D);

	//����geomap�������ھ���ĸ�������Դ��ͼ�㣩�ļ��ع���
	this->geoCameraState.FOV = 120.0;
	//��������ĳ�ʼλ�ú�ƫת�Ƕȣ�ֱ�Ӷ�ȡ�����ʵ��λ��
	this->geoCameraState.Location = UE_CameraController->OurCamera->GetComponentLocation();
	this->geoCameraState.Rotator = UE_CameraController->OurCamera->GetComponentRotation();
	this->geoCameraState.screenResolution = FVector2D(1920, 1080);
	this->geoCameraState.AspectRatio = 2.0;

	//�Ƿ���Ҫ���Ӵ�uecamera��geocamera�ľ���任��
	this->geoCameraState.Location = worldToUE_Transform.InverseTransformPosition(geoCameraState.Location);
	this->geoCameraState.Rotator = worldToUE_Transform.InverseTransformRotation(geoCameraState.Rotator.Quaternion()).Rotator();

	//����map����
	this->virtualGlobeMap = new GeoMap(earth3d_CoordinateSystem, this->geoCameraState,this);

	//��map�����layer����Դ	
	//��ȡ�����е����UE_World_Camera
	UWorld* world = GetWorld();
	check(world);
	TArray<AActor*> _actor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridTileLoader::StaticClass(), _actor);
	if (_actor.Num() > 0)
	{
		AGridTileLoader* gridTileLoaderActor = (AGridTileLoader*)_actor[0];
		if (gridTileLoaderActor)
		{
			FString basePath = "/Game/Data/";
			GridTileLayer* pGridTileLayer = new GridTileLayer(basePath, (AActor*)gridTileLoaderActor);
			this->virtualGlobeMap->AddLayer((GeoLayer*)pGridTileLayer);
		}
	}	
}

// Sets default values
AVirtualGlobeSceneManager::AVirtualGlobeSceneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->virtualGlobeSpaceType = 0;

	worldToUE_Transform.SetScale3D(FVector(1, 1, -1));		
	FRotator NewRotator(180, 180, 0);
	worldToUE_Transform.SetRotation(NewRotator.Quaternion());
}

// Called when the game starts or when spawned
void AVirtualGlobeSceneManager::BeginPlay()
{
	Super::BeginPlay();

	//��ȡ�����е����AViewerCameraController����
	if (SetViewerCameraControllerObject())
	{
		loadCameraSuccess = true;
	}

	//����ǰ����ռ�����Ϊ��ά����
	if (loadCameraSuccess&&this->virtualGlobeSpaceType == 0)
	{
		InitializeVirtualGlobeSphere();
	}
	
}

// Called every frame
void AVirtualGlobeSceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�Ƿ���Ҫ���Ӵ�uecamera��geocamera�ľ���任��
	this->geoCameraState.Location = UE_CameraController->OurCamera->GetComponentLocation();
	this->geoCameraState.Rotator = UE_CameraController->OurCamera->GetComponentRotation();

	this->geoCameraState.Location = worldToUE_Transform.InverseTransformPosition(geoCameraState.Location);
	this->geoCameraState.Rotator = worldToUE_Transform.InverseTransformRotation(geoCameraState.Rotator.Quaternion()).Rotator();
	this->virtualGlobeMap->OnTick(this->geoCameraState);

}

