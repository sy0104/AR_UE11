// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


UCLASS()
class UE11_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

protected:
	// �ִϸ��̼� �������Ʈ���� �а� ���Ⱑ �����ϵ���
	// �̵��ӵ��� 0 ~ 1, ��¥ �̵��ӵ��� �ִ�ӵ��� �Ǿ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float mSpeedRatio;	// ��¥ �̵��ӵ� / �ִ�ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mMoveDir;

	// TArray: STL ���Ͱ��� �������� ��� Ŭ����, �𸮾� ������ �迭 Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> mAttackMontageArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool  mAttackEnable;	// ���� ���� �������� �ƴ���

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 mAttackIndex;		// � ���� ��Ÿ�ָ� �����ؾ� �ϴ���

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool  mAttack;

public:
	void SetMoveDir(float Dir) { mMoveDir = Dir; }

public:
	virtual void NativeInitializeAnimation();	// ������ �� �ʱ�ȭ �뵵
	virtual void NativeUpdateAnimation(float DeltaSeconds);		// �� �����Ӹ��� �ǽð����� ȣ���

public:
	void Attack();

public:
	// ��Ƽ���� �Լ��� void AnimNotify_��Ƽ�����̸�() �� ���·� ������ش�.
	// ����⸸ �ص� �ڵ����� ȣ��ȴ�.
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackEnable();

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
