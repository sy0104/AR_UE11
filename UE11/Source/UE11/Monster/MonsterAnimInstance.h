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
	virtual void NativeInitializeAnimation();	// ������ �� �ʱ�ȭ �뵵
	virtual void NativeUpdateAnimation(float DeltaSeconds);		// �� �����Ӹ��� �ǽð����� ȣ���

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
