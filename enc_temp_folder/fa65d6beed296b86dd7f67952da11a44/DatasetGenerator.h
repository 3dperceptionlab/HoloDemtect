// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpResponse.h"
#include "HTTPModule.h"
#include <string>
#include <chrono>
#include "Kismet/GameplayStatics.h"
#include "ImageUtils.h"
#include "Serialization/BufferArchive.h"
#include "DatasetGenerator.generated.h"

#define PSCREEN(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT(x));}
#define PSCREENE(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(x));}
#define SERVER_ENDPOINT "http://clarke:5000"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTimeseriesRow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FString time;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FString RightHandObject;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FString LeftHandObject;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector RightHandPos;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FRotator RightHandRot;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector LeftHandPos;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FRotator LeftHandRot;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector HeadPos;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FRotator HeadRot;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		int32 ObjectPut;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		int32 TotalErrors;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector EyeTrackerPos;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FVector EyeTrackerDir;

};

UCLASS(BlueprintType)
class HOLODEMTECT_API UDatasetGenerator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray< FTimeseriesRow> timeseries;
		TArray<FBufferArchive> images;


	int id=-1;
	bool semaforo = true;
	FString task_type = "";
	FString task_name = "";
	FString total_time = "";
	int total_items = 0;


	UDatasetGenerator();
	~UDatasetGenerator();

	UFUNCTION(BlueprintCallable)
		bool sendTimeseries();
	UFUNCTION(BlueprintCallable)
		bool sendSummaryRow();
	UFUNCTION(BlueprintCallable)
		void setSummaryVariables(FString task_type_p, FString task_name_p, FString total_time_p, int total_items_p) { task_type = task_type_p; task_name = task_name_p; total_time = total_time_p; total_items = total_items_p; }
	UFUNCTION(BlueprintCallable)
		void addImage(UTextureRenderTarget2D* TextureRenderTarget);


	FString getTimeseriesString();
	FString getTimeseriesRowString(int id);
	FString getRowSummaryString();
	void OnResponseTimeseriesReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);
	void OnResponseSummaryReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully);

};
