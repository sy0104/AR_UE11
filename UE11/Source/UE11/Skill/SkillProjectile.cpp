// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillProjectile.h"

ASkillProjectile::ASkillProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	mProjectile->SetUpdatedComponent(RootComponent);

	mProjectile->InitialSpeed = 1000.f;
	mDistance = -1.f;
}

void ASkillProjectile::BeginPlay()
{
	Super::BeginPlay();

	mProjectile->OnProjectileStop.AddDynamic(this, 
		&ASkillProjectile::CollisionProjectile);

	mPrevLocation = GetActorLocation();
}

void ASkillProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mDistance > -1.f)
	{
		float Dist = FVector::Distance(mPrevLocation, GetActorLocation());

		mDistance -= Dist;

		if (Dist <= 0.f)
			Destroy();

		else
			mPrevLocation = GetActorLocation();
	}
}

void ASkillProjectile::CollisionProjectile(const FHitResult& Hit)
{
	CreateDecal(Hit);

	// ��ϵ� ��� �Լ��� ȣ�����ش�.
	mOnSkillEnd.Broadcast(this, Hit);
}
