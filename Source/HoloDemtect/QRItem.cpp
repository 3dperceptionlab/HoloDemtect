// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItem.h"

UQRItem::UQRItem(FString meshName_, float_t x, float_t y, float_t z, float_t rx, float_t ry, float_t rz, float_t scale_)
{
	this->meshName = meshName_;
	this->location = FVector(x, y, z);
	this->rotation = FRotator(rx, ry, rz);
	this->scale = scale_;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_object((const TCHAR*)*("StaticMesh'" + meshName_ + "'"));
	this->mesh = mesh_object.Object;
}
