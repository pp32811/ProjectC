// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataTableStructs.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefencePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeedMul;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedMul;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTimeAcc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCoolTimeLC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCoolTimeRC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCoolTimeSkill1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCoolTimeSkill2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCoolTimeAvoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LC_Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RC_Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Skill1_Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Skill2_Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Avoid_Cost;

};

USTRUCT(BlueprintType)
struct FNowCoolTime : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LC = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RC = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Skill1 = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Skill2 = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Avoid = 0.0f;
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Archor,
	Warrior,
	Mage,
	Count
};

UENUM(BlueprintType)
enum class ESkillKey : uint8
{
	LC,
	RC,
	Skill1,
	Skill2,
	Avoid
};
