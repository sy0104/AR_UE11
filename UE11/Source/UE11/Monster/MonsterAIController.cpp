// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMonsterAIController::SetBehaviorTree(const FString& Path)
{
	mAITree = LoadObject<UBehaviorTree>(nullptr, *Path);
}

void AMonsterAIController::SetBlackboard(const FString& Path)
{
	mAIBlackboard = LoadObject<UBlackboardData>(nullptr, *Path);
}
