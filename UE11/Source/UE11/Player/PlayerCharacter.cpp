// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "UE11PlayerState.h"
#include "PlayerAnimInstance.h"
#include "../Skill/SkillProjectile.h"
#include "../Item/WeaponActor.h"
#include "UE11PlayerController.h"
#include "../Material/UE11PhysicalMaterial.h"
#include "../Particle/ParticleCascade.h"
#include "../UE11SaveGame.h"
#include "../UE11GameModeBase.h"

#include "../Manager/InventoryManager.h"

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
	//JumpMaxCount = 10;
	//JumpMaxHoldTime = 3.f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mHitActor = nullptr;

	SetCanBeDamaged(true);

	mWeapon = nullptr;

	//GetMesh()->SetRenderCustomDepth(true);
	//GetMesh()->SetCustomDepthStencilValue(12);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	// ���� ���� �� �κ��丮�� ���ش�. ���� ���嵵 �������ش�.
	UInventoryManager::GetInstance(GetWorld())->ShowInventory(false);

	UUE11SaveGame* LoadGame = Cast<UUE11SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Save"), 0));

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");

	TSharedPtr<FArchive> Reader = MakeShareable(IFileManager::Get().CreateFileReader(*FullPath));

	if (Reader.IsValid())
	{
		*Reader.Get() << State->mPlayerInfo.Name;
		*Reader.Get() << State->mPlayerInfo.Job;
		*Reader.Get() << State->mPlayerInfo.AttackPoint;
		*Reader.Get() << State->mPlayerInfo.ArmorPoint;
		*Reader.Get() << State->mPlayerInfo.HP;
		*Reader.Get() << State->mPlayerInfo.HPMax;
		*Reader.Get() << State->mPlayerInfo.MP;
		*Reader.Get() << State->mPlayerInfo.MPMax;
		*Reader.Get() << State->mPlayerInfo.Level;
		*Reader.Get() << State->mPlayerInfo.Exp;
		*Reader.Get() << State->mPlayerInfo.Gold;
		*Reader.Get() << State->mPlayerInfo.MoveSpeed;
		*Reader.Get() << State->mPlayerInfo.AttackDistance;

		*Reader.Get() << State->mCameraZoomMin;
		*Reader.Get() << State->mCameraZoomMax;
	}

	else
	{
		// �� ������ �������� �⺻ ������ �ٸ��� ������ ������ �ʱ� �����͸� ���������̺�� �����ϰ� ������ ���̺�κ��� �о���� �Ѵ�.
		State->mPlayerInfo.AttackPoint = 80;
		State->mPlayerInfo.ArmorPoint = 60;
		State->mPlayerInfo.HP = 1000;
		State->mPlayerInfo.HPMax = 1000;
		State->mPlayerInfo.MP = 100;
		State->mPlayerInfo.MPMax = 100;

		State->mPlayerInfo.Level = 1;
		State->mPlayerInfo.Gold = 10000;
		State->mPlayerInfo.Exp = 0;
		State->mPlayerInfo.MoveSpeed = 1000.f;
		State->mPlayerInfo.AttackDistance = 200.f;
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	LOG(TEXT("EndPlay"));

	switch (EndPlayReason)
	{
	case EEndPlayReason::Destroyed:
		LOG(TEXT("Destroyed"));
		break;
	case EEndPlayReason::LevelTransition:
		LOG(TEXT("Level Transition"));
		break;
	case EEndPlayReason::EndPlayInEditor:
		LOG(TEXT("EndPlayInEditor"));
		break;
	case EEndPlayReason::RemovedFromWorld:
		LOG(TEXT("RemovedFromWorld"));
		break;
	case EEndPlayReason::Quit:
		LOG(TEXT("Quit"));
		break;
	}
}

void APlayerCharacter::UnPossessed()
{
	Super::UnPossessed();

	LOG(TEXT("UnPossessed"));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���ε� ����
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraZ"), this, &APlayerCharacter::RotationCameraZ);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraY"), this, &APlayerCharacter::RotationCameraY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoom);

	// �׼� ����
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::NormalAttack);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill1Key);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("DetachWeapon"), EInputEvent::IE_Pressed, this, &APlayerCharacter::WeaponDetach);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("MouseMove"), EInputEvent::IE_Pressed, this, &APlayerCharacter::MouseMove);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Skill2"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill2Key);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Skill2"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill2Key);

	FInputActionBinding& toggle = PlayerInputComponent->BindAction<APlayerCharacter>(
		TEXT("Inventory"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InventoryOn);

	toggle.bConsumeInput = false;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("Dmg : %.5f"), DamageAmount));

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	if (IsValid(State))
	{
		float Dmg = DamageAmount - State->mPlayerInfo.ArmorPoint;

		Dmg = Dmg < 1.f ? 1.f : Dmg;

		State->mPlayerInfo.HP -= (int32)Dmg;
	}

	return Damage;
}

void APlayerCharacter::MoveFront(float Scale)
{
	if (mDeath)
		return;

	mMoveDir = Scale;

	if (Scale == 0.f)
		return;

	// AddMovementInput : ĳ���� Ŭ�������� ĳ���� �����Ʈ ������Ʈ�� �̿��Ͽ� �̵��ϴ� ����� �������� �Լ��̴�.
	// GetActorForwardVector() : �� ������ ���� ���⺤�͸� ���´�.
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

	// AddMovementInput : ĳ���� Ŭ�������� ĳ���� �����Ʈ ������Ʈ�� �̿��Ͽ� �̵��ϴ� ����� �������� �Լ��̴�.
	// GetActorRightVector() : �� ������ ������ ���⺤�͸� ���´�.
	AddMovementInput(GetActorRightVector(), Scale);
}

void APlayerCharacter::RotationCameraZ(float Scale)
{
	if (mDeath)
		return;

	if (Scale == 0.f)
		return;

	//mSpringArm->AddRelativeRotation(FRotator(0.f, Scale * 180.f * GetWorld()->GetDeltaSeconds(), 0.f));
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

	// Cast : �ش� Ÿ���� ��� �ش� �޸� �ּҸ� ����ȯ�Ͽ� ��ȯ�ϰ� �ƴ� ��� nullptr�� ��ȯ�Ѵ�.
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	float	CameraZoomMin = 100.f, CameraZoomMax = 500.f;

	// State�� ���� ��� �ش� ���� �޾Ƽ� ����Ѵ�.
	if (IsValid(State))
	{
		CameraZoomMin = State->GetCameraZoomMin();
		CameraZoomMax = State->GetCameraZoomMax();
	}

	if (mSpringArm->TargetArmLength < CameraZoomMin)
		mSpringArm->TargetArmLength = CameraZoomMin;

	else if (mSpringArm->TargetArmLength > CameraZoomMax)
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

	// ������� ��� �� ���� ������ ������ �����ϵ��� �Ѵ�.
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
		PrintViewport(1.f, FColor::Red, TEXT("detach"));
		mWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		mWeapon = nullptr;
	}
}

void APlayerCharacter::MouseMove()
{
	AUE11PlayerController* PlayerController = Cast<AUE11PlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		PlayerController->SpawnMousePick();
	}
}

void APlayerCharacter::Skill2Key()
{
	Skill2();
}

void APlayerCharacter::InventoryOn()
{
	// InventoryManager�� ����, �κ��丮�� ���������� ���ش�.
	UInventoryManager::GetInstance(GetWorld())->ShowInventory(true);
}

void APlayerCharacter::NormalAttackCheck()
{
}

void APlayerCharacter::Skill1()
{
}

void APlayerCharacter::Skill2()
{
}

void APlayerCharacter::UseSkill(int32 SkillNumber)
{
}

void APlayerCharacter::FootStep(bool Left)
{
	FVector	LineStart;

	if (Left)
		LineStart = GetMesh()->GetSocketLocation(TEXT("Foot_L"));

	else
		LineStart = GetMesh()->GetSocketLocation(TEXT("Foot_R"));

	FVector	LineEnd = LineStart + FVector::DownVector * 50.f;

	FCollisionQueryParams	param(NAME_None, false, this);

	// �浹 ����� �������� ���� ���θ� ������ ������ �����Ѵ�.
	param.bReturnPhysicalMaterial = true;

	FHitResult	result;
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, LineStart, LineEnd, ECollisionChannel::ECC_GameTraceChannel9, param);

	if (Hit)
	{
		UUE11PhysicalMaterial* Phys = Cast<UUE11PhysicalMaterial>(result.PhysMaterial);

		if (IsValid(Phys))
		{
			//PrintViewport(1.f, FColor::Blue, TEXT("Phys"));
			//Phys->Play(result.ImpactPoint, result.ImpactNormal);
			if (IsValid(Phys->GetParticle()))
			{
				FActorSpawnParameters	SpawnParam;

				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AParticleCascade* Particle =
					GetWorld()->SpawnActor<AParticleCascade>(result.ImpactPoint, result.ImpactNormal.Rotation(), SpawnParam);

				Particle->SetParticle(Phys->GetParticle());
				Particle->SetSound(Phys->GetSound());
			}

			else if (IsValid(Phys->GetSound()))
			{
				PrintViewport(1.f, FColor::Blue, TEXT("Sound"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Phys->GetSound(), result.ImpactPoint);
			}
		}
	}
}

void APlayerCharacter::SavePlayer()
{
	UUE11SaveGame* SaveGame = NewObject<UUE11SaveGame>();

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	SaveGame->mPlayerInfo = State->mPlayerInfo;
	SaveGame->mCameraZoomMin = State->mCameraZoomMin;
	SaveGame->mCameraZoomMax = State->mCameraZoomMax;

	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Save"), 0);
}
