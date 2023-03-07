// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBehaviorTree* mAITree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBlackboardData* mAIBlackboard;
	
public:
	AMonsterAIController();

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	void SetBehaviorTree(const FString& Path);
	void SetBlackboard(const FString& Path);
};
