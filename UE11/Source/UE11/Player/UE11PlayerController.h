// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "UE11PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API AUE11PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AUE11PlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent();
	virtual void PostInitializeComponents() override;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
};
