// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE11GameModeBase.h"
#include "Player/KnightCharacter.h"
#include "Player/UE11PlayerController.h"
#include "Player/UE11PlayerState.h"

// �𸮾� Ÿ�� �̸�
// A: ����
// U: ������Ʈ Ȥ�� �Ϲ� �𸮾� Ŭ����
// F: ����ü
// E: ������

AUE11GameModeBase::AUE11GameModeBase()
{
	// Tick �Լ� ȣ��
	PrimaryActorTick.bCanEverTick = true;

	// DefaultPawnClass �� TSubClassOf ��� ��ü�� �ް� �Ǿ� �ִ�.
	// �� ��ü�� Ŭ������ Ÿ�� ������ ��Ƶδ� ��ü�̴�.
	// �𸮾� Ŭ�������� ���ο� static ����Լ��� �ڽ��� Ÿ�� ������ ��ȯ�� �� �ִ� �Լ��� ������ �ִ�.
	// Ÿ��::StaticClass() �Լ��� �̿��Ͽ� �ش� Ÿ���� Ÿ�� ������ ���� �� �ִ�.
	DefaultPawnClass = AKnightCharacter::StaticClass();
	PlayerControllerClass = AUE11PlayerController::StaticClass();
	PlayerStateClass = AUE11PlayerState::StaticClass();
}

void AUE11GameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUE11GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AUE11GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUE11PlayerState* State = NewPlayer->GetPlayerState<AUE11PlayerState>();

	// IsValid: �� ��ü�� ��ȿ���� �ƴ����� �Ǵ����ش�.
	if (IsValid(State))
		State->InitPlayer();
}

void AUE11GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
