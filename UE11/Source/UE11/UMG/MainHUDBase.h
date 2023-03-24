// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "PlayerInfoBase.h"
#include "InventoryBase.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UMainHUDBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPlayerInfoBase*	m_PlayerInfo;
	UInventoryBase*		m_Inventory;


public:
	// _Ratio 0.f ~ 1.f
	void SetHP(float _Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;

	UInventoryBase* GetInventoryWidget() { return m_Inventory; }
};
