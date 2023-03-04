// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "../Skill/SkillProjectile.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mMoveDir = 0.f;

	mAttack = false;
	mAttackEnable = true;
	mAttackIndex = 0;

	mAnimType = EPlayerAnimType::Ground;

	mGround = true;

	mFallRecoveryAdditive = 0.f;

	mUseSkillNumber = -1;	// ����ϴ� ��ų�� ����
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �÷��̾� ĳ������ �̵��ӵ��� ������ ���� �÷��̾���� ������
	// TryGetPawnOwner(): �� �ִϸ��̼� �ν��Ͻ��� ������ �ִ� ���� ����
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		// �� �����Ӹ��� �ӵ��� ������ �����صΰ� ����
		UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();
		mSpeedRatio = Movement->Velocity.Size() / Movement->MaxWalkSpeed;

		// �� ���� �ִ����� �Ǵ��Ѵ�.
		mGround = Movement->IsMovingOnGround();

		if (!mGround && mAnimType != EPlayerAnimType::Jump)
		{
			mAnimType = EPlayerAnimType::Fall;
		}
	}
}

void UPlayerAnimInstance::Attack()
{
	// ���� �Ұ��� ������ ���� ����Ű�� �ν��� �� ����.
	if (!mAttackEnable)
		return;

	// ������ �ϰ� �ִ� �߿��� �ٽ� �������� �ϱ� ���ؼ� ���� �Ұ��� ���·� ������ش�.
	mAttackEnable = false;

	// Montage_IsPlaying: ���ڷ� �� ��Ÿ�ְ� ����ǰ� �ִ��� �Ǵ����ش�.
	if (!Montage_IsPlaying(mAttackMontageArray[mAttackIndex]))
	{
		// ����� ����Ǵ� ��ġ�� ���� ���Ƿ� ������ �� ����
		Montage_SetPosition(mAttackMontageArray[mAttackIndex], 0.f);	// ���� ��ġ����
		Montage_Play(mAttackMontageArray[mAttackIndex]);	// ��� �ӵ� ���� ����

		mAttackIndex = (mAttackIndex + 1) % mAttackMontageArray.Num();
		mAttack = true;
	}

	else
		mAttackEnable = true;
}

void UPlayerAnimInstance::Jump()
{
	mAnimType = EPlayerAnimType::Jump;
}

void UPlayerAnimInstance::UseSkill(int32 SkillNumber)
{
	int32 Count = mSkillMontageArray.Num();

	for (int32 i = 0; i < Count; ++i)
	{
		if (mSkillMontageArray[i].SkillNumber == SkillNumber)
		{
			mUseSkillNumber = SkillNumber;

			if (!Montage_IsPlaying(mSkillMontageArray[i].Montage))
			{
				Montage_SetPosition(mSkillMontageArray[i].Montage, 0.f);
				Montage_Play(mSkillMontageArray[i].Montage);
			}

			break;
		}
	}
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->NormalAttackCheck();
	}
}

void UPlayerAnimInstance::AnimNotify_AttackEnable()
{
	PrintViewport(1.f, FColor::Red, TEXT("AttackEnable"));
	mAttackEnable = true;
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	PrintViewport(1.f, FColor::Red, TEXT("AttackEnd"));
	mAttackIndex = 0;
	mAttackEnable = true;
	mAttack = false;
}

void UPlayerAnimInstance::AnimNotify_JumpEnd()
{
	mAnimType = EPlayerAnimType::Fall;
}

void UPlayerAnimInstance::AnimNotify_FallEnd()
{
	mAnimType = EPlayerAnimType::Ground;

	if (IsValid(mFallRecoveryMontage))
	{
		mFallRecoveryAdditive = 1.f;

		Montage_SetPosition(mFallRecoveryMontage, 0.f);
		Montage_Play(mFallRecoveryMontage);
	}
}

void UPlayerAnimInstance::AnimNotify_FallRecoveryEnd()
{
	mFallRecoveryAdditive = 0.f;
}

void UPlayerAnimInstance::AnimNotify_UseSkill()
{
	// ��ų�� ����Ѵ�.
	if (mUseSkillNumber == -1)
		return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->UseSkill(mUseSkillNumber);

}
