// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE11GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API AUE11GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AUE11GameModeBase();

public:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Tick(float DeltaTime)	override;
};
