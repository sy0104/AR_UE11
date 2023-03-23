// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE11GameModeBase.h"
#include "Player/KnightCharacter.h"
#include "Player/UE11PlayerController.h"
#include "Player/UE11PlayerState.h"
#include "UE11SaveGame.h"

/*
�𸮾� Ÿ�� �̸�
A : ����
U : ������Ʈ Ȥ�� �Ϲ� �𸮾� Ŭ����
F : ����ü
E : ������
*/
AUE11GameModeBase::AUE11GameModeBase()
{
	// Tick �Լ� ȣ��
	PrimaryActorTick.bCanEverTick = true;

	// DefaultPawnClass �� TSubClassOf ��� ��ü�� �ް� �Ǿ� �ִ�.
	// �� ��ü�� Ŭ������ Ÿ�� ������ ��Ƶδ� ��ü�̴�.
	// �𸮾� Ŭ�������� ���ο� static ����Լ��� �ڽ��� Ÿ�������� ��ȯ�� �� �ִ� �Լ��� ������ �ִ�.
	// Ÿ��::StaticClass() �Լ��� �̿��Ͽ� �ش� Ÿ���� Ÿ�� ������ ���� �� �ִ�.
	PlayerControllerClass = AUE11PlayerController::StaticClass();
	PlayerStateClass = AUE11PlayerState::StaticClass();

	// UI_MainHUD �������Ʈ Ŭ������ UClass ������ �����´�.
	ConstructorHelpers::FClassFinder<UUserWidget>
		finder(TEXT("WidgetBlueprint'/Game/Blueprints/UIClass/UMG/UI_MainHUD.UI_MainHUD_C'"));

	if (finder.Succeeded())
		mMainHUDClass = finder.Class;	// �ڷ����� ���� ���� ������ �����صд�.
}

// �Ʒ� �Լ����� InitGame -> PostLogin -> BeginPlay �Լ� ������ ȣ���� �ȴ�.
void AUE11GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	mSaveGame = NewObject<UUE11SaveGame>();

	// ���� �������Ʈ UClass ������ �̿��ؼ� ��ü�� ������.
	if (IsValid(mMainHUDClass))
	{
		// ������ ��ü�� �ּҸ� mMainHUD�� �޾Ƶд�.
		mMainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), mMainHUDClass));

		if (IsValid(mMainHUD))
		{
			mMainHUD->AddToViewport();
			//mMainHUD->SetHP(0.33f);
		}
	}
}

void AUE11GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	switch (EndPlayReason)
	{
	case EEndPlayReason::Destroyed:
		LOG(TEXT("Destroyed"));
		break;
	case EEndPlayReason::LevelTransition:
		LOG(TEXT("Level Transition"));
		
		SaveGame();
		break;
	case EEndPlayReason::EndPlayInEditor:
		LOG(TEXT("EndPlayInEditor"));
		break;
	case EEndPlayReason::RemovedFromWorld:
		LOG(TEXT("RemovedFromWorld"));
		break;
	case EEndPlayReason::Quit:
		LOG(TEXT("Quit"));
		break;
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
	// IsValid : �� ��ü�� ��ȿ���� �ƴ����� �Ǵ����ش�.
	if (IsValid(State))
		State->InitPlayer();
}

void AUE11GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUE11GameModeBase::SaveGame()
{
	FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");
	FArchive* Writer = IFileManager::Get().CreateFileWriter(*FullPath);

	if (Writer)
	{
		*Writer << mSaveGame->mPlayerInfo.Name;
		*Writer << mSaveGame->mPlayerInfo.Job;
		*Writer << mSaveGame->mPlayerInfo.AttackPoint;
		*Writer << mSaveGame->mPlayerInfo.ArmorPoint;
		*Writer << mSaveGame->mPlayerInfo.HP;
		*Writer << mSaveGame->mPlayerInfo.HPMax;
		*Writer << mSaveGame->mPlayerInfo.MP;
		*Writer << mSaveGame->mPlayerInfo.MPMax;
		*Writer << mSaveGame->mPlayerInfo.Level;
		*Writer << mSaveGame->mPlayerInfo.Exp;
		*Writer << mSaveGame->mPlayerInfo.Gold;
		*Writer << mSaveGame->mPlayerInfo.MoveSpeed;
		*Writer << mSaveGame->mPlayerInfo.AttackDistance;

		*Writer << mSaveGame->mCameraZoomMin;
		*Writer << mSaveGame->mCameraZoomMax;

		// ����� �����ٸ� ��Ʈ���� �ݾ��ְ� �����ؾ� �Ѵ�.
		Writer->Close();

		delete Writer;
	}
}
