// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItemDictionary.h"

UQRItemDictionary::UQRItemDictionary()
{
	TArray<UQRItem*> a = TArray<UQRItem*>();
	UQRItem *i = NewObject<UQRItem>();
	i->SetParams("/Game/YCB/16K/011_banana.011_banana", FVector(0, 0, 0), FRotator(0, 0, 0), 1);

	a.Add(i);
	items.Add("bottle", a);
}

UQRItemDictionary* UQRItemDictionary::CreateInstance(FString filename)
{
	UQRItemDictionary* dict = NewObject<UQRItemDictionary>();
	// TODO: deserialize JSON 
	return dict;
}

void UQRItemDictionary::SpawnQRItems(const UObject* WorldContextObject, FString key, FVector center, FRotator rotation, FVector extent)
{
	for (UQRItem* item : *items.Find(key))
	{
		AGraspingObject::SpawnGraspingObject(WorldContextObject, center, extent, rotation, item);
	}
}
