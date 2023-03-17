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

		// ����� ���۵� ��
		// FOnMovieSceneSequencePlayerEvent		OnPlay;

		// ����� �ɶ�
		// FOnMovieSceneSequencePlayerEvent		OnPlayReverse;

		// ����ϴ� �������� ���⶧
		// FOnMovieSceneSequencePlayerEvent		OnPause;

		// ����ϴ� �������� ������
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
