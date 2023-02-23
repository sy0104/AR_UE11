// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightCharacter.h"

AKnightCharacter::AKnightCharacter()
{
	// 아래 로딩 방식은 무조건 클래스의 생성자 에서만 가능하다.
	// FObjectFinder<로딩하려는 에셋의 변수타입>
	// 언리얼의 문자열은 TEXT("") 문자열을 작성한다.
	// SkeletalMesh'/Game/ParagonYin/Characters/Heroes/Yin/Skins/WindGauntlet/Meshes/Yin_WindGauntlet.Yin_WindGauntlet'
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnightAsset(TEXT("SkeletalMesh'/Game/ParagonYin/Characters/Heroes/Yin/Skins/WindGauntlet/Meshes/Yin_WindGauntlet.Yin_WindGauntlet'"));

	// 위에서 로딩한 에셋이 정상적으로 로딩이 되었는지 판단하고 정상적으로 로딩이 되었다면
	// Character 클래스가 가지고 있는 SkeletalMeshComponent에 로딩된 SkeletalMesh를 지정하여 해당 SkeletalMesh를 출력해주도록 한다.
	// Succeeded() 함수는 해당 에셋의 로딩이 성공했는지를 bool 타입으로 판단해준다.
	if (KnightAsset.Succeeded())
	{
		// 성공 했다면 SkeletalMeshComponent의 멤버 함수 중 SetSkeletalMesh 함수를 이용하여 출력할 SkeletalMesh 에셋을 지정한다.
		// Character 클래스에서 제공하는 SkeletalMeshComponent, CapsuleComponent,
		// ArrowComponent, CharacterMovemenComponent 는 private로 되어 있다.
		// 그렇기 때문에 자식클래스에서 직접 접근이 불가능하여 Character 클래스에서 제공하는 함수를 이용하여 접근해야 한다.
		// FObjectFinder 객체의 Object 변수에 로딩한 에셋이 들어가 있다.
		GetMesh()->SetSkeletalMesh(KnightAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// ClassFinder는 레퍼런스 경로를 이용하여 에셋의 클래스 정보를 불러온다.
	// 이때 레퍼런스 경로는 에디터에서 우클릭 -> 레퍼런스 복사를 이용하여 경로를 얻어올 수 있는데
	// Class 정보를 불러오기 위해서 경로의 마짐가에 _C 를 무조건 붙여주어야 한다.
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Player/Animation/ABKnight.ABKnight_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
}
