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
