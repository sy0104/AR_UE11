// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

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
	float	mHitAdditive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage* mHitMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mLoopFalse;

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

	UFUNCTION()
	void AnimNotify_Skill1();

	UFUNCTION()
	void AnimNotify_Skill2();

	UFUNCTION()
	void AnimNotify_Skill3();

	UFUNCTION()		
	void AnimNotify_SkillEnd();
};
