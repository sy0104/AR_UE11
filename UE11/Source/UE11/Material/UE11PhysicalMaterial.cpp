// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PhysicalMaterial.h"
#include "../Particle/ParticleCascade.h"

UUE11PhysicalMaterial::UUE11PhysicalMaterial()
{
}

void UUE11PhysicalMaterial::Play(const FVector& Loc, const FVector& Dir)
{
	/*if (IsValid(mParticle))
	{
		FActorSpawnParameters	SpawnParam;

		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				Loc, Dir.Rotation(), SpawnParam);

		Particle->SetParticle(mParticle);
		Particle->SetSound(mSound);
	}

	else if (IsValid(mSound))
	{
		PrintViewport(1.f, FColor::Blue, TEXT("Sound"));
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSound, Loc, 10.f);
	}*/
}
