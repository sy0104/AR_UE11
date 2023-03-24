// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UItemDataBase : public UObject
{
	GENERATED_BODY()
private:
	FString		m_IconPath;		// 아이콘 이미지 경로
	FString		m_Description;	// 아이템 설명
	int			m_ItemCount;	// 아이템 수량


public:
	void SetIconPath(const FString& _IconPath) {m_IconPath = _IconPath;}
	const FString& GetIconPath() { return m_IconPath; }

	void SetDescription(const FString& _Desc) { m_Description = _Desc; }
	const FString& GetDescription() { return m_Description; }

	void SetItemCount(int _Count) { m_ItemCount = _Count; }
	int GetItemCount() { return m_ItemCount; }


public:
	UItemDataBase();
	~UItemDataBase();	
};
