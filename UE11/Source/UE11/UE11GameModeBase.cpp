// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE11GameModeBase.h"
#include "Player/KnightCharacter.h"
#include "Player/UE11PlayerController.h"
#include "Player/UE11PlayerState.h"

/*
언리얼 타입 이름
A : 액터
U : 컴포넌트 혹은 일반 언리얼 클래스
F : 구조체
E : 열거형
*/
AUE11GameModeBase::AUE11GameModeBase()
{
	// Tick 함수 호출
	PrimaryActorTick.bCanEverTick = true;

	// DefaultPawnClass 는 TSubClassOf 라는 객체를 받게 되어 있다.
	// 이 객체는 클래스의 타입 정보를 담아두는 객체이다.
	// 언리얼 클래스들은 내부에 static 멤버함수르 자신의 타입정보를 반환할 수 있는 함수를 가지고 있다.
	// 타입::StaticClass() 함수를 이용하여 해당 타입의 타입 정보를 얻어올 수 있다.
	DefaultPawnClass = AKnightCharacter::StaticClass();
	PlayerControllerClass = AUE11PlayerController::StaticClass();
	PlayerStateClass = AUE11PlayerState::StaticClass();

	// UI_MainHUD 블루프린트 클래스의 UClass 정보를 가져온다.
	ConstructorHelpers::FClassFinder<UUserWidget>
		finder(TEXT("WidgetBlueprint'/Game/Blueprints/UIClass/UMG/UI_MainHUD.UI_MainHUD_C'"));

	if (finder.Succeeded())
		mMainHUDClass = finder.Class;	// 자료형에 대한 고유 정보를 저장해둔다.
}

// 아래 함수들은 InitGame -> PostLogin -> BeginPlay 함수 순서로 호출이 된다.
void AUE11GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 블루프린트 UClass 정보를 이용해서 객체를 만들어낸다.
	if (IsValid(mMainHUDClass))
	{
		// 생성한 객체의 주소를 mMainHUD에 받아둔다.
		mMainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), mMainHUDClass));

		if (IsValid(mMainHUD))
		{
			mMainHUD->AddToViewport();
			//mMainHUD->SetHP(0.33f);
		}
	}
}

void AUE11GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UUE11GameInstance* GameInst = GetWorld()->GetGameInstance<UUE11GameInstance>();

	if (IsValid(GameInst))
	{
		int32	PlayerJobIndex = (int32)GameInst->GetSelectJob();

		if (PlayerJobIndex == 0)
			DefaultPawnClass = AKnightCharacter::StaticClass();

		else
			DefaultPawnClass = mPlayerClassArray[PlayerJobIndex - 1];
	}
}

void AUE11GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUE11PlayerState* State = NewPlayer->GetPlayerState<AUE11PlayerState>();

	//if(State)
	// IsValid : 이 객체가 유효한지 아닌지를 판단해준다.
	if (IsValid(State))
		State->InitPlayer();
}

void AUE11GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
