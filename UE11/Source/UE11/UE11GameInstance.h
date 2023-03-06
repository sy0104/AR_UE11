// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "UE11GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UUE11GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UUE11GameInstance();
	~UUE11GameInstance();

private:
	UDataTable*		mMonsterTable;

public:
	virtual void Init();

public:
	const FMonsterTableInfo* FindMonsterTable(const FName& Name);
};
