// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EMonsterAnimType : uint8
{
	Idle,
	Run,
	CombatRun,
	Attack,
	Death
};

/**
 * 
 */
UCLASS()
class UE11_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMonsterAnimInstance();

public:
	virtual void NativeInitializeAnimation();	// 생성될 때 초기화 용도
	virtual void NativeUpdateAnimation(float DeltaSeconds);		// 매 프레임마다 실시간으로 호출됨

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EMonsterAnimType	mAnimType;

public:
	void ChangeAnim(EMonsterAnimType Anim)
	{
		mAnimType = Anim;
	}


public:
	UFUNCTION()
	void AnimNotify_DeathEnd();
};
