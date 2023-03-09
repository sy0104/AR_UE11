// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightCharacter.h"
#include "UE11PlayerState.h"
#include "../Particle/ParticleCascade.h"
#include "PlayerAnimInstance.h"
#include "../Skill/SkillProjectile.h"
#include "../Particle/Decal.h"
#include "../Item/WeaponActor.h"

AKnightCharacter::AKnightCharacter()
{
	// 아래 로딩 방식은 무조건 클래스의 생성자 에서만 가능하다.
	// FObjectFinder<로딩하려는 애셋의 변수타입>
	// 언리얼의 문자열은 TEXT("") 문자열을 작성한다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	KnightAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonYin/Characters/Heroes/Yin/Skins/WindGauntlet/Meshes/Yin_WindGauntlet.Yin_WindGauntlet'"));

	// 위에서 로딩한 애셋이 정상적으로 로딩이 되었는지 판단하고
	// 정상적으로 로딩이 되었다면 Character 클래스가 가지고 있는
	// SkeletalMeshComponent에 로딩된 SkeletalMesh를 지정하여
	// 해당 SkeletalMesh를 출력해주도록 한다.
	// Succeeded() 함수는 해당 애셋의 로딩이 성공했는지를
	// bool 타입으로 판단해준다.
	if (KnightAsset.Succeeded())
	{
		// 성공 했다면 SkeletalMeshComponent의 멤버 함수 중
		// SetSkeletalMesh 함수를 이용하여 출력할 SkeletalMesh
		// 애셋을 지정한다.
		// Character 클래스에서 제공하는 SkeletalMeshComponent,
		// CapsuleComponent, ArrowComponent,
		// CharacterMovemenComponent는 private으로 되어 있다.
		// 그렇기 때문에 자식클래스에서 직접 접근이 불가능하여
		// Character 클래스에서 제공하는 함수를 이용하여
		// 접근해야 한다.
		// FObjectFinder 객체의 Object 변수에 로딩한 애셋이
		// 들어가 있다.
		GetMesh()->SetSkeletalMesh(KnightAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// ClassFinder는 레퍼런스경로를 이용하여 애셋의 클래스 정보를 불러온다.
	// 이때 레퍼런스 경로는 에디터에서 우클릭 -> 레퍼런스 복사를 이용하여
	// 경로를 얻어올 수 있는데 Class정보를 불러오기 위해서 경로의 마지막에
	// _C 를 무조건 붙여주어야 한다.
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("AnimBlueprint'/Game/Player/Animation/ABKnight.ABKnight_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

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

	FSkillInfo	SkillInfo;
	SkillInfo.SlotNumber = 0;
	SkillInfo.SkillNumber = 0;
	SkillInfo.Damage = 300;


	ASkillProjectile* SkillProjectile = NewObject<ASkillProjectile>(this,
		ASkillProjectile::StaticClass());
	SkillInfo.SkillActor = Cast<ASkillActor>(SkillProjectile);

	SkillProjectile->SetStaticMesh(TEXT("StaticMesh'/Game/ParagonYin/FX/Meshes/Environment/Animals/SM_Bat.SM_Bat'"));
	SkillProjectile->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Ultimate/FX/p_Ult_EnterBubble.p_Ult_EnterBubble'"));
	SkillProjectile->SetSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	SkillProjectile->SetCollisionProfile(TEXT("PlayerAttack"));

	//SkillProjectile->AddSkillEndDelegate<AKnightCharacter>(this,
	//	&AKnightCharacter::Skill1End);
	SkillProjectile->mOnSkillEnd.AddDynamic(this,
		&AKnightCharacter::Skill1End);

	UProjectileMovementComponent* Projectile = SkillProjectile->GetProjectile();

	Projectile->InitialSpeed = 1000.f;
	Projectile->ProjectileGravityScale = 0.f;

	ADecal* Decal = NewObject<ADecal>(this, ADecal::StaticClass());

	Decal->SetDecalMaterial(TEXT("Material'/Game/Materials/MTBurn.MTBurn'"));
	Decal->SetSpawnType(EDecalSpawnType::Floor);
	SkillProjectile->SetDecalLifeSpan(5.f);

	SkillProjectile->SetDecalTemplate(Decal);


	mSkillInfoArray.Add(SkillInfo);

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mWeapon = GetWorld()->SpawnActor<AWeaponActor>(
		AWeaponActor::StaticClass(), SpawnParam);

	mWeapon->SetMesh(TEXT("StaticMesh'/Game/Meshes/Axe1.Axe1'"));

	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("weapon_l_socket"));
}

void AKnightCharacter::NormalAttackCheck()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	FVector	StartLocation = GetActorLocation() +
		GetActorForwardVector() * 30.f;
	FVector	EndLocation = StartLocation +
		GetActorForwardVector() * State->GetInfo().AttackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	// 원하는 액터를 추가해서 탐색에서 제외시킬 수 있다.
	//param.AddIgnoredActor()

	// 충돌 결과로 물리적인 재질 여부를 가지고 올지를 결정한다.
	//param.bReturnPhysicalMaterial

	// 복잡한 충돌에 대해서 추적해야 하는지 여부
	//param.bTraceComplex

	TArray<FHitResult>	CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(
		CollisionResult, StartLocation,
		EndLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(50.f),
		param);

	// #define으로 ENABLE_DRAW_DEBUG 가 되어 있는지 체크한다.
#if ENABLE_DRAW_DEBUG

	// CollisionEnable 가 true이면 Red, false이면 Green을 저장한다.
	FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	// FRotationMatrix::MakeFromZ(GetActorForwardVector()) : 앞쪽을
	// 바라보는 회전행렬을 만들어서 .ToQuat() 함수를 이용하여 회전행렬을
	// 회전값으로 변환해준다.
	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
		State->GetInfo().AttackDistance / 2.f,
		50.f, 
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, false, 0.5f);

#endif

	if (CollisionEnable)
	{
		int32	Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			/*
			언리얼엔진의 CDO(Class Default Object) : 모든 언리얼 클래스들은
			CDO가 만들어진다. CDO는 해당 클래스의 기본 객체를 생성해두는 개념이다.

			FActorSpawnParameters 구조체 주요 멤버
			FName Name : 스폰되는 액터의 이름을 지정할 수 있다.

			AActor* Template : 액터를 스폰할때 템플릿으로 사용할 액터를 지정한다.
			nullptr로 남겨두게 되면 CDO를 이용해서 스폰하는 객체를 초기화 하고
			지정이 되면 해당 객체를 이용해서 액터를 초기화 한다.

			AActor* Owner : 이 액터를 스폰한 액터를 지정한다.

			class	ULevel* OverrideLevel : 원하는 레벨을 지정하여
			해당 레벨에 스폰시킨다. 

			ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride :
			액터를 스폰시키는 방법을 지정한다.
			*/
			FActorSpawnParameters	SpawnParam;
			//SpawnParam.Template = mHitActor;
			SpawnParam.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AParticleCascade* Particle = 
				GetWorld()->SpawnActor<AParticleCascade>(
				CollisionResult[i].ImpactPoint,
				CollisionResult[i].ImpactNormal.Rotation(),
					SpawnParam);

			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
			Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

			CollisionResult[i].GetActor()->TakeDamage(
				(float)State->mPlayerInfo.AttackPoint,
				FDamageEvent(), GetController(), this);
			//Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
		}
	}
}

void AKnightCharacter::Skill1()
{
	int32	SkillNumber = -1;

	int32	Count = mSkillInfoArray.Num();

	for (int32 i = 0; i < Count; ++i)
	{
		if (mSkillInfoArray[i].SlotNumber == 0)
		{
			SkillNumber = mSkillInfoArray[i].SkillNumber;
			break;
		}
	}

	if (SkillNumber == -1)
		return;

	mAnimInst->UseSkill(SkillNumber);
}

void AKnightCharacter::UseSkill(int32 SkillNumber)
{
	int32	Count = mSkillInfoArray.Num();

	for (int32 i = 0; i < Count; ++i)
	{
		if (mSkillInfoArray[i].SkillNumber == SkillNumber)
		{
			// 지금은 다른 종류의 스킬이 없어서 일단 투사체 스킬을
			// 생성해서 발사한다.
			FActorSpawnParameters	SpawnParam;
			SpawnParam.Template = mSkillInfoArray[i].SkillActor;
			SpawnParam.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASkillProjectile* Skill =
				GetWorld()->SpawnActor<ASkillProjectile>(
					GetActorLocation() + GetActorForwardVector() * 100.f,
					GetActorRotation(),
					SpawnParam);

			break;
		}
	}
}

void AKnightCharacter::Skill1End(ASkillActor* SkillActor,
	const FHitResult& Hit)
{
	SkillActor->Destroy();
}
