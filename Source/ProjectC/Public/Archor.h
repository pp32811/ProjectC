// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "DataTableStructs.h"
#include "Engine/DataTable.h"
#include "Particles/ParticleSystem.h"
#include "Archor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTC_API AArchor : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArchor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Skill1() override;
	void LC() override;
	void RC() override;

	UFUNCTION(BlueprintCallable)
	void LC_Spawn();



private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LC_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* LC_Effect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> LC_Bullet;
};
