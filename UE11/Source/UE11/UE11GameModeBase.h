// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Blueprint\UserWidget.h>
#include "UMG\MainHUDBase.h"

#include "GameInfo.h"
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

private:
	TSubclassOf<UUserWidget>	mMainHUDClass;
	UMainHUDBase*				mMainHUD;		// UsetWidget으로 만들어진 진짜 객체를 가리키는 포인터

public:
	virtual void BeginPlay()	override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Tick(float DeltaTime)	override;

public:
	UMainHUDBase* GetMainHUD()
	{
		return mMainHUD;
	}
};
