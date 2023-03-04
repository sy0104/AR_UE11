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
	// �Ʒ� �ε� ����� ������ Ŭ������ ������ ������ �����ϴ�.
	// FObjectFinder<�ε��Ϸ��� ������ ����Ÿ��>
	// �𸮾��� ���ڿ��� TEXT("") ���ڿ��� �ۼ��Ѵ�.
	// SkeletalMesh'/Game/ParagonYin/Characters/Heroes/Yin/Skins/WindGauntlet/Meshes/Yin_WindGauntlet.Yin_WindGauntlet'
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnightAsset(TEXT("SkeletalMesh'/Game/ParagonYin/Characters/Heroes/Yin/Skins/WindGauntlet/Meshes/Yin_WindGauntlet.Yin_WindGauntlet'"));

	// ������ �ε��� ������ ���������� �ε��� �Ǿ����� �Ǵ��ϰ� ���������� �ε��� �Ǿ��ٸ�
	// Character Ŭ������ ������ �ִ� SkeletalMeshComponent�� �ε��� SkeletalMesh�� �����Ͽ� �ش� SkeletalMesh�� ������ֵ��� �Ѵ�.
	// Succeeded() �Լ��� �ش� ������ �ε��� �����ߴ����� bool Ÿ������ �Ǵ����ش�.
	if (KnightAsset.Succeeded())
	{
		// ���� �ߴٸ� SkeletalMeshComponent�� ��� �Լ� �� SetSkeletalMesh �Լ��� �̿��Ͽ� ����� SkeletalMesh ������ �����Ѵ�.
		// Character Ŭ�������� �����ϴ� SkeletalMeshComponent, CapsuleComponent,
		// ArrowComponent, CharacterMovemenComponent �� private�� �Ǿ� �ִ�.
		// �׷��� ������ �ڽ�Ŭ�������� ���� ������ �Ұ����Ͽ� Character Ŭ�������� �����ϴ� �Լ��� �̿��Ͽ� �����ؾ� �Ѵ�.
		// FObjectFinder ��ü�� Object ������ �ε��� ������ �� �ִ�.
		GetMesh()->SetSkeletalMesh(KnightAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// ClassFinder�� ���۷��� ��θ� �̿��Ͽ� ������ Ŭ���� ������ �ҷ��´�.
	// �̶� ���۷��� ��δ� �����Ϳ��� ��Ŭ�� -> ���۷��� ���縦 �̿��Ͽ� ��θ� ���� �� �ִµ�
	// Class ������ �ҷ����� ���ؼ� ����� �������� _C �� ������ �ٿ��־�� �Ѵ�.
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Player/Animation/ABKnight.ABKnight_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();

	// PlayerInfo �� ����
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

	// ���� ��ų
	// NewObject �� �����ڰ� �ƴ� ������ ����Ѵ�.
	ASkillProjectile* SkillProjectile = NewObject<ASkillProjectile>(this, ASkillProjectile::StaticClass());
	SkillInfo.SkillActor = Cast<ASkillActor>(SkillProjectile);
	
	SkillProjectile->SetStaticMesh(TEXT("StaticMesh'/Game/ParagonYin/FX/Meshes/Environment/Animals/SM_Bat.SM_Bat'"));
	SkillProjectile->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Ultimate/FX/p_Ult_EnterBubble.p_Ult_EnterBubble'"));
	SkillProjectile->SetSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	SkillProjectile->SetCollisionProfile(TEXT("PlayerAttack"));
	
	// ��ų�� ����Ǿ��� �� ȣ��Ǵ� �Լ��� ���̳��� ��Ƽĳ��Ʈ�� �����
	// ��ų�� ����Ǿ��� ���� ��������Ʈ�� ��ϵ� �Լ����� ȣ�����ش�.
	//SkillProjectile->AddSkillEndDelegate<AKnightCharacter>(this, &AKnightCharacter::Skill1End);
	SkillProjectile->mOnSkillEnd.AddDynamic(this, &AKnightCharacter::Skill1End);

	UProjectileMovementComponent* Projectile = SkillProjectile->GetProjectile();
	Projectile->InitialSpeed = 1000.f;
	Projectile->ProjectileGravityScale = 0.f;

	// ��Į ���� ����
	ADecal* Decal = NewObject<ADecal>(this, ADecal::StaticClass());
	Decal->SetSpawnType(EDecalSpawnType::Floor);
	SkillProjectile->SetDecalLifeSpan(5.f);
	SkillProjectile->SetDecalTemplate(Decal);

	mSkillInfoArray.Add(SkillInfo);

	// ���͸� �ϳ� �������Ѿ� �Ѵ�.
	// ���͸� ������Ų ���� ĳ���� �޽��� ���ϵ�� �ٿ��ش�.
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mWeapon = GetWorld()->SpawnActor<AWeaponActor>(AWeaponActor::StaticClass(), SpawnParam);
	mWeapon->SetMesh(TEXT("StaticMesh'/Game/Meshes/Axe1.Axe1'"));
	mWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_l_socket"));
}

void AKnightCharacter::NormalAttackCheck()
{
	// ������ ���� üũ
	
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * State->GetInfo().AttackDistance;

	// �ӽ÷� �浹ü�� ���� �浹ó���� �� ���� ���� ��ü�� ���ؼ� ó���� �� �ִ�.
	// ȭ�鿡 ��ġ�Ǿ� �ִ� ���͵��� ���尡 �� �˰� �ִ� �����̴�.
	// ���� ��ü���� ���� �ȿ� �ִ� ��� ���͵�� �浹ó���� �����ؼ� �浹ü�� ã���� �Ѵ�.
	// �츮�� �浹ü�� ���� ����� ���� �ƴ϶� �׷� ������ �ϴ� �Լ��� �����ȴ�.

	FCollisionQueryParams param(NAME_None, false, this);
	
	// ���ϴ� ���͸� �߰��ؼ� Ž������ ���ܽ�ų �� �ִ�.
	//param.AddIgnoredActor();

	// �浹 ����� �������� ���� ���θ� ������ ������ �����Ѵ�.
	//param.bReturnPhysicalMaterial

	// ������ �浹�� ���ؼ� �����ؾ� �ϴ��� ����
	//param.bTraceComplex

	TArray<FHitResult> CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, StartLocation, EndLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(50.f), param);

	// ������ �浹ü �׸�
#if ENABLE_DRAW_DEBUG

	// CollisionEnable �� true�̸� Red, false�̸� Green�� �����Ѵ�.
	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	// FRotationMatrix::MakeFromZ(GetActorForwardVector())
	// ������ �ٶ󺸴� ȸ������� ���� .ToQuat() �Լ��� �̿��Ͽ� ȸ������� ȸ�������� ��ȯ���ش�.
	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f, 
		State->GetInfo().AttackDistance / 2.f, 50.f, 
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);

#endif

	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			// �ε��� ��ġ(ImpactPoint)�� �������ش�.
			// �ε��� ������ �ٶ󺸴� ����(ImpactNormal)�� ȸ�������� ����.

			// ���͸� ������ų�� �ʿ��� ��ҵ��� ������ �� �ִ�.

			FActorSpawnParameters SpawnParam;
			SpawnParam.Template = mHitActor;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AParticleCascade* Particle = 
				GetWorld()->SpawnActor<AParticleCascade>(CollisionResult[i].ImpactPoint,
				CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

			// GetActor(): �ε��� ���͸� �����´�.
			// GetController(): �� �������� �� ����ִ� ����. �� �������� ��Ʈ�ѷ��� ���ǰ� �����ϱ� ������
			// ���ǵ� ��Ʈ�ѷ��� ��ȯ���ش�.
			CollisionResult[i].GetActor()->TakeDamage((float)State->mPlayerInfo.AttackPoint,
				FDamageEvent(), GetController(), this);
		}
	}
}

void AKnightCharacter::Skill1()
{
	int32 SkillNumber = -1;
	int32 Count = mSkillInfoArray.Num();

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
	int32 Count = mSkillInfoArray.Num();

	for (int32 i = 0; i < Count; ++i)
	{
		if (mSkillInfoArray[i].SkillNumber == SkillNumber)
		{
			// ������ �ٸ� ������ ��ų�� ��� �ϴ� ����ü ��ų�� �����ؼ� �߻��Ѵ�.
			FActorSpawnParameters SpawnParam;
			SpawnParam.Template = mSkillInfoArray[i].SkillActor;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASkillProjectile* Skill =
				GetWorld()->SpawnActor<ASkillProjectile>(
					GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), SpawnParam);

			break;
		}
	}
}

void AKnightCharacter::Skill1End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	SkillActor->Destroy();
}
