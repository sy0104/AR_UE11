// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include <Components\Button.h>
#include "Blueprint/UserWidget.h"
#include "SelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API USelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UButton* mStartBtn;

public:
	void SetStartButtonVisibility(bool Visible)
	{
		if (Visible)
			mStartBtn->SetVisibility(ESlateVisibility::Visible);

		else
			mStartBtn->SetVisibility(ESlateVisibility::Collapsed);
	}

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime);

public:
	UFUNCTION()
	void OnStartBtnClicked();
};
