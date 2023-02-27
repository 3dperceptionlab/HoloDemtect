// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItem.h"


UQRItem::UQRItem()
{
}

void UQRItem::SetParams(FString meshName_, FVector loc, FRotator rot, float_t scale_)
{
	meshName = meshName_;
	//mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, (const TCHAR*)*("StaticMesh'" + meshName_ + "'")));
	//UStaticMesh* mesh_ = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/YCB/16K/011_banana.011_banana'"), nullptr, LOAD_None, nullptr);
	
	//UStaticMesh* mesh_ = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/YCB/16K/017_orange/017_orange'")));
	UStaticMesh* mesh_ = LoadObject<UStaticMesh>(nullptr, (const TCHAR*)*(meshName_), nullptr, LOAD_None, nullptr);
	if (mesh_ != nullptr) {
		mesh = mesh_;
	}

	location = loc;
	rotation = rot;
	scale = scale_;
}
