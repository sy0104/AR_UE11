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
	// TWeakPtr은 참조카운팅을 안한다.
	// 대상이 소멸하면 자동으로 nullptr이 된다.
	// TWeakPtr은 UObject를 못다룬다.
	// TWeakObjectPtr은 UObject를 다룬다.
	TWeakObjectPtr<AActor> mPickActor;
	class ADecal* mMousePick;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent();
	virtual void PostInitializeComponents() override;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	void SpawnMousePick();
};
