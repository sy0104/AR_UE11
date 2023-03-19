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
	Death,
	KnockBack,
	Stun,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	Skill5,
	Skill6,
	Skill7,
	Skill8,
	Skill9,
	Skill10,
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EMonsterAnimType	mAnimType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				mHitAdditive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage*		mHitMontage;

public:
	void ChangeAnim(EMonsterAnimType Anim)
	{
		mAnimType = Anim;
	}

	void Hit()
	{
		if (!IsValid(mHitMontage))
			return;

		mHitAdditive = 1.f;

		if (!Montage_IsPlaying(mHitMontage))
		{
			Montage_SetPosition(mHitMontage, 0.f);
			Montage_Play(mHitMontage);
		}
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	// 노티파이 함수는 void AnimNotify_노티파이이름() 의 형태로 만들어준다.
	UFUNCTION()
	void AnimNotify_DeathEnd();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
