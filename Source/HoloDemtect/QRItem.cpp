// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItem.h"


UQRItem::UQRItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_object(TEXT("StaticMesh'/Game/YCB/16K/017_orange/017_orange'"));
	mesh = mesh_object.Object;
}

void UQRItem::SetParams(FString meshName_, FVector loc, FRotator rot, float_t scale_)
{
	meshName = meshName_;
	//mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, (const TCHAR*)*("StaticMesh'" + meshName_ + "'")));
	location = loc;
	rotation = rot;
	scale = scale_;
}
