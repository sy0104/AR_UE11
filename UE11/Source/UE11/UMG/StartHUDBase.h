// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "StartHUDBase.generated.h"


UCLASS()
class UE11_API UStartHUDBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UButton*	mStartButton;
	UButton*	mEndButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName		mNextLevelName;

	TMap<FString, UWidgetAnimation*>	mMapWidgetAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartBtnHoverAnim;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime);

public:
	UFUNCTION()
	void OnStartBtnClicked();

	UFUNCTION()
	void OnStartBtnHover();

	UFUNCTION()
	void OnStartBtnUnhover();

};
