// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "UE11PlayerState.h"
#include "PlayerAnimInstance.h"
#include "../Skill/SkillProjectile.h"
#include "../Item/WeaponActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CreateDefaultSubobject : ������Ʈ�� �����Ѵ�.
	// �� �Լ��� ���ø����� ������ Ÿ���� �����ϰ� ���ڷ� �̸��� �����Ѵ�.
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// SpringArm�� Mesh�� Child�� �����Ѵ�.
	mSpringArm->SetupAttachment(GetMesh());

	// Camera ������Ʈ�� SpringArm ������Ʈ�� �ڽ� ������Ʈ�� �ٿ��ش�.
	mCamera->SetupAttachment(mSpringArm);

	mSpringArm->TargetArmLength = 500.f;
	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	mSpringArm->SetRelativeRotation(FRotator(-15.0, 90.0, 0.0));

	mMoveDir = 0.f;

	GetCharacterMovement()->JumpZVelocity = 750.f;

	mDeath = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mHitActor = nullptr;

	SetCanBeDamaged(true);

	mWeapon = nullptr;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PrintViewport(1.f, FColor::Red, GetCharacterMovement()->Velocity.ToString());
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ����� ���ε�
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraZ"), this, &APlayerCharacter::RotationCameraZ);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraY"), this, &APlayerCharacter::RotationCameraY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoom);

	// �׼Ǹ��� ���ε�
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::NormalAttack);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill1Key);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("DetachWeapon"), EInputEvent::IE_Pressed, this, &APlayerCharacter::WeaponDetach);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
	return Damage;		// Can Be Damaged�� false��� 0���� ���ϵȴ�.
}

void APlayerCharacter::MoveFront(float Scale)
{
	if (mDeath)
		return;

	mMoveDir = Scale;

	if (Scale == 0.f)
		return;

	// AddMovementInput: ĳ���� Ŭ�������� ĳ���� �����Ʈ ������Ʈ�� �̿��Ͽ� �̵��ϴ� ����� �������� �Լ��̴�.
	// GetActorForwardVector(): �� ������ ���� ���⺤�͸� ���´�.
	AddMovementInput(GetActorForwardVector(), Scale);
}

void APlayerCharacter::MoveSide(float Scale)
{
	if (mDeath)
		return;

	if (mMoveDir == 1.f)
	{
		// wŰ�� ���� ���¿��� a, d�� ���� ����.
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);

		// wŰ�� ���� ���¿��� ���������� �̵�.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(45.f);

		// wŰ�� ���� ���¿��� �������� �̵�.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-45.f);
	}

	else if (mMoveDir == -1.f)
	{
		// sŰ�� ���� ���¿��� a, d�� ���� ����.
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(180.f);

		// sŰ�� ���� ���¿��� ���������� �̵�.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(135.f);

		// sŰ�� ���� ���¿��� �������� �̵�.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-135.f);
	}

	else
	{
		// �ƹ� Ű�� ������ ���� ���
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);

		// ���������� �̵�.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(90.f);

		// �������� �̵�.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-90.f);
	}

	if (Scale == 0.f)
		return;

	// AddMovementInput : ĳ���� Ŭ�������� ĳ���� �����Ʈ ������Ʈ��
	// GetActorRightVector() : �� ������ ������ ���⺤�͸� ���´�.
	AddMovementInput(GetActorRightVector(), Scale);
}

void APlayerCharacter::RotationCameraZ(float Scale)
{
	if (mDeath || Scale == 0.f)
		return;

	AddControllerYawInput(Scale * 180.f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::RotationCameraY(float Scale)
{
	if (Scale == 0.f)
		return;

	mSpringArm->AddRelativeRotation(FRotator(Scale * 180.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
}

void APlayerCharacter::CameraZoom(float Scale)
{
	if (Scale == 0.f)
		return;

	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Scale : %.5f"), Scale));
	mSpringArm->TargetArmLength += Scale * -5.f;

	// Cast: �ش� Ÿ���� ��� �ش� �޸� �ּҸ� ����ȯ�Ͽ� ��ȯ�ϰ� �ƴ� ��� nulptr�� ��ȯ�Ѵ�.
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	float CameraZoomMin = 100.f, CameraZoomMax = 500.f;

	// State�� ���� ��� �ش� ���� �޾Ƽ� ����Ѵ�.
	if (IsValid(State))
	{
		CameraZoomMin = State->GetCameraZoomMin();
		CameraZoomMax = State->GetCameraZoomMax();
	}

	if (mSpringArm->TargetArmLength < CameraZoomMin)
		mSpringArm->TargetArmLength = CameraZoomMin;

	if (mSpringArm->TargetArmLength > CameraZoomMax)
		mSpringArm->TargetArmLength = CameraZoomMax;
}

void APlayerCharacter::NormalAttack()
{
	if (mDeath)
		return;

	mAnimInst->Attack();
}

void APlayerCharacter::JumpKey()
{
	if (mDeath)
		return;

	// ������� ��� �� ���� ���� �� ������ �����ϵ��� �Ѵ�.
	else if (mAnimInst->GetPlayerAnimType() != EPlayerAnimType::Ground)
		return;

	Jump();
	mAnimInst->Jump();
}

void APlayerCharacter::Skill1Key()
{
	Skill1();
}

void APlayerCharacter::WeaponDetach()
{
	if (IsValid(mWeapon))
	{
		mWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		mWeapon = nullptr;
	}
}

void APlayerCharacter::NormalAttackCheck()
{
	
}

void APlayerCharacter::Skill1()
{

}

void APlayerCharacter::UseSkill(int32 SkillNumber)
{

}

