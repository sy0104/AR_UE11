// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "SelectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API ASelectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASelectPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent();
};
