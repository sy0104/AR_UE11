// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE11GameModeBase.h"
#include "Player/KnightCharacter.h"
#include "Player/UE11PlayerController.h"
#include "Player/UE11PlayerState.h"

// 언리얼 타입 이름
// A: 액터
// U: 컴포넌트 혹은 일반 언리얼 클래스
// F: 구조체
// E: 열거형

AUE11GameModeBase::AUE11GameModeBase()
{
	// Tick 함수 호출
	PrimaryActorTick.bCanEverTick = true;

	// DefaultPawnClass 는 TSubClassOf 라는 객체를 받게 되어 있다.
	// 이 객체는 클래스의 타입 정보를 담아두는 객체이다.
	// 언리얼 클래스들은 내부에 static 멤버함수로 자신의 타입 정보를 반환할 수 있는 함수를 가지고 있다.
	// 타입::StaticClass() 함수를 이용하여 해당 타입의 타입 정보를 얻어올 수 있다.
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

	// IsValid: 이 객체가 유효한지 아닌지를 판단해준다.
	if (IsValid(State))
		State->InitPlayer();
}

void AUE11GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
