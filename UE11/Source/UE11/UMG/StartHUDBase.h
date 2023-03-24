// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Button.h>
#include "../GameInfo.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UStartHUDBase : public UUserWidget
{
	GENERATED_BODY()
private:
	UButton*	m_StartBtn;
	UButton*	m_EndBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName		m_NextLevelName;

	TMap<FString, UWidgetAnimation*> m_mapWidgetAnim;
	
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
