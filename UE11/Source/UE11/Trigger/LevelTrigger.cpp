// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTrigger.h"

// Sets default values
ALevelTrigger::ALevelTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelTrigger::TriggerBegin(const FHitResult& SweepResult)
{
	UGameplayStatics::OpenLevel(GetWorld(), mLevelName);
}

void ALevelTrigger::TriggerEnd()
{
}
