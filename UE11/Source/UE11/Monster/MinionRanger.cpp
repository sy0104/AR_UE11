// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionRanger.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"

AMinionRanger::AMinionRanger()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("MinionRanger");

	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);

	mMuzzleParticle = CreateDefaultSubobject<AParticleCascade>(TEXT("MuzzleParticle"));

	Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_MinionMuzzle.P_MinionMuzzle'"));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);
}

void AMinionRanger::BeginPlay()
{
	Super::BeginPlay();
}

void AMinionRanger::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 다른 BehaviorTree나 BlackboardData를 사용하면 여기에서
	// 다르게 지정해준다.
}

void AMinionRanger::UnPossessed()
{
	Super::UnPossessed();
}

void AMinionRanger::Attack()
{
	// 총구의 위치를 얻어온다.
	FVector MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_Front_XForward"));

	FActorSpawnParameters	SpawnParam;
	//SpawnParam.Template = mMuzzleParticle;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* MuzzleParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			MuzzleLoc,
			GetActorForwardVector().Rotation(),
			SpawnParam);

	MuzzleParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_MinionMuzzle.P_MinionMuzzle'"));
	MuzzleParticle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

	FVector	End = MuzzleLoc + GetActorForwardVector() *
		mInfo.AttackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	FHitResult	result;
	bool Hit = GetWorld()->LineTraceSingleByChannel(result,
		MuzzleLoc, End, ECollisionChannel::ECC_GameTraceChannel7,
		param);


#if ENABLE_DRAW_DEBUG
	FColor	Color = Hit ? FColor::Red : FColor::Green;

	DrawDebugLine(GetWorld(), MuzzleLoc, End, Color,
		false, 1.f, 2.f);

#endif

	if (Hit)
	{
		FActorSpawnParameters	HitSpawnParam;
		//HitSpawnParam.Template = mHitActor;
		HitSpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				result.ImpactPoint,
				result.ImpactNormal.Rotation(),
				HitSpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
		Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

		result.GetActor()->TakeDamage(
			(float)mInfo.AttackPoint,
			FDamageEvent(), GetController(), this);
	}
}
