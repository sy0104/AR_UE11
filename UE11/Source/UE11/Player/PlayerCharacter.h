// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ../�� ���� ������ �ǹ��Ѵ�.
#include "../CharacterBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class UE11_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent* mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = true))
	TArray<FSkillInfo>	mSkillInfoArray;

	float	mMoveDir;
	class UPlayerAnimInstance* mAnimInst;
	bool	mDeath;

	AActor* mHitActor;

	class AWeaponActor* mWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	virtual void UnPossessed();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	// ����� ���ε� �Լ�
	// ����Ÿ�� : void ���ڷ� float Ÿ�� �ϳ��� ������ �ȴ�.
	void MoveFront(float Scale);
	void MoveSide(float Scale);
	void RotationCameraZ(float Scale);
	void RotationCameraY(float Scale);
	void CameraZoom(float Scale);

	// �׼Ǹ��� ���ε� �Լ�
	// ����Ÿ�� : void ���� ���� �����̴�.
	void NormalAttack();
	void JumpKey();
	void Skill1Key();
	void WeaponDetach();
	void MouseMove();
	void Skill2Key();
	void InventoryOn();

public:
	virtual void NormalAttackCheck();
	virtual void Skill1();
	virtual void Skill2();


	virtual void UseSkill(int32 SkillNumber);

	void FootStep(bool Left);


public:
	virtual void SavePlayer();
};
