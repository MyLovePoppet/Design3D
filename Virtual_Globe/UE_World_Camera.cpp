// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_World_Camera.h"

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

// Called when the game starts or when spawned
void AUE_World_Camera::BeginPlay()
{
	Super::BeginPlay();

	//��ʼ������
	ScrollWheelSpeed = 1;
	//���ڿ���ƽ�Ʒ��ȵı���
	MoveStepFactor = 0.01;
	
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
		SetActorLocation(newLocation);
	}

}

// Called to bind functionality to input
void AUE_World_Camera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�����ĸ����̰������ڿ��ƾ�ȷ��ƽ�Ʋ���
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.0f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.0f));
	//PlayerInputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);

	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, 1.0f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, -1.0f));
	//PlayerInputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);


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
//��ϣ��20191225
void AUE_World_Camera::OnScrollWheelUpPress(float axisValue)
{
	MoveDirection = FVector::ZeroVector;
	if (FMath::Abs(axisValue) > KINDA_SMALL_NUMBER)
	{
		GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorPos.X, CursorPos.Y);
		UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), CursorPos, WorldPos, MoveDirection);
	}
	//���ݵ�ǰ��������ʵ�ʸ߶ȣ�ȷ������ƶ���ʵ�ʲ���(ScrollWheelSpeed)��ԭʼ�ƶ�����(axisValue)Ϊ��1��
	//Ŀǰ�߼�Ϊʵ�ʸ߶ȵ�10%��Ϊ�������ԷŴ��ӽǲ���Ϊ����ÿ�εݽ����и߶ȵ�10%��������º��λ��Ϊԭ���߶ȵ�90%
	//����axisVaule��֪Ϊ��ÿ��������λ����˴˴�ȡ0.05����ϣ��20191225��
	ScrollWheelSpeed = WorldPos.Z*0.05;
	OurCamera->AddWorldOffset(MoveDirection*ScrollWheelSpeed*axisValue);
}

//����Ҽ�����ק�¼���Ӧ����
//ֻ�����߼��жϣ����õ�ǰ�Ĳ�����ʶ�����������CalcCameraOnTurn���������
//��ϣ20191225
void AUE_World_Camera::OnMouseRightDrag(float axisValue)
{
	if (FMath::Abs(axisValue) < KINDA_SMALL_NUMBER)
	{
		CurrentCursorPos = PreviousCursorPos = FVector2D::ZeroVector;
		IsRotateCameraState = false;
		IsMoveCameraState = false;
	}
	else
	{
		//������shift������ʼ��ק		
		if (IsBindShiftKey)
		{
			//��ק״̬			
			IsMoveCameraState = true;
			IsRotateCameraState = false;
		}
		//û�а�����Shift��ʱ����������ת		
		else
		{
			IsRotateCameraState = true;
			IsMoveCameraState = false;
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
		GetWorld()->GetFirstPlayerController()->GetMousePosition(CurrentCursorPos.X, CurrentCursorPos.Y);
		FVector2D MoveOffset = CurrentCursorPos - PreviousCursorPos;
		PreviousCursorPos = CurrentCursorPos;

		//����ʵ�ʸ߶ȣ�����ƽ����
		//��������ÿ��ƽ�Ƶ�λΪ�߶�ֵ��0.01��������ֵ��
		//����������Ҫ����ֵ����Ϊһ�����Թ��û��޸ĵĲ���
		MoveDirection = FVector::ZeroVector;
		GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorPos.X, CursorPos.Y);
		UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0), CursorPos, WorldPos, MoveDirection);
		int32 moveStep = WorldPos.Z*MoveStepFactor;

		OurCamera->AddLocalOffset(FVector(0, -MoveOffset.X*moveStep, MoveOffset.Y*moveStep));
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

