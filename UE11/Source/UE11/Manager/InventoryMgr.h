// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryMgr.generated.h"



UCLASS()
class UE11_API UInventoryMgr : public UObject
{
	GENERATED_BODY()
private:
	static	UWorld*		m_CurWorld;
public:
	static UInventoryMgr* GetInst(UWorld* _World);

	UFUNCTION(BlueprintCallable)
	static UInventoryMgr* GetInst(UGameInstance* _GameInst);

public:
	UDataTable*				m_ItemTable;
	TMap<EITEM_ID, FName>	m_RownameFromID;

	int32					m_PrevTime; // 이전 시간


public:
	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool _Show);

	bool IsInventoryOpen();
	void SetItemTable(UDataTable* _Table);
	const FItemDataInfo* GetItemInfo(EITEM_ID _ItemID);


public:
	UInventoryMgr();
	~UInventoryMgr();	
};
