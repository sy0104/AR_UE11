// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UInventoryManager : public UObject
{
	GENERATED_BODY()
	
public:
	UInventoryManager();
	~UInventoryManager();

private:
	static UWorld*	mCurWorld;

public:
	// 아이템 테이블은 게임 인스턴스가 생성될 때 만들어지고 있다.
	// 게임 인스턴스가 만들어질 때는 월드가 존재하지 않는다.
	// GetInstance()를 두가지 버전으로 만든다.
	static UInventoryManager* GetInstance(UWorld* World);

	UFUNCTION(BlueprintCallable)
	static UInventoryManager* GetInstance(UGameInstance* GameInstance);

public:
	UDataTable*				mItemTable;
	TMap<EItemID, FName>	mRownameFromID;

	int32					mPrevTime;

public:
	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool Show);

	bool IsInventoryOpen();

public:
	const FItemDataInfo* GetItemInfo(EItemID ItemID);
	void SetItemTable(UDataTable* Table);
	
};
