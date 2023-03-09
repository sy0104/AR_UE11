// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayerAnimType : uint8
{
	Ground,		// Idle, Run, Attack
	Jump,		// ���� �ڱ�ġ�°͸�
	Fall,		// �����ؼ� �������ų� ���������� ������
	Death
};

USTRUCT(BlueprintType)
struct FSkillAnimationInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			SkillNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage*	Montage;
};


UCLASS()
class UE11_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

protected:
	// �ִϸ��̼� �������Ʈ���� �а� ���Ⱑ �����ϵ���
	// �̵��ӵ��� 0 ~ 1, ��¥ �̵��ӵ��� �ִ�ӵ��� �Ǿ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpeedRatio;	// ��¥ �̵��ӵ� / �ִ�ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mMoveDir;

	// TArray: STL ���Ͱ��� �������� ��� Ŭ����, �𸮾� ������ �迭 Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mAttackMontageArray;

	// ���� �� �������� �� ȸ�� ����� �������ִ� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage* mFallRecoveryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillAnimationInfo>	mSkillMontageArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mAttackEnable;		// ���� ���� �������� �ƴ���

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mAttackIndex;		// � ���� ��Ÿ�ָ� �����ؾ� �ϴ���

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mAttack;			// �÷��̾ ���� �������� �ƴ��� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPlayerAnimType	mAnimType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mJumpLoop;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mFallRecoveryAdditive;		// ����Ƽ�� ���İ� ������

	int32	mUseSkillNumber;	// � ��ų�� ����ϴ���

public:
	EPlayerAnimType GetPlayerAnimType()	const
	{
		return mAnimType;
	}

	void SetMoveDir(float Dir)
	{
		mMoveDir = Dir;
	}

public:
	virtual void NativeInitializeAnimation();	// ������ �� �ʱ�ȭ �뵵
	virtual void NativeUpdateAnimation(float DeltaSeconds);		// �� �����Ӹ��� �ǽð����� ȣ���

public:
	void Attack();
	void Jump();
	void UseSkill(int32 SkillNumber);

public:
	// ��Ƽ���� �Լ��� void AnimNotify_��Ƽ�����̸�() �� ���·� ������ش�.
	// ����⸸ �ϸ� �ڵ����� ȣ��ȴ�.
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackEnable();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_JumpEnd();

	UFUNCTION()
	void AnimNotify_FallEnd();

	UFUNCTION()
	void AnimNotify_FallRecoveryEnd();

	UFUNCTION()
	void AnimNotify_UseSkill();
};
