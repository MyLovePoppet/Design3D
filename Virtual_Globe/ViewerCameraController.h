// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerInput.h"

#include "CoordinateSystem.h"
#include "TileLoadManager.h"
#include "SceneCulling.h"

#include "ViewerCameraController.generated.h"

UCLASS()
class VIRTUAL_GLOBE_API AViewerCameraController : public APawn
{
	GENERATED_BODY()

public:

	CameraState UE_CameraState;

	UCameraComponent* OurCamera;

	FTransform WorldToUETransformation;

	// Sets default values for this pawn's properties
	AViewerCameraController();

	CameraState GetUECameraState();
	void SetUECameraState(CameraState Camera);

	CoordinateSystem* geoCoordinateSystem;
	float degreePerPixelInScreen;
	float meterPerPixelInScreen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* OurCameraSpringArm;

	//�����ԭ������
	FVector SphereOrigin;

	//����İ뾶
	FVector SphereR;

	float MaxSphereR;

	//�������
	FVector2D MovementInput;
	FVector2D CameraInput;
	float MoveStepFactor;
	bool bZoomingIn;
	//�����ֵĹ�������
	float ScrollWheelSpeed;

	//��ǰ�������Ļ�еĶ�ά����λ��
	FVector2D CursorPos;
	//һ�����ڼ�¼��ǰ�������Ļ�ж�ά����λ�õĵ�
	FVector2D CurrentCursorPos;
	////һ�����ڼ�¼֮ǰ�������Ļ�ж�ά����λ�õĵ�
	FVector2D PreviousCursorPos;

	//��¼��ǰ����ӽ��Ƿ���ת��״̬��ʶ
	bool IsRotateCameraState;
	//��¼��ǰ����ӽ��Ƿ�ƽ�Ƶ�״̬��ʶ
	bool IsMoveCameraState;

	//��¼��ǰ�û��Ƿ���shift�����ĵ�״̬��ʶ
	bool IsBindShiftKey;


	//һ��������ά����λ��
	FVector WorldPos;
	//һ��������άƫת����
	FVector MoveDirection;

	//���뺯��
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ZoomIn();
	void ZoomOut();


	void OnScrollWheelUpPress(float axisValue);
	void OnMouseRightDrag(float axisValue);
	void CalcCameraMoveDragDirection();
	void OnKeyLeftShiftPressed();
	void OnKeyLeftShiftReleased();
	void OnTurnRotate(float axisValue);
	void OnLookAtRotate(float axisValue);

	void InitializeVirtualGlobe();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	CameraState WorldToUETransform(CameraState Camera);
	CameraState UEToWorldTransform(CameraState Camera);

};
