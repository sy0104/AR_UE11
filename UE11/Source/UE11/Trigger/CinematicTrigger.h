// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerShape.h"
#include "CinematicTrigger.generated.h"


UCLASS()
class UE11_API ACinematicTrigger : public ATriggerShape
{
	GENERATED_BODY()

public:
	ACinematicTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ULevelSequence*	mLevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMovieSceneSequencePlaybackSettings	mSetting;

	ULevelSequencePlayer* mSequencePlayer;
	ALevelSequenceActor* mSequenceActor;

public:
	virtual void TriggerBegin(const FHitResult& SweepResult);
	virtual void TriggerEnd();

public:
	UFUNCTION()
	void SequenceFinish();
};
