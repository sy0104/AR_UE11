// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UMonsterHPBase : public UUserWidget
{
	GENERATED_BODY()
private:
	UProgressBar*	m_HPBar;
	float			m_InitRatio;

public:
	void SetInitHP(float _Ratio)
	{
		m_InitRatio = _Ratio;
	}

	// BeginPlay 에서 호출하면 안됨
	void SetHP(float _Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
};
