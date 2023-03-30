// Fill out your copyright notice in the Description page of Project Settings.


#include "LayTableTask.h"

ULayTableTask::ULayTableTask()
{
	eval_point_info = NewObject<UQRItem>();
	eval_point_info->SetParams("/Game/EvalPointAssets/Basket", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
}

void ULayTableTask::initialize(AGraspingObject* evaluation_point_, FTaskInfo taskInfo_)
{
	UTask::initialize(evaluation_point_, taskInfo_);

	for (FString s : taskInfo.items)
	{
		table_elements.AddTail(s);
	}
}

TArray<AGraspingObject*> ULayTableTask::evaluate()
{
	return TArray<AGraspingObject*>();
}

bool ULayTableTask::AreObjectsValid(TArray<AGraspingObject*> objs)
{
	TDoubleLinkedList<FString> objs_classes;
	for (auto* obj : objs) {
		objs_classes.AddTail(obj->className);
	}

	for (FString s : table_elements) {
		auto* node = objs_classes.FindNode(s);
		if (node == nullptr)
			return false;

		objs_classes.RemoveNode(node);
	}

	SpawnedObjects = objs;
	return true;
}

TArray<FString> ULayTableTask::GetTaskItems(TArray<AGraspingObject*> objs)
{
	TArray<FString> items;

	TDoubleLinkedList<FString> objs_classes;
	for (auto* obj : objs) {
		objs_classes.AddTail(obj->className);
	}

	for (FString s : table_elements) {
		auto* node = objs_classes.FindNode(s);
		if (node == nullptr) {
			FString itemString;
			s.Split(TEXT("_"), nullptr, &itemString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

			items.Add(itemString);
		}
	}

	return items;
}

bool ULayTableTask::IsTaskFinished()
{
	return false;
}
