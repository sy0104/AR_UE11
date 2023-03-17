// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerShape.h"
#include "CinematicTrigger.generated.h"


// 특정 시네마틱을 처리할 수 있도록 만든다.
// ACinematicTrigger가 재생시키고자 하는 시네마틱 에셋을 가지고 있도록 한다.

UCLASS()
class UE11_API ACinematicTrigger : public ATriggerShape
{
	GENERATED_BODY()
	
public:
	ACinematicTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ULevelSequence*			mLevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMovieSceneSequencePlaybackSettings		mSetting;

	ULevelSequencePlayer*	mSequencePlayer;
	ALevelSequenceActor*	mSequenceActor;

public:
	virtual void TriggerBegin(const FHitResult& SweepResult);
	virtual void TriggerEnd();

public:
	UFUNCTION()
	void SequenceFinish();
};
