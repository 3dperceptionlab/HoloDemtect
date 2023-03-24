// Fill out your copyright notice in the Description page of Project Settings.


#include "Task.h"

UTask::UTask() 
{

}

void UTask::initialize(TArray<AGraspingObject*> SpawnedObjects_, AGraspingObject* evaluation_point_, FTaskInfo taskInfo_)
{
	this->SpawnedObjects = SpawnedObjects_;
	this->evaluation_point = evaluation_point_;
	this->taskInfo = taskInfo_;
}

TArray<AGraspingObject*> UTask::evaluate()
{
	TArray<AGraspingObject*> objs;

	FBox bbox = FBox::BuildAABB(evaluation_point->GetActorLocation(), bbox_size);

	for (AGraspingObject* obj : SpawnedObjects)
	{
		FVector loc = obj->GetActorLocation();
		if (bbox.IsInside(obj->GetActorLocation()))
		{
			objs.Add(obj);
		}
	}
	return objs;
}

UClass* UTask::GetTaskClassByName(const FString& name)
{
	// The following code is stolen from this function, but there is a linker error :(
	// return FEditorClassUtils::GetClassFromString(name);

	if (name.IsEmpty() || name == "None")
	{
		return nullptr;
	}

	UClass* Class = FindObject<UClass>(ANY_PACKAGE, *name);
	if (!Class)
	{
		Class = LoadObject<UClass>(nullptr, *name);
	}
	return Class;
}

TMap<FString, FTaskInfo> UTask::LoadTasksFromJSON(const FString& filename)
{
	// File content to FString
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *filename)) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read JSON file"));
	}

	TMap<FString, FTaskInfo> map;
	return map;

	// JSON reader
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		auto values = JsonValue->AsObject()->GetArrayField("tasks");
		for (auto& value : values)
		{
			FTaskInfo taskInfo;
			taskInfo.className = value->AsObject()->GetStringField("type");
			taskInfo.text = value->AsObject()->GetStringField("text");

			auto items = value->AsObject()->GetArrayField("items");
			TArray<FString> items_array;
			for (auto& item : items)
			{
				taskInfo.items.Add(item->AsObject()->GetStringField("id"));
			}
			map.Add(value->AsObject()->GetStringField("task_id"), taskInfo);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't deserialize JSON"));
	}

	return map;
}
