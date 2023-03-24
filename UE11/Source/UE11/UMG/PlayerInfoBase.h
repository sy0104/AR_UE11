// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UPlayerInfoBase : public UUserWidget
{
	GENERATED_BODY()
private:
	UProgressBar*	m_HPBar;
	UProgressBar*	m_MPBar;

public:
	void SetHP(float _Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
};
