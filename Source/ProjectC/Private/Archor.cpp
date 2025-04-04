// Fill out your copyright notice in the Description page of Project Settings.


#include "Archor.h"
#include "Kismet/GameplayStatics.h"

AArchor::AArchor()
{
}

void AArchor::BeginPlay()
{
	Super::BeginPlay();
}

void AArchor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArchor::Skill1()
{
	UE_LOG(LogTemp, Log, TEXT("Skill1"));
	NowCoolTime.Skill1 = CharacterData.MaxCoolTimeSkill1 / CharacterData.AttackSpeedMul * CharacterData.CoolTimeAcc;

}

void AArchor::LC()
{
	UE_LOG(LogTemp, Log, TEXT("LC_Cool : %f"), NowCoolTime.LC);
	UE_LOG(LogTemp, Log, TEXT("Max_LC_Cool : %f"), CharacterData.MaxCoolTimeLC);

	if (LC_Montage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && NowCoolTime.LC <= 0.0f)
		{
			AnimInstance->Montage_Play(LC_Montage, CharacterData.AttackSpeedMul);
			NowCoolTime.LC = CharacterData.MaxCoolTimeLC / CharacterData.AttackSpeedMul * CharacterData.CoolTimeAcc;
		}
	}
}

void AArchor::RC()
{
	UE_LOG(LogTemp, Log, TEXT("RC"));
}

void AArchor::LC_Spawn()
{
		if (LC_Effect)
		{
			UGameplayStatics::SpawnEmitterAttached(LC_Effect, AimStart);
		}
		if (LC_Bullet)
		{
			GetWorld()->SpawnActor<AActor>(LC_Bullet, AimStart->GetComponentLocation(), AimArrowComponent->GetComponentRotation());
		}
}



