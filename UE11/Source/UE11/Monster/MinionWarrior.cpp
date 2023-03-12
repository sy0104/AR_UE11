// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionWarrior.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"

AMinionWarrior::AMinionWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Super_Dusk.Minion_Lane_Super_Dusk'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);*/

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/*static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("AnimBlueprint'/Game/Monster/Animation/ABMinionWarrior.ABMinionWarrior_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);*/

	mMonsterTableRowName = TEXT("MinionWarrior");


	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);

	FConvertMaterial	Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);
}

void AMinionWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void AMinionWarrior::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 다른 BehaviorTree나 BlackboardData를 사용하면 여기에서
	// 다르게 지정해준다.
}

void AMinionWarrior::UnPossessed()
{
	Super::UnPossessed();
}

void AMinionWarrior::Attack()
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
