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

	// CreateDefaultSubobject : 컴포넌트를 생성한다.
	// 이 함수는 템플릿으로 생성할 타입을 지정하고 인자로 이름을 지정한다.
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// SpringArm을 Mesh의 Child로 지정한다.
	mSpringArm->SetupAttachment(GetMesh());

	// Camera 컴포넌트를 SpringArm 컴포넌트의 자식 컴포넌트로 붙여준다.
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

	// 축매핑 바인드
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraZ"), this, &APlayerCharacter::RotationCameraZ);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraY"), this, &APlayerCharacter::RotationCameraY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoom);

	// 액션매핑 바인드
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::NormalAttack);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill1Key);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("DetachWeapon"), EInputEvent::IE_Pressed, this, &APlayerCharacter::WeaponDetach);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
	return Damage;		// Can Be Damaged가 false라면 0으로 리턴된다.
}

void APlayerCharacter::MoveFront(float Scale)
{
	if (mDeath)
		return;

	mMoveDir = Scale;

	if (Scale == 0.f)
		return;

	// AddMovementInput: 캐릭터 클래스에서 캐릭터 무브먼트 컴포넌트를 이용하여 이동하는 기능을 만들어놓은 함수이다.
	// GetActorForwardVector(): 이 액터의 전방 방향벡터를 얻어온다.
	AddMovementInput(GetActorForwardVector(), Scale);
}

void APlayerCharacter::MoveSide(float Scale)
{
	if (mDeath)
		return;

	if (mMoveDir == 1.f)
	{
		// w키를 누른 상태에서 a, d는 없는 상태.
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);

		// w키를 누른 상태에서 오른쪽으로 이동.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(45.f);

		// w키를 누른 상태에서 왼쪽으로 이동.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-45.f);
	}

	else if (mMoveDir == -1.f)
	{
		// s키를 누른 상태에서 a, d는 없는 상태.
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(180.f);

		// s키를 누른 상태에서 오른쪽으로 이동.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(135.f);

		// s키를 누른 상태에서 왼쪽으로 이동.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-135.f);
	}

	else
	{
		// 아무 키도 누른게 없을 경우
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);

		// 오른쪽으로 이동.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(90.f);

		// 왼쪽으로 이동.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-90.f);
	}

	if (Scale == 0.f)
		return;

	// AddMovementInput : 캐릭터 클래스에서 캐릭터 무브먼트 컴포넌트를
	// GetActorRightVector() : 이 액터의 오른쪽 방향벡터를 얻어온다.
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

	// Cast: 해당 타입일 경우 해당 메모리 주소를 형변환하여 반환하고 아닐 경우 nulptr을 반환한다.
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	float CameraZoomMin = 100.f, CameraZoomMax = 500.f;

	// State가 있을 경우 해당 값을 받아서 사용한다.
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

	// 살아있을 경우 땅 위에 있을 때 점프가 가능하도록 한다.
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

