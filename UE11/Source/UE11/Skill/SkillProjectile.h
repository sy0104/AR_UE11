// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "SkillProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API ASkillProjectile : public ASkillActor
{
	GENERATED_BODY()
	
public:
	ASkillProjectile();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent*	mProjectile;

public:
	UProjectileMovementComponent* GetProjectile() const
	{
		return mProjectile;
	}

public:
	UFUNCTION()
	void CollisionProjectile(const FHitResult& Hit);
};
