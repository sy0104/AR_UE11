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
	// ������ ���̺��� ���� �ν��Ͻ��� ������ �� ��������� �ִ�.
	// ���� �ν��Ͻ��� ������� ���� ���尡 �������� �ʴ´�.
	// GetInstance()�� �ΰ��� �������� �����.
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
