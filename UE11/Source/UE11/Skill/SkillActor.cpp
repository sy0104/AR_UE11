// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActor.h"

// Sets default values
ASkillActor::ASkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	mSound = nullptr;

	mSkeletalMesh->SetupAttachment(mRoot);
	mStaticMesh->SetupAttachment(mRoot);
	mParticle->SetupAttachment(mRoot);

	mRoot->bVisualizeComponent = true;
}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActor::SetSkeletalMesh(const FString& Path)
{
	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, *Path);

	if (IsValid(SkeletalMesh))
		mSkeletalMesh->SetSkeletalMesh(SkeletalMesh);
}

void ASkillActor::SetStaticMesh(const FString& Path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *Path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);

}

void ASkillActor::SetParticle(const FString& Path)
{
	UParticleSystem* Particle = LoadObject<UParticleSystem>(nullptr, *Path);

	if (IsValid(Particle))
		mParticle->SetTemplate(Particle);
}

void ASkillActor::SetSound(const FString& Path)
{
	mSound = LoadObject<USoundBase>(nullptr, *Path);
}

