// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"	// ������ ���� ��±��.
#include "EngineGlobals.h"
#include "Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

#include "UObject/NoExportTypes.h"
#include "GameInfo.generated.h"

#define	PACKET_SIZE	2048


DECLARE_LOG_CATEGORY_EXTERN(UE11, Log, All);

// �α׸� ����ϱ� ���ϰ� ��ũ�θ� �����.
// __FUNCTION__ : �Լ� �̸��� �����´�.
// __LINE__ : �ش� ���Ͽ����� �� ���� ������ ���´�.
// FString::FromInt(__LINE__) : �� ��ȣ�� ������ ���ͼ� ���ڿ���
// ������ش�.
// �Ʒ� ��ũ�δ�      �Լ��̸�[�ٹ�ȣ] <== ���ڿ��� ������ش�.
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

// UE_LOG : �𸮾󿡼� �α� ����� ���� ����Ѵ�.
// ... �� �س��� ���� �������ڸ� ����ϱ� �����̴�.
// �������ڴ� ������ ������ ���ϴ´�� �߰����� �� �ִ� ����̴�.
// %s : ���ڿ��� �޾ƿͼ� ��ü���ֱ� ���� ����Ѵ�. ���ڿ� �����͸� ������
// �־�� �Ѵ�.
// FString �տ� * �� �ٿ��� �������� �ϸ� FString�� ������ �ִ�
// ���ڿ� �����͸� ���´�.
// __VA_ARGS__ : �������ڸ� ���´�.
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

UENUM(BlueprintType)
enum class EMonsterAnimType : uint8
{
	Idle,
	Run,
	CombatRun,
	Attack,
	Death,
	KnockBack,
	Stun,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	Skill5,
	Skill6,
	Skill7,
	Skill8,
	Skill9,
	Skill10
};


USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName	Name;

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

USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	SlotNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	SkillNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class ASkillActor*	SkillActor;
};


USTRUCT(BlueprintType)
struct FMonsterTableInfo :
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	AttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	ArmorPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UAnimInstance>	MonsterAnimClass;
};


UENUM(BlueprintType)
enum class EPatrolEndDir : uint8
{
	Progress,
	Repeat
};

UENUM(BlueprintType)
enum class EPatrolType : uint8
{
	Point,
	Spline
};

UENUM(BlueprintType)
enum class EOutLineColor : uint8
{
	Color1,
	Color2,
	Color3,
	Color4,
	Color5,
	Color6,
	Color7,
	Color8,
	Color9
};


// Item ����
UENUM(BlueprintType)
enum class EITEM_TYPE : uint8
{
	EQUIP_WEAPON,
	EQUIP_ARMOR,
	EQUIP_ACCESARY,

	CONSUMABLE,
	QUEST,
};

UENUM(BlueprintType)
enum class EITEM_ID : uint8
{
	CI_POTION,
	CI_POTION_MID,
	CI_POTION_MEGA,

	EW_GREATSWORD,
	EW_LONGBOW,
	EW_POISONDAGGER,

	EA_LEATHERARMOR,
	EA_KNIGHTARMOA,

	AC_RING,
	AC_NECKELESS,
};

USTRUCT(Atomic, BlueprintType)
struct FItemDataInfo
	: public FTableRowBase // ������ ���̺�� ����
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EITEM_ID	ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EITEM_TYPE	ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		HPHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		MPHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		Att;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		Def;
};


UENUM(BlueprintType)
enum class ETriggerShape : uint8
{
	Box,
	Sphere,
	Capsule
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Passive,
	Active
};

UENUM(BlueprintType)
enum class ESkillSystem : uint8
{
	Attack_Single_Once,
	Attack_Single_Duration,
	Attack_Multi_Once,
	Attack_Multi_Duration,
	Buf_Single,
	Buf_Multi
};


UENUM(BlueprintType)
enum class ESkillOptionType : uint8
{
	Damage,
	AttackUp,
	ArmorUp,
	HPRecovery,
	MPRecovery
};


USTRUCT(Atomic, BlueprintType)
struct FSkillOption
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillOptionType	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		Option;

};

UENUM(BlueprintType)
enum class ESkillEffectType : uint8
{
	Cast,
	Play,
	Trail
};

USTRUCT(Atomic, BlueprintType)
struct FSkillEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillEffectType	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraSystem* Effect;
};


UENUM(BlueprintType)
enum class ESkillUseType : uint8
{
	HPPercent,
	Duration,
	Ratio
};

USTRUCT(Atomic, BlueprintType)
struct FSkillUseData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillUseType	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	Data;
};



USTRUCT(Atomic, BlueprintType)
struct FSkillData
	: public FTableRowBase // ������ ���̺�� ����
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillType	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillSystem	System;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		LimitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPlayerJob	LimitJob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillOption>	SkillOptionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillEffectData>	SkillEffectArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillUseData>	SkillUseDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EMonsterAnimType	AnimType;
};

USTRUCT(BlueprintType)
struct FMonsterSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ESkillType	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ESkillSystem	System;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FString		SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FString		Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillOption>	SkillOptionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillEffectData>	SkillEffectArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillUseData>	SkillUseDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	UseSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	UseMulti;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EMonsterAnimType	AnimType;
};



enum EChat_Packet_Header
{
	CPH_MSG
};



/**
 * 
 */
UCLASS()
class UE11_API UGameInfo : public UObject
{
	GENERATED_BODY()
	
};
