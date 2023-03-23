// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "KnightCharacter.generated.h"





UCLASS()
class UE11_API AKnightCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AKnightCharacter();

protected:
	bool	mGhostEnable;
	float	mGhostTime;
	float	mGhostTimeMax;
	float	mGhostCreateTime;
	float	mGhostCreateTimeMax;
	USkeletalMesh* mGhostMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NormalAttackCheck();
	virtual void Skill1();
	virtual void Skill2();

	virtual void UseSkill(int32 SkillNumber);

public:
	UFUNCTION()
	void Skill1End(class ASkillActor* SkillActor, const FHitResult& Hit);

public:
	virtual void SavePlayer();
};
