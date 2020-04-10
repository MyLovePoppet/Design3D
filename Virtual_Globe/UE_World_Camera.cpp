// Fill out your copyright notice in the Description page of Project Settings.
#include "UE_World_Camera.h"
#include "CoordinateSystem.h"
#include "TileInfo.h"
#include "TileLoadManager.h"


// Sets default values
AUE_World_Camera::AUE_World_Camera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�������
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("OurCameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-90.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 40.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);
	OurCamera->bUsePawnControlRotation = false;
	float defaultFOV = OurCamera->FieldOfView;
	OurCamera->FieldOfView = 120.0;

	OurCamera->SetActive(true);

	//����Ĭ�����
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AUE_World_Camera::InitializeVirtualGlobe()
{
	//������������
	//FVector radii = FVector(6378137.0, 6378137.0, 6356752.314245);
	FVector radii = SphereR;
	FRotator InRotation = FRotator(0.0, 0.0, 0.0);
	FVector InTranslation = SphereOrigin;
	FVector InScale3D = FVector(1.0, 1.0, 1.0);

	Sphere_CoordinateSystem earth3d_CoordinateSystem0 = Sphere_CoordinateSystem(radii,
		InRotation,
		InTranslation,
		InScale3D); 

	//���������ʼλ��
	FVector inPt0 = FVector(FMath::DegreesToRadians(114.30), FMath::DegreesToRadians(30.51), 10000.0);	

	FVector outPt0 = earth3d_CoordinateSystem0.ToUE_CoordinateSystem(inPt0);

	//�������λ��
	//SetActorLocation(outPt0);

	this->UE_CameraState.FOV = 120.0;
	this->UE_CameraState.Location = outPt0;
	this->UE_CameraState.Rotator = FRotator(0.0, 0.0, 0.0);
	this->UE_CameraState.screenResolution = FVector2D(1920, 1080);
	this->UE_CameraState.AspectRatio = 2.0;

	
	//���Ի����������Ƭ�����߼�
	this->sccts = SceneCulling_CenterTileStrategy(earth3d_CoordinateSystem0,this->UE_CameraState);
	this->tlm = TileLoadManager();

	
	

	//����pt0����
	//TSet<TileNode *> shouldLoadingTileSet = sccts.GetTilesShouldbeLoaded(currentCameraState, currentCameraState.screenResolution);

	//TSet<TileNode *> loadingTileSet = tlm.UpdateLoadingTileArray(shouldLoadingTileSet);
	//TSet<TileNode *> unLoadingTileSet = tlm.UpdateUnLoadingTileArray(shouldLoadingTileSet);

	//
	//loadingTileSet = tlm.UpdateLoadingTileArray(shouldLoadingTileSet);
	////��Ҫά��tlm.loadedTileSet
	////tlm.loadedTileSet = loadingTileSet;
	//unLoadingTileSet = tlm.UpdateUnLoadingTileArray(shouldLoadingTileSet);
}

// Called when the game starts or when spawned
void AUE_World_Camera::BeginPlay()
{
	Super::BeginPlay();

	//��ʼ������
	ScrollWheelSpeed = 1;
	//���ڿ���ƽ�Ʒ��ȵı���
	MoveStepFactor = 0.01;

	//�����ԭ������
	this->SphereOrigin = FVector(0,0,0);

	//����İ뾶
	//FVector SphereR = FVector(6378137.0, 6378137.0, 6356752.314245);
	this->SphereR = FVector(5000.0, 5000.0, 5000.0);
	this->MaxSphereR = FMath::Max3(SphereR.X, SphereR.Y, SphereR.Z);

	//����ĳ�ʼ��
	InitializeVirtualGlobe();
}

// Called every frame
void AUE_World_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�����ƽ�Ʊ�ʶΪtrue������CalcCameraMoveDragDirection��ɲ����߼�
	if (IsMoveCameraState)
	{
		CalcCameraMoveDragDirection();
	}

	if (!MovementInput.IsZero())
	{
		MovementInput = MovementInput.GetSafeNormal();
		FVector newLocation = GetActorLocation();
		newLocation += GetActorForwardVector()*MovementInput.X;
		newLocation += GetActorRightVector()*MovementInput.Y;
		//SetActorLocation(newLocation);
	}

}

// Called to bind functionality to input
void AUE_World_Camera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�����ĸ����̰������ڿ��ƾ�ȷ��ƽ�Ʋ���
	/*UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.0f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.0f));
	PlayerInputComponent->BindAxis("MoveForward", this, &AUE_World_Camera::MoveForward);

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, 1.0f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, -1.0f));
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE_World_Camera::MoveRight);*/


	//���ֲ���	
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("ScrollWheelUp", EKeys::MouseWheelAxis, 1.0f));
	PlayerInputComponent->BindAxis("ScrollWheelUp", this, &AUE_World_Camera::OnScrollWheelUpPress);

	//����Ҽ�	
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MouseRightDrag", EKeys::RightMouseButton, 1.0f));
	PlayerInputComponent->BindAxis("MouseRightDrag", this, &AUE_World_Camera::OnMouseRightDrag);

	//����leftShift��	
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("KeyLeftShift", EKeys::LeftShift));
	PlayerInputComponent->BindAction("KeyLeftShift", IE_Pressed, this, &AUE_World_Camera::OnKeyLeftShiftPressed);
	PlayerInputComponent->BindAction("KeyLeftShift", IE_Released, this, &AUE_World_Camera::OnKeyLeftShiftReleased);

	//���λ�ã�X,Y����	
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("OnTurnRotate", EKeys::MouseX, 1.0f));
	PlayerInputComponent->BindAxis("OnTurnRotate", this, &AUE_World_Camera::OnTurnRotate);
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookAtRotate", EKeys::MouseY, 1.0f));
	PlayerInputComponent->BindAxis("LookAtRotate", this, &AUE_World_Camera::OnLookAtRotate);

}

// ���뺯��
void AUE_World_Camera::MoveForward(float AxisValue)
{
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		MoveDirection = FVector::ZeroVector;
		GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorPos.X, CursorPos.Y);
		UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), CursorPos, WorldPos, MoveDirection);
		int32 moveStep = WorldPos.Z*MoveStepFactor*-1;
		MovementInput.X = FMath::Clamp<float>(AxisValue*moveStep, -1.0f*moveStep, 1.0f*moveStep);

		MovementInput = MovementInput.GetSafeNormal();
		FVector newLocation = GetActorLocation();
		newLocation += GetActorForwardVector()*MovementInput.X;
		SetActorLocation(newLocation);
	}
}

void AUE_World_Camera::MoveRight(float AxisValue)
{
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		MoveDirection = FVector::ZeroVector;
		GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorPos.X, CursorPos.Y);
		UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), CursorPos, WorldPos, MoveDirection);
		int32 moveStep = WorldPos.Z*MoveStepFactor*-1;

		MovementInput.Y = FMath::Clamp<float>(AxisValue*moveStep, -1.0f*moveStep, 1.0f*moveStep);

		MovementInput = MovementInput.GetSafeNormal();
		FVector newLocation = GetActorLocation();
		newLocation += GetActorRightVector()*MovementInput.Y;
		SetActorLocation(newLocation);
	}
}





//����shift����İ��²�����Ӧ����
void AUE_World_Camera::OnKeyLeftShiftPressed()
{
	IsBindShiftKey = true;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "key left shift pressed");
}

//����shift�����̧�������Ӧ����
void AUE_World_Camera::OnKeyLeftShiftReleased()
{
	IsBindShiftKey = false;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "key left shift released");
}


//�������ֵ��¼���Ӧ����
//���ڲ��ݵ�ǰ��ҵ������Ұ
//��ϣ��20200410
void AUE_World_Camera::OnScrollWheelUpPress(float axisValue)
{
	MoveDirection = FVector::ZeroVector;
	if (FMath::Abs(axisValue) > KINDA_SMALL_NUMBER)
	{
		GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorPos.X, CursorPos.Y);
		UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), CursorPos, WorldPos, MoveDirection);

		//���ݵ�ǰ��������ʵ�ʸ߶ȣ�ȷ������ƶ���ʵ�ʲ���(ScrollWheelSpeed)��ԭʼ�ƶ�����(axisValue)Ϊ��1��
	    //Ŀǰ�߼�Ϊʵ�ʸ߶ȵ�10%��Ϊ�������ԷŴ��ӽǲ���Ϊ����ÿ�εݽ����и߶ȵ�10%��������º��λ��Ϊԭ���߶ȵ�90%
	    //����ϣ��20191225��

		//Ĭ�Ϲ��ַŴ󣬸��ݾ�������ľ��룬�������ž�������С
		float stepRadio = 0.05;

		/*if (axisValue < 0)
			stepRadio = 1.05;*/
		
		float cameraHeight = FVector::Dist(WorldPos, SphereOrigin) - MaxSphereR;
		ScrollWheelSpeed = cameraHeight * stepRadio;
		OurCamera->AddWorldOffset(MoveDirection*ScrollWheelSpeed*axisValue);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::SanitizeFloat(cameraHeight));
	}
	
}

//����Ҽ�����ק�¼���Ӧ����
//ֻ�����߼��жϣ����õ�ǰ�Ĳ�����ʶ�����������CalcCameraOnTurn���������
//��ϣ20191225
void AUE_World_Camera::OnMouseRightDrag(float axisValue)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::SanitizeFloat(axisValue));
	if (FMath::Abs(axisValue) < KINDA_SMALL_NUMBER)
	{
		CurrentCursorPos = PreviousCursorPos = FVector2D::ZeroVector;
		IsRotateCameraState = false;
		IsMoveCameraState = false;
	}
	else
	{
		//������shift������������ת		
		if (IsBindShiftKey)
		{
			//��ק״̬			
			IsMoveCameraState = false;
			IsRotateCameraState = true;
		}
		//û�а�����Shift��ʱ�򣬿�ʼ��ק		
		else
		{
			IsRotateCameraState = false;
			IsMoveCameraState = true;
		}
	}
}




//�������ƽ�Ʋ����߼���ʵ�ֺ���
//��ϣ20191225
void AUE_World_Camera::CalcCameraMoveDragDirection()
{
	if (FMath::Abs(FVector2D::Distance(PreviousCursorPos, CurrentCursorPos)) < KINDA_SMALL_NUMBER)
	{
		GetWorld()->GetFirstPlayerController()->GetMousePosition(PreviousCursorPos.X, PreviousCursorPos.Y);
	}
	else
	{
		//1.��ȡ�����ǰ��ue����ϵ�е�λ�ã����������ھ�γ�������ϵ����꣬���߶�
		FVector currentCameraLocation = GetActorLocation();
		FVector currentCameraLocationInGlobe = this->sccts.TileCoordinateSystem.FromUE_CoordinateSystem(currentCameraLocation);
		//2.���ݵ�ǰ����ھ�γ�������еĸ߶ȣ����㵥�����ض�Ӧ�ľ�γ�ȳ���
		float degreePerPixel = this->sccts.GetDegreePerPixelInScreen();

		//3.��ȡ��ǰ���λ�ã���֮ǰ��¼�����λ������õ�����Ļ�ϵ����λ��
		GetWorld()->GetFirstPlayerController()->GetMousePosition(CurrentCursorPos.X, CurrentCursorPos.Y);
		FVector2D MoveOffset = CurrentCursorPos - PreviousCursorPos;

		if(MoveOffset.X >10)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::SanitizeFloat(MoveOffset.X));
		
		PreviousCursorPos = CurrentCursorPos;

		//4.������Ļ�ϵ����λ�ƶ�Ӧ�ľ�γ��λ��
		FVector MoveOffsetInGlobe = FVector(MoveOffset.X*degreePerPixel, MoveOffset.Y*degreePerPixel,0);

		//5.�������յ���꾭γ��λ�ã������ݸ�λ�÷���UE�е����꣬�����ƽ�Ƶ���λ��
		currentCameraLocationInGlobe = currentCameraLocationInGlobe.operator+(MoveOffsetInGlobe);
		FVector nextCameraLocation = this->sccts.TileCoordinateSystem.ToUE_CoordinateSystem(currentCameraLocationInGlobe);
		SetActorLocation(nextCameraLocation);

		//6.��������µ�λ�ã��������λ�õ�ue����ԭ��ķ���������ͨ�����������������ƫת�Ƕ�
		FVector nextCameraShootToOriginVector =  SphereOrigin - nextCameraLocation;
		OurCamera->RelativeRotation = nextCameraShootToOriginVector.Rotation();

		//����ʵ�ʸ߶ȣ�����ƽ����
		//��������ÿ��ƽ�Ƶ�λΪ�߶�ֵ��0.01��������ֵ��
		//����������Ҫ����ֵ����Ϊһ�����Թ��û��޸ĵĲ���
		//MoveDirection = FVector::ZeroVector;
		//GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorPos.X, CursorPos.Y);
		//UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), CursorPos, WorldPos, MoveDirection);

		//
		/////*if (axisValue < 0)
		////	stepRadio = 1.05;*/

		//float cameraHeight = FVector::Dist(WorldPos, SphereOrigin) - MaxSphereR;	

		//int32 moveStep = cameraHeight *MoveStepFactor;

		//OurCamera->AddLocalOffset(FVector(0, -MoveOffset.X*moveStep, MoveOffset.Y*moveStep));
	}
}


//Ĭ�ϵ�����Ҽ������ӽ��߼�������mouseX��ֵ,��Z����ת����ƫ���ǵı仯
void AUE_World_Camera::OnTurnRotate(float axisValue)
{
	if (!IsRotateCameraState) return;
	if (IsRotateCameraState&&FMath::Abs(axisValue) > KINDA_SMALL_NUMBER)
	{
		FRotator NewRotationYaw = GetActorRotation();
		NewRotationYaw.Yaw += axisValue;
		SetActorRotation(NewRotationYaw);
	}
}

//Ĭ�ϵ�����Ҽ������ӽ��߼�������mouseY��ֵ����Y����ת���������ǵı仯
//Ŀǰ���ø����ǵı仯��ΧΪ-90 ��-15������ֱ���¿���б�ӵ���15�ȵĽǶ������ķ�Χ����������ʵ����Ҫ�ٵ���
//��ϣ20191225
void AUE_World_Camera::OnLookAtRotate(float axisValue)
{
	if (!IsRotateCameraState) return;
	if (IsRotateCameraState && FMath::Abs(axisValue) > KINDA_SMALL_NUMBER)
	{
		FRotator NewRotationPitch = OurCameraSpringArm->GetComponentRotation();
		NewRotationPitch.Pitch = FMath::Clamp(NewRotationPitch.Pitch + axisValue, -90.0f, -15.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotationPitch);
	}
}

