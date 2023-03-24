// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoppingTask.h"

UShoppingTask::UShoppingTask()
{
	eval_point_info = NewObject<UQRItem>();
	eval_point_info->SetParams("/Game/YCB/16K/009_gelatin_box/009_gelatin_box", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
	bbox_size.X = 1;
	bbox_size.Y = 1;
	bbox_size.Z = 1;

	shopping_list.Enqueue("/Game/YCB/16K/011_banana/011_banana");
	shopping_list.Enqueue("/Game/YCB/16K/017_orange/017_orange");
}

void UShoppingTask::initialize(TArray<AGraspingObject*> SpawnedObjects_, AGraspingObject* evaluation_point_, FTaskInfo taskInfo_)
{
	UTask::initialize(SpawnedObjects_, evaluation_point_, taskInfo_);

	for (FString s : taskInfo.items)
	{
		shopping_list.Enqueue(s);
	}
}

TArray<AGraspingObject*> UShoppingTask::evaluate()
{
	TArray<AGraspingObject*> objs = UTask::evaluate();
	TArray<AGraspingObject*> errors;

	while (objs.Num() > 0)
	{
		if (objs[0]->className.Equals(*shopping_list.Peek()))
		{
			objs[0]->Destroy();
			shopping_list.Pop();
			// TODO: comprobar que el objeto ya no esta agarrado
		}
		else {
			errors.Add(objs[0]);
		}
		objs.RemoveAt(0);
	}
	return errors;
}
