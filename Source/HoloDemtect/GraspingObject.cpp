// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspingObject.h"

// Sets default values
AGraspingObject::AGraspingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ParentNode"));

	this->className = "DefaultMesh";
	std::string nameStr = std::string(TCHAR_TO_UTF8(*(this->className)));
	node = CreateDefaultSubobject<UStaticMeshComponent>(nameStr.c_str());
	node->SetupAttachment(SceneRoot);
	node->SetSimulatePhysics(true);
	node->SetLinearDamping(55);
	node->SetAngularDamping(55);
	//node set mass to 5 kg
	node->SetMassOverrideInKg(NAME_None, 5, true);

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
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	if (world == nullptr)
		return nullptr;

	AGraspingObject* grasping_object = world->SpawnActor<AGraspingObject>(SpawnInfo);
	grasping_object->node->SetStaticMesh(item->mesh);

	// Global Location/Rotation (extracted from QR)
	grasping_object->SetActorLocation(center + item->location);
	grasping_object->SetActorRotation(rotation + item->rotation);

	// Scale
	//float scale = (extent.Y + extent.Z) / 50; // Calculated from QR
	grasping_object->SetActorScale3D(FVector(item->scale, item->scale, item->scale)); // Different for each object

	// Not working code
	// grasping_object->node->SetRelativeLocation(item->location);
	// grasping_object->node->SetRelativeRotation(item->rotation);

	return grasping_object;
}

bool AGraspingObject::GrabObject_Implementation(USceneComponent* attach_to)
{
	//SetSimulatePhysics StaticMeshComponent
	node->SetSimulatePhysics(false);
	//AttachComponentToComponent attach_to to root
	//AttachToComponent(attach_to, FAttachmentTransformRules::KeepWorldTransform);
	node->AttachToComponent( attach_to, FAttachmentTransformRules::KeepWorldTransform);
	UE_LOG(LogTemp, Display, TEXT("Grabing object"));
	
	return true;
}

bool AGraspingObject::ReleaseObject_Implementation()
{
	node->SetEnableGravity(true);
	//SetSimulatePhysics StaticMeshComponent
	node->SetSimulatePhysics(true);
	//DetachFromComponent
	node->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform);
	DetachFromActor( FDetachmentTransformRules::KeepWorldTransform);
	
	DetachRootComponentFromParent( true );
	UE_LOG(LogTemp, Display, TEXT("Releasing object"));

	
	return true;
}

