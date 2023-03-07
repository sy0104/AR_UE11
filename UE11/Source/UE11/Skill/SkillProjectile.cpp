// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillProjectile.h"

ASkillProjectile::ASkillProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	mProjectile->SetUpdatedComponent(RootComponent);

	mProjectile->InitialSpeed = 1000.f;
}

void ASkillProjectile::BeginPlay()
{
	Super::BeginPlay();

	mProjectile->OnProjectileStop.AddDynamic(this, 
		&ASkillProjectile::CollisionProjectile);
}

void ASkillProjectile::CollisionProjectile(const FHitResult& Hit)
{
	CreateDecal(Hit);

	// ��ϵ� ��� �Լ��� ȣ�����ش�.
	mOnSkillEnd.Broadcast(this, Hit);
}
