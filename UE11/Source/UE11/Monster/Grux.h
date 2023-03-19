// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Grux.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API AGrux : public AMonster
{
	GENERATED_BODY()
	
public:
	AGrux();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void Attack();
};
