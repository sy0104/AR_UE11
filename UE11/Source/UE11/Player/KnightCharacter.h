// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "KnightCharacter.generated.h"



UCLASS()
class UE11_API AKnightCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AKnightCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void NormalAttackCheck();
	virtual void Skill1();
};
