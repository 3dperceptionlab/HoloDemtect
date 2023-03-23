// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoppingTask.h"

UShoppingTask::UShoppingTask()
{
	eval_point_info = NewObject<UQRItem>();
	eval_point_info->SetParams("", FVector(0, 0, 0), FRotator(0, 0, 0), 1);
}

void UShoppingTask::initialize()
{
	UTask::initialize();
}

void UShoppingTask::evaluate()
{
	UTask::evaluate();
}
