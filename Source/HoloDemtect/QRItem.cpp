// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItem.h"


UQRItem::UQRItem()
{
}

void UQRItem::SetParams(FString meshName_, FVector loc, FRotator rot, float_t scale_)
{
	meshName = meshName_;
	mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, (const TCHAR*)*("StaticMesh'" + meshName_ + "'")));
	location = loc;
	rotation = rot;
	scale = scale_;
}
