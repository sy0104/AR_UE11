// Fill out your copyright notice in the Description page of Project Settings.


#include "Grux.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleNiagara.h"
#include "../UE11GameInstance.h"
#include "../Skill/SkillProjectile.h"

AGrux::AGrux()
{
	PrimaryActorTick.bCanEverTick = true;

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Super_Dusk.Minion_Lane_Super_Dusk'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);*/

	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -120.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/*static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("AnimBlueprint'/Game/Monster/Animation/ABMinionWarrior.ABMinionWarrior_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);*/

	mMonsterTableRowName = TEXT("Grux");


	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);

	FConvertMaterial	Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);

	mSkillNameArray.Add(TEXT("GruxSkill1"));
	mSkillNameArray.Add(TEXT("GruxSkill2"));
	mSkillNameArray.Add(TEXT("GruxSkill3"));
}

void AGrux::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AGrux::BeginPlay()
{
	Super::BeginPlay();
}

void AGrux::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 다른 BehaviorTree나 BlackboardData를 사용하면 여기에서
	// 다르게 지정해준다.
}

void AGrux::UnPossessed()
{
	Super::UnPossessed();
}

void AGrux::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters	SpawnParam;
		//SpawnParam.Template = mHitActor;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 타겟과 몬스터 사이에 이펙트를 재생한다.
		FVector	Dir = GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();

		FVector	ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				ParticleLoc,
				Dir.Rotation(),
				SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
		Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

		Target->TakeDamage(
			(float)mInfo.AttackPoint,
			FDamageEvent(), GetController(), this);
	}
}

void AGrux::Skill1()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters	SpawnParam;
		//SpawnParam.Template = mHitActor;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 타겟과 몬스터 사이에 이펙트를 재생한다.
		FVector	Dir = GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();

		FVector	ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleNiagara* Particle =
			GetWorld()->SpawnActor<AParticleNiagara>(
				ParticleLoc,
				Dir.Rotation(),
				SpawnParam);

		//Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));

		UNiagaraSystem* PlayEffect = nullptr;
		UNiagaraSystem* TrailEffect = nullptr;

		int32	EffectCount = mSkillDataArray[mUseSkillIndex].SkillEffectArray.Num();

		for (int32 i = 0; i < EffectCount; ++i)
		{
			switch (mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Type)
			{
			case ESkillEffectType::Cast:
				break;
			case ESkillEffectType::Play:
				PlayEffect = mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Effect;
				break;
			case ESkillEffectType::Trail:
				break;
			}
			
		}

		Particle->SetParticle(PlayEffect);
		Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

		float	DmgRatio = 0.f;

		int32	OptionCount = mSkillDataArray[mUseSkillIndex].SkillOptionArray.Num();

		for (int32 i = 0; i < OptionCount; ++i)
		{
			if (mSkillDataArray[mUseSkillIndex].SkillOptionArray[i].Type ==
				ESkillOptionType::Damage)
				DmgRatio += mSkillDataArray[mUseSkillIndex].SkillOptionArray[i].Option;
		}

		DmgRatio = DmgRatio == 0.f ? 1.f : DmgRatio;

		Target->TakeDamage(
			(float)mInfo.AttackPoint * DmgRatio,
			FDamageEvent(), GetController(), this);
	}
}

void AGrux::Skill2()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	FActorSpawnParameters	SpawnParam;

	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASkillProjectile* Skill =
		GetWorld()->SpawnActor<ASkillProjectile>(
			GetActorLocation() + GetActorForwardVector() * 100.f,
			GetActorRotation(),
			SpawnParam);

	UNiagaraSystem* PlayEffect = nullptr;
	UNiagaraSystem* TrailEffect = nullptr;

	int32	EffectCount = mSkillDataArray[mUseSkillIndex].SkillEffectArray.Num();

	for (int32 i = 0; i < EffectCount; ++i)
	{
		switch (mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Type)
		{
		case ESkillEffectType::Cast:
			break;
		case ESkillEffectType::Play:
			PlayEffect = mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Effect;
			break;
		case ESkillEffectType::Trail:
			break;
		}

	}

	Skill->SetNiagara(PlayEffect);
	Skill->SetCollisionProfile(TEXT("MonsterAttack"));
	Skill->SetDistance(mSkillDataArray[mUseSkillIndex].Distance);
}

void AGrux::Skill3()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	FActorSpawnParameters	SpawnParam;

	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleNiagara* Skill =
		GetWorld()->SpawnActor<AParticleNiagara>(
			Target->GetActorLocation(),
			GetActorRotation(),
			SpawnParam);

	UNiagaraSystem* PlayEffect = nullptr;
	UNiagaraSystem* TrailEffect = nullptr;

	int32	EffectCount = mSkillDataArray[mUseSkillIndex].SkillEffectArray.Num();

	for (int32 i = 0; i < EffectCount; ++i)
	{
		switch (mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Type)
		{
		case ESkillEffectType::Cast:
			break;
		case ESkillEffectType::Play:
			PlayEffect = mSkillDataArray[mUseSkillIndex].SkillEffectArray[i].Effect;
			break;
		case ESkillEffectType::Trail:
			break;
		}

	}
	
	Skill->SetParticle(PlayEffect);
}
