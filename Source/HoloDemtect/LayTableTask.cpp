// Fill out your copyright notice in the Description page of Project Settings.


#include "LayTableTask.h"

ULayTableTask::ULayTableTask()
{
	eval_point_info = FQRItem();
	eval_point_info.SetParams("/Game/EvalPointAssets/SetupTable", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
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
	TArray<AGraspingObject*> objs = UTask::evaluate();
	TArray<AGraspingObject*> errors;

	while (objs.Num() > 0){

		//Loop through all table_elements list checking if the obj className equals any of the table_element values
		auto head = table_elements.GetHead();
		bool found = false;
		while (head != nullptr) {
			if (objs[0]->className.Equals(head->GetValue())) {
				found = true;
				break;
			}
			head = head->GetNextNode();
		}

		if (!found) {
			errors.Add(objs[0]);
		}

		objs.RemoveAt(0);
	}


	if (UTask::currentErrors < errors.Num()) {
		UTask::totalErrors += errors.Num() - UTask::currentErrors;
	}
	UTask::currentErrors = errors.Num();

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
			FString itemString, itemString2;
			s.Split(TEXT("/"), nullptr, &itemString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			itemString.Split(TEXT("_"), nullptr, &itemString2, ESearchCase::IgnoreCase, ESearchDir::FromStart);
			items.Add(itemString2);
		}
	}

	return items;
}

bool ULayTableTask::IsTaskFinished()
{
	return false;
}
