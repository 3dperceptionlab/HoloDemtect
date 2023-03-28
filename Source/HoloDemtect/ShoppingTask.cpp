// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoppingTask.h"

UShoppingTask::UShoppingTask()
{
	eval_point_info = NewObject<UQRItem>();
	eval_point_info->SetParams("/Game/EvalPointAssets/Basket", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
	bbox_size.X = 50;
	bbox_size.Y = 50;
	bbox_size.Z = 10;
}

void UShoppingTask::initialize(TArray<AGraspingObject*> SpawnedObjects_, AGraspingObject* evaluation_point_, FTaskInfo taskInfo_)
{
	UTask::initialize(SpawnedObjects_, evaluation_point_, taskInfo_);

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
		UE_LOG(LogTemp, Warning, TEXT("Obj: %s"), *objs[0]->className);
		UE_LOG(LogTemp, Warning, TEXT("Queue: %s"), *head->GetValue());

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
	return (bool)shopping_list.Num();
}

bool UShoppingTask::AreObjectsValid(TArray<AGraspingObject*> objs)
{
	TDoubleLinkedList<FString> objs_classes;
	for (auto* obj : objs) {
		objs_classes.AddTail(obj->className);
	}

	for (FString s : shopping_list)
	{
		if (!objs_classes.Contains(s))
			return false;
		objs_classes.RemoveNode(s);
	}
	return true;
}
