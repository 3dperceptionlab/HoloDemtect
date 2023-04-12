// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspingObject.h"

// Sets default values
AGraspingObject::AGraspingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));

	grabbed = false;
	movable = true;
	this->className = "DefaultMesh";
	std::string nameStr = std::string(TCHAR_TO_UTF8(*(this->className)));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	node->SetupAttachment(SceneRoot);
	//SetRootComponent(SceneRoot);

	//node set mass to 5 kg
	//node->SetMassOverrideInKg(NAME_None, 5, true);

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

AGraspingObject* AGraspingObject::SpawnGraspingObject(const UObject* WorldContextObject, FVector center, FVector extent, FRotator rotation, UQRItem *item)
{
	//check item->location variables are not -nan
	if (item->location.ContainsNaN()){

		UE_LOG(LogTemp, Warning, TEXT("Location is NaN"));
		return nullptr;
	}
	

	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	if (world == nullptr)
		return nullptr;

	AGraspingObject* grasping_object = world->SpawnActor<AGraspingObject>(SpawnInfo);
	grasping_object->className = item->meshName;
	grasping_object->node->SetStaticMesh(item->mesh);

	// Global Location/Rotation (extracted from QR)
	//grasping_object->SetActorLocation(center + item->location);
	//grasping_object->SetActorRotation(rotation + item->rotation);
	//grasping_object->SetLocaton
	// Scale
	//float scale = (extent.Y + extent.Z) / 50; // Calculated from QR
	//grasping_object->SetActorScale3D(FVector(item->scale, item->scale, item->scale)); // Different for each object
	grasping_object->node->SetWorldScale3D(FVector(item->scale, item->scale, item->scale));
	grasping_object->node->SetSimulatePhysics(true);
	grasping_object->node->SetEnableGravity(true);
	grasping_object->node->SetLinearDamping(10);
	grasping_object->node->SetAngularDamping(10);
	grasping_object->node->SetMassOverrideInKg(NAME_None, 5, true);
	

	grasping_object->node->SetWorldLocationAndRotation(center + item->location,rotation + item->rotation);


	// Not working code
	// grasping_object->node->SetRelativeLocation(item->location);
	// grasping_object->node->SetRelativeRotation(item->rotation);

	return grasping_object;
}

bool AGraspingObject::GrabObject_Implementation(USceneComponent* attach_to)
{
	if (!movable)
		return false;

	grabbed = true;
	//SetSimulatePhysics StaticMeshComponent
	node->SetEnableGravity(false);
	node->SetSimulatePhysics(false);
	node->AttachToComponent(attach_to, FAttachmentTransformRules::KeepWorldTransform);
	UE_LOG(LogTemp, Display, TEXT("Grabing object"));

	return true;
	
}

bool AGraspingObject::ReleaseObject_Implementation()
{
	if (!movable)
		return false;

	grabbed = false;
	node->SetEnableGravity(true);
	//SetSimulatePhysics StaticMeshComponent
	node->SetSimulatePhysics(true);
	node->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform);
	DetachFromActor( FDetachmentTransformRules::KeepWorldTransform);
	
	DetachRootComponentFromParent( true );
	UE_LOG(LogTemp, Display, TEXT("Releasing object"));


	return true;
}

void AGraspingObject::SetMovable(bool movable_){

	movable = movable_;
	node->SetEnableGravity(movable);
	node->SetSimulatePhysics(movable);
	if (movable) {
		node->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else {
		node->SetCollisionProfileName(TEXT("OverlapAll"));
		node->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	
}

