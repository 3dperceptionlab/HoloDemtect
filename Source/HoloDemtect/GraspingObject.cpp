// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspingObject.h"

// Sets default values
AGraspingObject::AGraspingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(TEXT("StaticMesh'/Game/RotationHandleFull.RotationHandleFull'"));
	//className = "DefaultMesh";
	//std::string nameStr = std::string(TCHAR_TO_UTF8(*className));
	//node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	//node->SetStaticMesh(sphere.Object);
	//node->SetRelativeScale3D(FVector(0.05, 0.05, 0.05));
	//node->SetupAttachment(SceneRoot);

}

// Called when the game starts or when spawned
void AGraspingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGraspingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

