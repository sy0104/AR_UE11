// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UE11PhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UUE11PhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	UUE11PhysicalMaterial();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UParticleSystem* mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USoundBase* mSound;

public:
	UParticleSystem* GetParticle()	const
	{
		return mParticle;
	}

	USoundBase* GetSound()	const
	{
		return mSound;
	}

public:
	void Play(const FVector& Loc, const FVector& Dir);
};
