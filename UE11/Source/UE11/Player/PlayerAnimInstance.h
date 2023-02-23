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
	// 애니메이션 블루프린트에서 읽고 쓰기가 가능하도록
	// 이동속도가 0 ~ 1, 진짜 이동속도가 최대속도가 되어야함
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float mSpeedRatio;	// 진짜 이동속도 / 최대속도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mMoveDir;

	// TArray: STL 벡터같은 가변적인 어레이 클래스, 언리얼 엔진의 배열 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> mAttackMontageArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool  mAttackEnable;	// 공격 가능 상태인지 아닌지

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 mAttackIndex;		// 어떤 공격 몽타주를 선택해야 하는지

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool  mAttack;

public:
	void SetMoveDir(float Dir) { mMoveDir = Dir; }

public:
	virtual void NativeInitializeAnimation();	// 생성될 때 초기화 용도
	virtual void NativeUpdateAnimation(float DeltaSeconds);		// 매 프레임마다 실시간으로 호출됨

public:
	void Attack();

public:
	// 노티파이 함수는 void AnimNotify_노티파이이름() 의 형태로 만들어준다.
	// 만들기만 해도 자동으로 호출된다.
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackEnable();

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
