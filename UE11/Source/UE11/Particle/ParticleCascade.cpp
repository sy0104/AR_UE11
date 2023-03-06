// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleCascade.h"

AParticleCascade::AParticleCascade()
{
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	mParticle->SetupAttachment(mAudio);
}

void AParticleCascade::BeginPlay()
{
	Super::BeginPlay();

	mParticle->OnSystemFinished.AddDynamic(this, &AParticleCascade::ParticleFinish);
}

void AParticleCascade::SetParticle(const FString& Path)
{
	// ����ȭ ������� �ε��Ѵ�.
	UParticleSystem* Particle = LoadObject<UParticleSystem>(nullptr, *Path);

	if (IsValid(Particle))
		mParticle->SetTemplate(Particle);
}

void AParticleCascade::ParticleFinish(UParticleSystemComponent* Particle)
{
	Destroy();
}