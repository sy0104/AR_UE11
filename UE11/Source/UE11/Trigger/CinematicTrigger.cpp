// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicTrigger.h"

ACinematicTrigger::ACinematicTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	mSequencePlayer = nullptr;
	mSequenceActor = nullptr;
}

void ACinematicTrigger::TriggerBegin(const FHitResult& SweepResult)
{
	if (IsValid(mLevelSequence))
	{
		if (!mSequencePlayer)
		{
			mSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld(), mLevelSequence, mSetting, mSequenceActor);
		}

		// 재생이 시작될 때
		// FOnMovieSceneSequencePlayerEvent		OnPlay;

		// 역재생 될때
		// FOnMovieSceneSequencePlayerEvent		OnPlayReverse;

		// 재생하던 시퀀스가 멈출때
		// FOnMovieSceneSequencePlayerEvent		OnPause;

		// 재생하던 시퀀스가 끝날때
		// FOnMovieSceneSequencePlayerEvent		OnFinished;

		mSequencePlayer->Play();
		mSequencePlayer->OnFinished.AddDynamic(this, &ACinematicTrigger::SequenceFinish);
	}
}

void ACinematicTrigger::TriggerEnd()
{
}

void ACinematicTrigger::SequenceFinish()
{
}
