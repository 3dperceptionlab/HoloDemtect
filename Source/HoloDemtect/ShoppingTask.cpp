// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoppingTask.h"

UShoppingTask::UShoppingTask()
{
	eval_point_info = FQRItem();
	eval_point_info.SetParams("/Game/EvalPointAssets/Basket", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
}

void UShoppingTask::initialize(AGraspingObject* evaluation_point_, FTaskInfo taskInfo_)
{
	UTask::initialize(evaluation_point_, taskInfo_);

	for (FString s : taskInfo.items)
	{
		shopping_list.AddTail(s);
	}
}

TArray<AGraspingObject*> UShoppingTask::evaluate()
{
	TArray<AGraspingObject*> objs = UTask::evaluate();
	TArray<AGraspingObject*> errors;

	while (objs.Num() > 0)
	{
		auto head = shopping_list.GetHead();

		if (objs[0]->className.Equals(head->GetValue()))
		{
			objs[0]->Destroy();
			shopping_list.RemoveNode(head);
			SpawnedObjects.Remove(objs[0]);
		}else {
			errors.Add(objs[0]);
		}
		objs.RemoveAt(0);
	}


	if (UTask::currentErrors < errors.Num()) {
		UTask::totalErrors += errors.Num() - UTask::currentErrors;
	}
	UTask::currentErrors = errors.Num();

	return errors;
}

bool UShoppingTask::IsTaskFinished()
{
	return !(bool)shopping_list.Num();
}

bool UShoppingTask::AreObjectsValid(TArray<AGraspingObject*> objs)
{
	TArray<FString> objs_classes;
	for (auto* obj : objs) {
		objs_classes.Add(obj->className);
	}

	TArray<FString> objs_names;
	for (auto* obj : objs) {
		objs_names.Add(obj->GetName());
	}



	// Print all elements of the list objs_classes

	for (FString s : shopping_list){
		UE_LOG(LogTemp, Warning, TEXT("Shopping list: %s"), *s);
		UE_LOG(LogTemp, Warning, TEXT("Objs_classes: %s"), *(FString::Join(objs_classes, TEXT(", "))));
		UE_LOG(LogTemp, Warning, TEXT("Objs_names: %s"), *(FString::Join(objs_names, TEXT(", "))));

		int a = objs_classes.Find(s);
		if (a == INDEX_NONE) {
			int b = objs_names.Find(s);
			if (b == INDEX_NONE) {
				return false;
			}

		}

		//objs_classes.Remove(s);
	}

	SpawnedObjects = objs;
	return true;
}

TArray<FString> UShoppingTask::GetTaskItems(TArray<AGraspingObject*> objs)
{
	TArray<FString> items;

	TDoubleLinkedList<FString> objs_classes;

	for (auto* obj : objs) {
		objs_classes.AddTail(obj->className);
	}

	for (FString s : shopping_list){
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
