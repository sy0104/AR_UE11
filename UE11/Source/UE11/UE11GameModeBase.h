// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Blueprint\UserWidget.h>

#include "UMG\MainHUDBase.h"
#include "Player/PlayerCharacter.h"

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
private:	
	TSubclassOf<UUserWidget>	m_MainHUDClass;
	UMainHUDBase*				m_MainHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<APlayerCharacter>>	mPlayerClassArray;

	class UUE11SaveGame* mSaveGame;

public:
	class UUE11SaveGame* GetSaveGame()
	{
		return mSaveGame;
	}

public:
	AUE11GameModeBase();

public:
	virtual void BeginPlay()	override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Tick(float DeltaTime)	override;

public:
	UMainHUDBase* GetMainHUD() { return m_MainHUD;}

private:
	void SaveGame();
};
