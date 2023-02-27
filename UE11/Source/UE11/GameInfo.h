// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "UObject/NoExportTypes.h"
#include "GameInfo.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UE11, Log, All);

// 로그를 사용하기 편하게 매크로를 만든다.
// __FUNCTION__ : 함수 이름을 가져온다.
// __LINE__ : 해당 파일에서의 줄 수를 정수로 얻어온다.
// FString::FromInt(__LINE__) : 줄 번호를 정수로 얻어와서 문자열로 만들어준다.
// 아래 매크로는      함수이름[줄번호] <== 문자열을 만들어준다.
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

// UE_LOG : 언리얼에서 로그 출력을 위해 사용한다.
// ... 을 해놓은 것은 가변인자를 사용하기 위함이다.
// 가변인자는 인자의 개수를 원하는대로 추가해줄 수 있는 기능이다.
// %s : 문자열을 받아와서 대체해주기 위해 사용한다. 문자열 포인터를 지정해주어야 한다.
// FString 앞에 * 을 붙여서 역참조를 하면 FString이 가지고 있는 문자열 포인터를 얻어온다.
// __VA_ARGS__ : 가변인자를 얻어온다.
#define	LOG(Format, ...)	UE_LOG(UE11, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define	LOGSTRING(Str)		UE_LOG(UE11, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)


void PrintViewport(float Time, const FColor& Color, const FString& Text);


UENUM(BlueprintType)
enum class EPlayerJob : uint8
{
	None,
	Knight,
	Archer,
	Magicion,
	End
};

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	AttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	ArmorPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	MoveSpeed;
};

USTRUCT(BlueprintType)
struct FPlayerInfo : public FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPlayerJob	Job;
};


USTRUCT(BlueprintType)
struct FMonsterInfo : public FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	TraceDistance;
};



UCLASS()
class UE11_API UGameInfo : public UObject
{
	GENERATED_BODY()
	
};
