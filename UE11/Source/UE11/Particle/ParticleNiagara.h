// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleBase.h"
#include "ParticleNiagara.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API AParticleNiagara : public AParticleBase
{
	GENERATED_BODY()

public:
	AParticleNiagara();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mParticle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void SetParticle(UNiagaraSystem* Particle);
	virtual void SetParticle(const FString& Path);

public:
	UFUNCTION()
	void ParticleFinish(UNiagaraComponent* Particle);
};
