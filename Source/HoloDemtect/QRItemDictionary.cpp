// Fill out your copyright notice in the Description page of Project Settings.


#include "QRItemDictionary.h"

UQRItemDictionary::UQRItemDictionary()
{
}

UQRItemDictionary* UQRItemDictionary::CreateInstance(FString filename)
{
	UQRItemDictionary* dict = NewObject<UQRItemDictionary>();
	
	// File content to FString
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *filename)) {
		UE_LOG(LogTemp, Error, TEXT("Couldn't read JSON file"));
	}
	

	// JSON reader
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	
	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		// Get keys field: so we know which fields to iterate
		TArray<FString> keys;
		JsonValue->AsObject()->TryGetStringArrayField("keys", keys);

		// Iterate over the keys
		for (FString key : keys)
		{
			// Extract UQRItems for each key
			TArray<UQRItem*> qr_items;
			auto values = JsonValue->AsObject()->GetArrayField(key);
			for (auto& value : values)
			{
				UQRItem* uqritem = NewObject<UQRItem>();
				FString meshName = value->AsObject()->GetStringField("mesh");
				FVector loc(value->AsObject()->GetNumberField("x"), value->AsObject()->GetNumberField("y"), value->AsObject()->GetNumberField("z"));
				FRotator rot(value->AsObject()->GetNumberField("rx"), value->AsObject()->GetNumberField("ry"), value->AsObject()->GetNumberField("rz"));
				float scale = value->AsObject()->GetNumberField("scale");
				uqritem->SetParams(meshName, loc, rot, scale);
				qr_items.Add(uqritem);
			}
			dict->items.Add(key, qr_items);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't deserialize JSON"));
	}
	
	return dict;
}

void UQRItemDictionary::SpawnQRItems(const UObject* WorldContextObject, FString key, FVector center, FRotator rotation, FVector extent)
{
	if (!items.Contains(key)) {
		UE_LOG(LogTemp, Error, TEXT("Key is not in QR items dict"));
		return;
	}

	for (UQRItem* item : *items.Find(key))
	{
		auto graspNewObject = AGraspingObject::SpawnGraspingObject(WorldContextObject, center, extent, rotation, item);
		if(graspNewObject != nullptr)
			spawned_objects.Add(graspNewObject);
	}

}
