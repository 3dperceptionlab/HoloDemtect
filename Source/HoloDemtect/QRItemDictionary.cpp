// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItemDictionary.h"

UQRItemDictionary::UQRItemDictionary()
{
}

UQRItemDictionary* UQRItemDictionary::CreateInstance(FString filename)
{
	UQRItemDictionary* dict = NewObject<UQRItemDictionary>();
	// TODO: deserialize JSON 
	return dict;
}

void UQRItemDictionary::SpawnQRItems(const UObject* WorldContextObject, FString key, FVector center, FRotator rotation, FVector extent)
{
	for (const UQRItem& item : *items.Find(key))
	{
		AGraspingObject::SpawnGraspingObject(WorldContextObject, center, extent, rotation, item);
	}
}
