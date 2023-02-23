// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightCharacter.h"

AKnightCharacter::AKnightCharacter()
{
	// �Ʒ� �ε� ����� ������ Ŭ������ ������ ������ �����ϴ�.
	// FObjectFinder<�ε��Ϸ��� ������ ����Ÿ��>
	// �𸮾��� ���ڿ��� TEXT("") ���ڿ��� �ۼ��Ѵ�.
	// SkeletalMesh'/Game/ParagonYin/Characters/Heroes/Yin/Skins/WindGauntlet/Meshes/Yin_WindGauntlet.Yin_WindGauntlet'
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnightAsset(TEXT("SkeletalMesh'/Game/ParagonYin/Characters/Heroes/Yin/Skins/WindGauntlet/Meshes/Yin_WindGauntlet.Yin_WindGauntlet'"));

	// ������ �ε��� ������ ���������� �ε��� �Ǿ����� �Ǵ��ϰ� ���������� �ε��� �Ǿ��ٸ�
	// Character Ŭ������ ������ �ִ� SkeletalMeshComponent�� �ε��� SkeletalMesh�� �����Ͽ� �ش� SkeletalMesh�� ������ֵ��� �Ѵ�.
	// Succeeded() �Լ��� �ش� ������ �ε��� �����ߴ����� bool Ÿ������ �Ǵ����ش�.
	if (KnightAsset.Succeeded())
	{
		// ���� �ߴٸ� SkeletalMeshComponent�� ��� �Լ� �� SetSkeletalMesh �Լ��� �̿��Ͽ� ����� SkeletalMesh ������ �����Ѵ�.
		// Character Ŭ�������� �����ϴ� SkeletalMeshComponent, CapsuleComponent,
		// ArrowComponent, CharacterMovemenComponent �� private�� �Ǿ� �ִ�.
		// �׷��� ������ �ڽ�Ŭ�������� ���� ������ �Ұ����Ͽ� Character Ŭ�������� �����ϴ� �Լ��� �̿��Ͽ� �����ؾ� �Ѵ�.
		// FObjectFinder ��ü�� Object ������ �ε��� ������ �� �ִ�.
		GetMesh()->SetSkeletalMesh(KnightAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// ClassFinder�� ���۷��� ��θ� �̿��Ͽ� ������ Ŭ���� ������ �ҷ��´�.
	// �̶� ���۷��� ��δ� �����Ϳ��� ��Ŭ�� -> ���۷��� ���縦 �̿��Ͽ� ��θ� ���� �� �ִµ�
	// Class ������ �ҷ����� ���ؼ� ����� �������� _C �� ������ �ٿ��־�� �Ѵ�.
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Player/Animation/ABKnight.ABKnight_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
}
