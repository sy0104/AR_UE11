// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "MinionRanger.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API AMinionRanger : public AMonster
{
	GENERATED_BODY()

public:
	AMinionRanger();

protected:
	AActor* mMuzzleParticle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void Attack();
};
