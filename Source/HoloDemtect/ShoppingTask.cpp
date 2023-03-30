// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoppingTask.h"

UShoppingTask::UShoppingTask()
{
	eval_point_info = NewObject<UQRItem>();
	eval_point_info->SetParams("/Game/EvalPointAssets/Basket", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
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
		}
		else {
			errors.Add(objs[0]);
		}
		objs.RemoveAt(0);
	}
	return errors;
}

bool UShoppingTask::IsTaskFinished()
{
	return !(bool)shopping_list.Num();
}

bool UShoppingTask::AreObjectsValid(TArray<AGraspingObject*> objs)
{
	TDoubleLinkedList<FString> objs_classes;
	for (auto* obj : objs) {
		objs_classes.AddTail(obj->className);
	}

	for (FString s : shopping_list){
		UE_LOG(LogTemp, Warning, TEXT("Shopping list: %s"), *s);
		auto* node = objs_classes.FindNode(s);
		if (node == nullptr)
			return false;

		objs_classes.RemoveNode(node);
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
			FString itemString;
			s.Split(TEXT("_"), nullptr, &itemString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

			items.Add(itemString);
		}

	}

	return items;
}
