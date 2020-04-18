// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTileLoader.h"

// Sets default values
AGridTileLoader::AGridTileLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��ȡLandcapeMesh�Ĳ���
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> LandscapeMesh_Mat_Inst(TEXT("MaterialInstance'/Game/MeshMaterial_Inst.MeshMaterial_Inst'"));
	if (LandscapeMesh_Mat_Inst.Succeeded())
	{
		MeshMaterial = LandscapeMesh_Mat_Inst.Object;
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

}

// Called when the game starts or when spawned
void AGridTileLoader::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridTileLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool AGridTileLoader::LoadStaticMesh_Tile(TileInfo_Grid* tileLoading)
{	
	int32 LevelNum = tileLoading->LevelNum;
	int32 XNum = tileLoading->Row;
	int32 YNum = tileLoading->Col;

	//��ȡTileMesh
	FString TileMeshFolder = "/Game/Data/" + FString::FromInt(LevelNum) + "/" + FString::FromInt(YNum) + "/" + FString::FromInt(XNum) + "/";
	FString TileMeshName = "mesh-" + FString::FromInt(LevelNum) + "_" + FString::FromInt(YNum) + "_" + FString::FromInt(XNum);
	FString TileMeshUassectName = "StaticMesh'" + TileMeshFolder + TileMeshName + "." + TileMeshName + "'";
	UStaticMesh* TileMesh = LoadMeshFromPath(FName(*TileMeshUassectName));

	if (!TileMesh)
	{
		FString Message = "Loadinig Tile No Data: " + TileMeshName;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
		return false;
	}

	//����TileMeshComponent
	UStaticMeshComponent* TileMeshComponent = NewObject<UStaticMeshComponent>(this);
	this->AddOwnedComponent(TileMeshComponent);
	TileMeshComponent->AttachTo(RootComponent);

	//����TileMeshComponent�ļ���ΪTileMesh
	TileMeshComponent->SetStaticMesh(TileMesh);
	//����TileMeshComponent�Ĳ��ʺ�����
	FString TextureName = "/Game/Data/" + FString::FromInt(LevelNum) + "/" + FString::FromInt(YNum) + "/" + FString::FromInt(XNum) + "/dom256";
	UTexture* MaterialTexture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *(TextureName)));
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MeshMaterial, this);
	DynamicMaterial->SetTextureParameterValue(FName("TextureParam"), MaterialTexture);
	TileMeshComponent->SetMaterial(0, DynamicMaterial);

	TileMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TileMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	TileMeshComponent->RegisterComponent();
	TileMeshComponent->MarkRenderStateDirty();

	//��mXYtoComponentMap��¼��TileMeshComponent�Ĳ�ź����к�
	//��ͨ��UStaticMeshComponent* TileMeshComponent = mXYtoComponentMap.FindRef(FVector(LevelNum, XNum, YNum))��ȡ�ض����кŵ�TileMeshComponent
	mXYtoComponentMap.Add(FVector(LevelNum, XNum, YNum), TileMeshComponent);


	FString Message = "Loadinig Tile Success: " + TileMeshName;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);

	return true;
}


bool AGridTileLoader::UnLoadStaticMesh_Tile(TileInfo_Grid* tileUnloading)
{	
	int32 LevelNum = tileUnloading->LevelNum;
	int32 XNum = tileUnloading->Row;
	int32 YNum = tileUnloading->Col;

	UStaticMeshComponent* TileMeshComponent = mXYtoComponentMap.FindRef(FVector(LevelNum, XNum, YNum));
	if (!TileMeshComponent)
		return false;

	TileMeshComponent->DestroyComponent();

	FString Message = "UnLoadinig Tile: Mesh-" + FString::FromInt(LevelNum) + "_" + FString::FromInt(YNum) + "_" + FString::FromInt(XNum);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
	return true;
}

