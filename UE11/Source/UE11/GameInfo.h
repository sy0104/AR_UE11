// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameInfo.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UE11, Log, All);

// �α׸� ����ϱ� ���ϰ� ��ũ�θ� �����.
// __FUNCTION__ : �Լ� �̸��� �����´�.
// __LINE__ : �ش� ���Ͽ����� �� ���� ������ ���´�.
// FString::FromInt(__LINE__) : �� ��ȣ�� ������ ���ͼ� ���ڿ��� ������ش�.
// �Ʒ� ��ũ�δ�      �Լ��̸�[�ٹ�ȣ] <== ���ڿ��� ������ش�.
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

// UE_LOG : �𸮾󿡼� �α� ����� ���� ����Ѵ�.
// ... �� �س��� ���� �������ڸ� ����ϱ� �����̴�.
// �������ڴ� ������ ������ ���ϴ´�� �߰����� �� �ִ� ����̴�.
// %s : ���ڿ��� �޾ƿͼ� ��ü���ֱ� ���� ����Ѵ�. ���ڿ� �����͸� �������־�� �Ѵ�.
// FString �տ� * �� �ٿ��� �������� �ϸ� FString�� ������ �ִ� ���ڿ� �����͸� ���´�.
// __VA_ARGS__ : �������ڸ� ���´�.
#define	LOG(Format, ...)	UE_LOG(UE11, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define	LOGSTRING(Str)		UE_LOG(UE11, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)


void PrintViewport(float Time, const FColor& Color, const FString& Text);



UCLASS()
class UE11_API UGameInfo : public UObject
{
	GENERATED_BODY()
	
};
