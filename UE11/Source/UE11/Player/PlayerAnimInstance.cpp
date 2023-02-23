// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mMoveDir = 0.f;

	mAttack = false;
	mAttackEnable = true;
	mAttackIndex = 0;
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

void UPlayerAnimInstance::AnimNotify_Attack()
{
	PrintViewport(1.f, FColor::Red, TEXT("Attack"));

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
