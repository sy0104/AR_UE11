// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components/ListView.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()
private:
	UListView*	m_ListView;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;	
};
