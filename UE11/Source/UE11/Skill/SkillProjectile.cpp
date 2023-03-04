// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillProjectile.h"

ASkillProjectile::ASkillProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// UProjectileMovementComponent �� ��������� ������(������Ʈ��) ������Ʈ�� ��������� ��.
	// �θ� Ŭ�������� SceneComponent ��Ʈ�� ��������, �� ��Ʈ�� ��������
	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	mProjectile->SetUpdatedComponent(RootComponent);

	mProjectile->InitialSpeed = 1000.f;
}

void ASkillProjectile::BeginPlay()
{
	Super::BeginPlay();

	// ��������Ʈ�� �Ǿ�����. �������Ʈ�� ȣ���� �����ϴ� ���̳������� �Ǿ�����.
	// �ε����� �� ȣ��� �Լ�(CollisionProjectile)�� �����.
	mProjectile->OnProjectileStop.AddDynamic(this, &ASkillProjectile::CollisionProjectile);
}

void ASkillProjectile::CollisionProjectile(const FHitResult& Hit)
{
	// Projectile�� ������ �κ�
	
	CreateDecal(Hit);

	// SkillActor::mOnSkillEnd �� ��ϵ� �Լ����� ȣ�����ش�.
	// Broadcast(): ��ϵ� ��� �Լ��� ȣ�����ش�.
	// ��Ƽĳ��Ʈ�� �Ǿ��ִ� �ֵ��� Braodcast�� ����ؼ� ��ϵ� ��� �Լ��� ȣ���Ѵ�.
	mOnSkillEnd.Broadcast(this, Hit);
}
