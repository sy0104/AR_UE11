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

void ASkillProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 델리게이트로 되어있음. 블루프린트로 호출이 가능하니 다이나믹으로 되어있음.
	// 부딪혔을 때 호출될 함수(CollisionProjectile)을 등록함.
	mProjectile->OnProjectileStop.AddDynamic(this, &ASkillProjectile::CollisionProjectile);
}

void ASkillProjectile::CollisionProjectile(const FHitResult& Hit)
{
	// Projectile이 끝나는 부분
	
	CreateDecal(Hit);

	// SkillActor::mOnSkillEnd 에 등록된 함수들을 호출해준다.
	// Broadcast(): 등록된 모든 함수를 호출해준다.
	// 멀티캐스트로 되어있는 애들은 Braodcast를 사용해서 등록된 모든 함수를 호출한다.
	mOnSkillEnd.Broadcast(this, Hit);
}
