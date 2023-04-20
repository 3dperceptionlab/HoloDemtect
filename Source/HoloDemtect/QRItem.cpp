// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItem.h"


FQRItem::FQRItem()
{
}

void FQRItem::SetParams(FString meshName_, FVector loc, FRotator rot, float scale_)
{
	meshName = meshName_;
	mesh = LoadObject<UStaticMesh>(nullptr, (const TCHAR*)*(meshName_), nullptr, LOAD_None, nullptr);
	location = loc;
	rotation = rot;
	scale = scale_;
}
