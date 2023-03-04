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

	mUseSkillNumber = -1;	// 사용하는 스킬이 없음
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 플레이어 캐릭터의 이동속도를 얻어오기 위해 플레이어부터 가져옴
	// TryGetPawnOwner(): 이 애니메이션 인스턴스를 가지고 있는 폰을 얻어옴
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		// 매 프레임마다 속도의 비율을 저장해두고 있음
		UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();
		mSpeedRatio = Movement->Velocity.Size() / Movement->MaxWalkSpeed;

		// 땅 위에 있는지를 판단한다.
		mGround = Movement->IsMovingOnGround();

		if (!mGround && mAnimType != EPlayerAnimType::Jump)
		{
			mAnimType = EPlayerAnimType::Fall;
		}
	}
}

void UPlayerAnimInstance::Attack()
{
	// 공격 불가능 상태일 때는 공격키를 인식할 수 없다.
	if (!mAttackEnable)
		return;

	// 공격을 하고 있는 중에는 다시 못들어오게 하기 위해서 공격 불가능 상태로 만들어준다.
	mAttackEnable = false;

	// Montage_IsPlaying: 인자로 들어간 몽타주가 재생되고 있는지 판단해준다.
	if (!Montage_IsPlaying(mAttackMontageArray[mAttackIndex]))
	{
		// 모션이 재생되는 위치를 내가 임의로 지정할 수 있음
		Montage_SetPosition(mAttackMontageArray[mAttackIndex], 0.f);	// 시작 위치부터
		Montage_Play(mAttackMontageArray[mAttackIndex]);	// 재생 속도 설정 가능

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
	// 스킬을 사용한다.
	if (mUseSkillNumber == -1)
		return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->UseSkill(mUseSkillNumber);

}
