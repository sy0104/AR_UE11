// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleBase.h"
#include "ParticleCascade.generated.h"


UCLASS()
class UE11_API AParticleCascade : public AParticleBase
{
	GENERATED_BODY()
	
public:
	AParticleCascade();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UParticleSystemComponent*	mParticle;

public:
	virtual void SetParticle(const FString& Path);
	
public:
	UFUNCTION()
	void ParticleFinish(UParticleSystemComponent* Particle);

};
