// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API AStartGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	TSubclassOf<UUserWidget>	m_StartHUDClass;
	UUserWidget*				m_StartHUD;

public:
	AStartGameModeBase();

public:
	virtual void BeginPlay()	override;
	virtual void Tick(float DeltaTime)	override;
};
