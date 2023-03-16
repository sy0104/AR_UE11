// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleNiagara.h"

AParticleNiagara::AParticleNiagara()
{
	mParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	mParticle->SetupAttachment(mAudio);
}

void AParticleNiagara::BeginPlay()
{
	Super::BeginPlay();

	// 파라미터에 값을 집어넣을 수 있게끔 지원해주고 있다.
	//mParticle->SetNiagaraVariableFloat(TEXT("SpawnCount"), 30.f);

	mParticle->OnSystemFinished.AddDynamic(this, &AParticleNiagara::ParticleFinish);
}

void AParticleNiagara::SetParticle(UNiagaraSystem* Particle)
{
	if (IsValid(Particle))
		mParticle->SetAsset(Particle);
}

void AParticleNiagara::SetParticle(const FString& Path)
{
	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(nullptr, *Path);

	// 나이아가라 컴포넌트는 SetAsset() 함수로 집어넣어야 한다.
	if (IsValid(Particle))
		mParticle->SetAsset(Particle);
}

void AParticleNiagara::ParticleFinish(UNiagaraComponent* Particle)
{
	Destroy();
}
