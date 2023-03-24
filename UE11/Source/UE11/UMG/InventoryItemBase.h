// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Image.h>
#include <Components\TextBlock.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UInventoryItemBase : public UUserWidget
{
	GENERATED_BODY()
private:
	UImage*		m_IconImg;
	UTextBlock* m_ItemNameTxt;
	UTextBlock* m_CountTxt;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DT) override;	

public:
	UFUNCTION(BlueprintCallable)
	void InitFromData(UObject* _Data);
};
