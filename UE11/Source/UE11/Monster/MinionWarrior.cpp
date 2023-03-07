// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionWarrior.h"
#include "MonsterAIController.h"

AMinionWarrior::AMinionWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Super_Dusk.Minion_Lane_Super_Dusk'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);*/

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/*static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("AnimBlueprint'/Game/Monster/Animation/ABMinionWarrior.ABMinionWarrior_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);*/

	mMonsterTableRowName = TEXT("MinionWarrior");
}

void AMinionWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void AMinionWarrior::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 다른 BehaviorTree나 BlackboardData를 사용하면 여기에서
	// 다르게 지정해준다.
}

void AMinionWarrior::UnPossessed()
{
	Super::UnPossessed();
}
