// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	//RootComponent = SkeletalMeshComponent;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 355.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AimArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("AimArrowComponent"));
	AimArrowComponent->SetupAttachment(RootComponent); // 초기화 및 부모 컴포넌트에 부착

	AimStart = CreateDefaultSubobject<USceneComponent>(TEXT("AimStart"));
	AimStart->SetupAttachment(RootComponent); // 초기화 및 부모 컴포넌트에 부착
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateAimWidget();
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 데이터 테이블에서 행을 가져와서 CharacterData의 값을 변경합니다.
	if (CharacterDataTable)
	{
		static const FString ContextString(TEXT("Character Data Context"));
		FCharacterData* Row = CharacterDataTable->FindRow<FCharacterData>(RowName, ContextString, true);

		if (Row)
		{
			CharacterData.MaxHp = Row->MaxHp;
			CharacterData.MaxMp = Row->MaxMp;
			CharacterData.AttackPower = Row->AttackPower;
			CharacterData.DefencePower = Row->DefencePower;
			CharacterData.MoveSpeedMul = Row->MoveSpeedMul;
			CharacterData.AttackSpeedMul = Row->AttackSpeedMul;
			CharacterData.CoolTimeAcc = Row->CoolTimeAcc;
			CharacterData.MaxCoolTimeLC = Row->MaxCoolTimeLC;
			CharacterData.MaxCoolTimeRC = Row->MaxCoolTimeRC;
			CharacterData.MaxCoolTimeSkill1 = Row->MaxCoolTimeSkill1;
			CharacterData.MaxCoolTimeSkill2 = Row->MaxCoolTimeSkill2;
			CharacterData.MaxCoolTimeAvoid = Row->MaxCoolTimeAvoid;
			CharacterData.LC_Cost = Row->LC_Cost;
			CharacterData.RC_Cost = Row->RC_Cost;
			CharacterData.Skill1_Cost = Row->Skill1_Cost;
			CharacterData.Skill2_Cost = Row->Skill2_Cost;
			CharacterData.Avoid_Cost = Row->Avoid_Cost;
		}
	}
}

void ABaseCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(AvoidAction, ETriggerEvent::Started, this, &ABaseCharacter::Avoid);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABaseCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Aim);

		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &ABaseCharacter::LC);

		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &ABaseCharacter::RC);

		EnhancedInputComponent->BindAction(Skill1Action, ETriggerEvent::Started, this, &ABaseCharacter::Skill1);

		EnhancedInputComponent->BindAction(Skill2Action, ETriggerEvent::Started, this, &ABaseCharacter::Skill2);
	}
	else
	{
		//UE_LOG(Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	inputX = MovementVector.X;
	inputY = MovementVector.Y;

	//UE_LOG(LogTemp, Log, TEXT("MovementVector.X: %f"), MovementVector.X);
	//UE_LOG(LogTemp, Log, TEXT("MovementVector.Y: %f"), MovementVector.Y);

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		/*FRotator NewRotation = Controller->GetControlRotation();
		float NewPitch = NewRotation.Pitch - LookAxisVector.Y;
		NewPitch = FMath::Clamp(NewPitch, 320.0f, 359.0f);
		NewPitch = FMath::Clamp(NewPitch, -20.0f, 40.0f);

		NewRotation = FRotator(NewPitch, NewRotation.Yaw, NewRotation.Roll);
		NewRotation.Normalize();

		Controller->SetControlRotation(NewRotation);*/
	}
}

void ABaseCharacter::Aim(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//if (Controller != nullptr)
	//{
	//	FRotator ArrowRot = AimArrowComponent->GetComponentRotation();
	//	FRotator ControllerRot = Controller->GetControlRotation();

	//	float NewPitch = ArrowRot.Pitch - LookAxisVector.Y*5;
	//	NewPitch = FMath::Clamp(NewPitch, -20.0f, 60.0f);

	//	AimArrowComponent->SetWorldRotation(FRotator(NewPitch, ControllerRot.Yaw, ControllerRot.Roll));

	//	//UE_LOG(LogTemp, Log, TEXT("ArrowRotation.pitch: %f"), AimArrowComponent->GetComponentRotation().Pitch)
	//}
	FRotator ControllerRot = Controller->GetControlRotation();
	float NewPitch = ControllerRot.Pitch * 2;

	AimArrowComponent->SetWorldRotation(FRotator(NewPitch, ControllerRot.Yaw, ControllerRot.Roll));

}
// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetAimScreeenSpace();
	CoolDown(DeltaTime);
}

void ABaseCharacter::Skill1()
{
	UE_LOG(LogTemp, Log, TEXT("Skill1"));
}

void ABaseCharacter::Skill2()
{
	UE_LOG(LogTemp, Log, TEXT("Skill2"));
}

void ABaseCharacter::LC()
{
	UE_LOG(LogTemp, Log, TEXT("LC"));
}

void ABaseCharacter::RC()
{
	UE_LOG(LogTemp, Log, TEXT("RC"));
}

void ABaseCharacter::Avoid()
{
	UE_LOG(LogTemp, Log, TEXT("Avoid"));
}

void ABaseCharacter::CreateAimWidget()
{
	AimWidget = CreateWidget<UUserWidget>(GetWorld(), AimWidgetClass);

	if (AimWidget)
	{
		AimWidget->AddToViewport();
		AimWidget->SetDesiredSizeInViewport(FVector2D(AimSize, AimSize));
	}
}

void ABaseCharacter::SetAimScreeenSpace()
{
	if (AimWidget)
	{
		FVector2D ScreenPosition;

		FVector AimLoc =  AimStart->GetComponentLocation() + AimArrowComponent->GetForwardVector() * 500.0f;
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, AimLoc, ScreenPosition,false);

		int32 ScreenWidth, ScreenHeight;
		PC->GetViewportSize(ScreenWidth, ScreenHeight);

		//float MagicMul = 1080.f / ScreenHeight >= 2 ? ScreenHeight / 1080.0f : 1080.0f / ScreenHeight;
		float MagicMul = 1080.f / ScreenHeight;
		ScreenPosition.X = ScreenWidth * MagicMul / 2.0f - AimSize / 2.2f;

		//ScreenPosition.X -= AimSize / 2;
		ScreenPosition.Y -= AimSize / 2;


		//UE_LOG(LogTemp, Log, TEXT("Y : %f"), 1080.0f / ScreenHeight);
		//UE_LOG(LogTemp, Log, TEXT("x : %f y : %f"), ScreenPosition.X, ScreenPosition.Y);

		AimWidget->SetPositionInViewport(ScreenPosition,false);
	}
}

void ABaseCharacter::CoolDown(float DeltaTime)
{
	if (NowCoolTime.LC > 0.0f)
	{
		NowCoolTime.LC -= DeltaTime;
		NowCoolTime.LC = FMath::Clamp(NowCoolTime.LC, 0.0f, CharacterData.MaxCoolTimeLC);
	}
	if (NowCoolTime.RC > 0.0f)
	{
		NowCoolTime.RC -= DeltaTime;
		NowCoolTime.RC = FMath::Clamp(NowCoolTime.RC, 0.0f, CharacterData.MaxCoolTimeRC);
	}
	if (NowCoolTime.Skill1 > 0.0f)
	{
		NowCoolTime.Skill1 -= DeltaTime;
		NowCoolTime.Skill1 = FMath::Clamp(NowCoolTime.Skill1, 0.0f, CharacterData.MaxCoolTimeSkill1);
	}
	if (NowCoolTime.Skill2 > 0.0f)
	{
		NowCoolTime.Skill2 -= DeltaTime;
		NowCoolTime.Skill2 = FMath::Clamp(NowCoolTime.Skill2, 0.0f, CharacterData.MaxCoolTimeSkill1);
	}
	if (NowCoolTime.Avoid > 0.0f)
	{
		NowCoolTime.Avoid -= DeltaTime;
		NowCoolTime.Avoid = FMath::Clamp(NowCoolTime.Avoid, 0.0f, CharacterData.MaxCoolTimeSkill1);
	}
	
}
