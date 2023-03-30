// Fill out your copyright notice in the Description page of Project Settings.


#include "PairMatchingTask.h"

UPairMatchingTask::UPairMatchingTask()
{
	eval_point_info = NewObject<UQRItem>();
	eval_point_info->SetParams("/Game/EvalPointAssets/Basket", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
}

void UPairMatchingTask::initialize(AGraspingObject* evaluation_point_, FTaskInfo taskInfo_)
{
	UTask::initialize(evaluation_point_, taskInfo_);

	for (int i = 0; i < taskInfo.items.Num(); i+=2)
	{
		pairings.Add(taskInfo.items[i], taskInfo.items[i + 1]);
		pairings.Add(taskInfo.items[i + 1], taskInfo.items[i]);
	}
}

TArray<AGraspingObject*> UPairMatchingTask::evaluate()
{
	TArray<AGraspingObject*> objs = UTask::evaluate();
	TArray<AGraspingObject*> errors;

	if (objs.Num() == 2)
	{
		if (objs[0]->className.Equals(pairings[objs[1]->className]))
		{
			pairings.FindAndRemoveChecked(pairings[objs[0]->className]);
			pairings.FindAndRemoveChecked(objs[0]->className);

			objs[1]->Destroy();
			objs.RemoveAt(1);
			objs[0]->Destroy();
			objs.RemoveAt(0);


			objs.Empty();
		}
	}
	else
		errors.Append(objs);


	return errors;
}

bool UPairMatchingTask::AreObjectsValid(TArray<AGraspingObject*> objs)
{
	TDoubleLinkedList<FString> objs_classes;
	for (auto* obj : objs) {
		objs_classes.AddTail(obj->className);
	}

	TArray<FString> keys;
	pairings.GetKeys(keys);

	for (FString s : keys) {
		auto* node = objs_classes.FindNode(s);
		if (node == nullptr)
			return false;

		objs_classes.RemoveNode(node);
	}

	SpawnedObjects = objs;
	return true;
}

TArray<FString> UPairMatchingTask::GetTaskItems(TArray<AGraspingObject*> objs)
{
	TArray<FString> items;

	TDoubleLinkedList<FString> objs_classes;
	for (auto* obj : objs) {
		objs_classes.AddTail(obj->className);
	}

	TSet<FString> keys;
	pairings.GetKeys(keys);

	for (FString s : keys) {
		auto* node = objs_classes.FindNode(s);
		if (node == nullptr) {
			FString itemString;
			s.Split(TEXT("_"), nullptr, &itemString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

			items.Add(itemString);
		}
	}

	return items;
}

bool UPairMatchingTask::IsTaskFinished()
{
	return !(bool)pairings.Num();
}