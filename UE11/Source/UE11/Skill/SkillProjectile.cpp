// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillProjectile.h"

ASkillProjectile::ASkillProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// UProjectileMovementComponent 를 만들었으면 움직일(업데이트할) 컴포넌트를 지정해줘야 함.
	// 부모 클래스에서 SceneComponent 루트를 만들어놨음, 그 루트로 지정해줌
	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	mProjectile->SetUpdatedComponent(RootComponent);

	mProjectile->InitialSpeed = 1000.f;
}
