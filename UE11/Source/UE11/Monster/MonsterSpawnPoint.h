// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

UCLASS()
class UE11_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent*	mRoot;

	// TSubclassOf �� ���ø����� ���� Ÿ���� UCLASS ������ �����ϴ� Ÿ���̴�.
	// class ������ �����صξ��ٰ� �ʿ�� �ش� Ŭ���� ������ �̿��Ͽ� ��ü�� ������ �� �ִ� �뵵�� ����� �� �ִ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMonster>		mSpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mSpawnCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonster*>		mMonsterArray;

	float	mTime;	// �ð� üũ��

public:
	void RemoveMonster(class AMonster* Monster)
	{
		mMonsterArray.Remove(Monster);
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
