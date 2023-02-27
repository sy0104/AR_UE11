// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionWarrior.h"

AMinionWarrior::AMinionWarrior()
{
	// 한번만 동작하면 되니까 static으로 해준다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Super_Dusk.Minion_Lane_Super_Dusk'"));
	
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Monster/Animation/ABMinionWarrior.ABMinionWarrior_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);


	// MonsterInfo 값 설정
	mInfo.Level = 1;
	mInfo.Exp = 1000;
	mInfo.Gold = 1000;
	mInfo.AttackDistance = 150.f;
	mInfo.AttackPoint = 50;
	mInfo.ArmorPoint = 10;
	mInfo.HP = 100;
	mInfo.HPMax = 100;
	mInfo.MP = 50;
	mInfo.MPMax = 50;
	mInfo.TraceDistance = 1000.f;
	mInfo.MoveSpeed = 600.f;
}
