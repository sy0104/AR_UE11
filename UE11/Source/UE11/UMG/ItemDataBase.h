// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataBase.generated.h"


UCLASS()
class UE11_API UItemDataBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDataBase();
	~UItemDataBase();

public:
	const FString& GetIconPath()
	{
		return mIconPath;
	}
	
	const FString& GetDescription()
	{
		return mDescription;
	}

	int GetItemCount()
	{
		return mItemCount;
	}

public:
	void SetIconPath(const FString& IconPath)
	{
		mIconPath = IconPath;
	}

	void SetDescription(const FString& Desc)
	{
		mDescription = Desc;
	}

	void SetItemCount(int Count)
	{
		mItemCount = Count;
	}


private:
	FString		mIconPath;		// 아이콘 이미지 경로
	FString		mDescription;	// 아이템 설명
	int			mItemCount;		// 아이템 수량
};
