// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemBase.h"

#include "ItemDataBase.h"

void UInventoryItemBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImg = Cast<UImage>(GetWidgetFromName(FName(TEXT("Icon"))));
	m_ItemNameTxt = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ItemName"))));
	m_CountTxt = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Count"))));
}

void UInventoryItemBase::NativeTick(const FGeometry& _geo, float _DT)
{
	Super::NativeTick(_geo, _DT);


}

// InventoryItem �� �����ǰ� �� ��, ����Ʈ�� �Էµ� �����ͷκ��� IventoryItem �� �ʱ�ȭ�ȴ�
void UInventoryItemBase::InitFromData(UObject* _Data)
{
	UItemDataBase* pData = Cast<UItemDataBase>(_Data);

	if (IsValid(pData))
	{		
		// �Էµ� �����Ϳ� ����ִ� ������ InventoryItem �� �ʱ�ȭ �ȴ�.	
		const FString& IconPath = pData->GetIconPath();
		const FString& ItemName = pData->GetDescription();
		int Count = pData->GetItemCount();

		// �����Ϳ� ����ִ� IconPath �� �̿��ؼ� �ش� �ؽ��ĸ� �ε� ��, 
		// Image ������ �ش� �ؽ��ĸ� �������ش�.
		UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *IconPath);
		if (IsValid(pTex2D))
		{
			m_IconImg->SetBrushFromTexture(pTex2D);
		}

		// �����Ϳ� ����ִ� ������ �̸��� ItemName �� ����� �� �ؽ�Ʈ��� ������ ����
		m_ItemNameTxt->SetText(FText::FromString(ItemName));

		// �����Ϳ� ����ִ� ������ ������ ���ڿ��� �ٲپ, ItemCount �ؽ�Ʈ��� ������ ����		
		m_CountTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}
}