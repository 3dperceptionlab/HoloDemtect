// Fill out your copyright notice in the Description page of Project Settings.


#include "DatasetGenerator.h"


UDatasetGenerator::UDatasetGenerator()
{

}

UDatasetGenerator::~UDatasetGenerator()
{

}

TArray<uint8> FStringToUint8(const FString& InString){
	TArray<uint8> OutBytes;

	// Handle empty strings
	if (InString.Len() > 0)
	{
		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
	}

	return OutBytes;
}

FString AddData(FString Name, FString Value, FString BoundaryBegin) {
	return FString(TEXT("\r\n"))
		+ BoundaryBegin
		+ FString(TEXT("Content-Disposition: form-data; name=\""))
		+ Name
		+ FString(TEXT("\"\r\n\r\n"))
		+ Value;
}

FString UDatasetGenerator::getTimeseriesString() {

	//FString output = "Time;RightHandObject;LeftHandObject;RightHandPos;RightHandRot;LeftHandPos;LeftHandRot;HeadPos;HeadRot;ObjectPut;TotalErrors;EyeTrackerPos;EyeTrackerDir\n";
	FString output = "";
	//recorrer el TArray de TimeseriesRow y crear un string con los valores de cada uno de los campos	
	for (auto row : timeseries) {
		FString RightHandObjectShort, RightHandObjectShort2;
		row.RightHandObject.Split(TEXT("/"), nullptr, &RightHandObjectShort, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		RightHandObjectShort.Split(TEXT("_"), nullptr, &RightHandObjectShort2, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		FString LeftHandObjectShort, LeftHandObjectShort2;
		row.LeftHandObject.Split(TEXT("/"), nullptr, &LeftHandObjectShort, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		LeftHandObjectShort.Split(TEXT("_"), nullptr, &LeftHandObjectShort2, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		FString rowData = row.time + ";" + RightHandObjectShort2 + ";" + LeftHandObjectShort2 +
			";" + row.RightHandPos.ToString() + ";" + row.RightHandRot.ToString() + ";" +
			row.LeftHandPos.ToString() + ";" + row.LeftHandRot.ToString() + ";" +
			row.HeadPos.ToString() + ";" + row.HeadRot.ToString() + ";" +
			FString::FromInt(row.ObjectPut) + ";" + FString::FromInt(row.TotalErrors) + ";"+
			row.EyeTrackerPos.ToString() + ";" + row.EyeTrackerDir.ToString() + "\n";
		output += rowData;
	}

	return output;
}

FString UDatasetGenerator::getTimeseriesRowString(int id_p){
//FString output = "Time;RightHandObject;LeftHandObject;RightHandPos;RightHandRot;LeftHandPos;LeftHandRot;HeadPos;HeadRot;ObjectPut;TotalErrors;EyeTrackerPos;EyeTrackerDir\n";
FString output = "";
//recorrer el TArray de TimeseriesRow y crear un string con los valores de cada uno de los campos	
if (timeseries.Num() > id_p) {
	auto row = timeseries[id_p];
	FString RightHandObjectShort, RightHandObjectShort2;
	row.RightHandObject.Split(TEXT("/"), nullptr, &RightHandObjectShort, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	RightHandObjectShort.Split(TEXT("_"), nullptr, &RightHandObjectShort2, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	FString LeftHandObjectShort, LeftHandObjectShort2;
	row.LeftHandObject.Split(TEXT("/"), nullptr, &LeftHandObjectShort, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	LeftHandObjectShort.Split(TEXT("_"), nullptr, &LeftHandObjectShort2, ESearchCase::IgnoreCase, ESearchDir::FromStart);

	FString rowData = row.time + ";" + RightHandObjectShort2 + ";" + LeftHandObjectShort2 +
		";" + row.RightHandPos.ToString() + ";" + row.RightHandRot.ToString() + ";" +
		row.LeftHandPos.ToString() + ";" + row.LeftHandRot.ToString() + ";" +
		row.HeadPos.ToString() + ";" + row.HeadRot.ToString() + ";" +
		FString::FromInt(row.ObjectPut) + ";" + FString::FromInt(row.TotalErrors) + ";" +
		row.EyeTrackerPos.ToString() + ";" + row.EyeTrackerDir.ToString() + "\n";
	output += rowData;
}

return output;
}

FString UDatasetGenerator::getRowSummaryString() {

	//Get pawn and cast to MRPawn

	FString output = FString::FromInt(id) + ";" + task_type + ";" + total_time + ";None;"+ FString::FromInt(timeseries.Last().TotalErrors) + ";" + FString::FromInt(total_items) + ";visual_evaluation;eval_id";

	return output;

}

/*void UDatasetGenerator::addImage(UTextureRenderTarget2D* TextureRenderTarget) {
	FBufferArchive Buffer;
	FImageUtils::ExportRenderTarget2DAsPNG(TextureRenderTarget, Buffer);
	images.Add(Buffer);
}*/
void UDatasetGenerator::addImage(UTextureRenderTarget2D* TextureRenderTarget) {

	images.Add(TextureRenderTarget);
}


bool UDatasetGenerator::sendTimeseries(){

	if (timeseries.Num() <= 0)
		return false;


	semaforo = false;

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();
	// We set the api URL
	FString URLEndpoint = FString(SERVER_ENDPOINT) + FString("/time_series");
	HttpRequest->SetURL(URLEndpoint);
	//HttpRequest->SetURL(URLEndpoint);
	HttpRequest->SetVerb(TEXT("POST"));

	// Create a boundary label, for the header
	FString BoundaryLabel = FString(TEXT("e543322540af456f9a3773049ca02529-")) + FString::FromInt(FMath::Rand());
	// boundary label for begining of every payload chunk 
	FString BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));
	// boundary label for the end of payload
	FString BoundaryEnd = FString(TEXT("\r\n--")) + BoundaryLabel + FString(TEXT("--\r\n"));

	// Set the content-type for server to know what are we going to send
	HttpRequest->SetHeader(TEXT("Content-Type"), FString(TEXT("multipart/form-data; boundary=")) + BoundaryLabel);

	// This is binary content of the request
	TArray<uint8> CombinedContent;
	TArray<uint8> FileRawData;



	CombinedContent.Append(FStringToUint8(AddData("id", FString::FromInt(id), BoundaryBegin)));
	CombinedContent.Append(FStringToUint8(AddData("row", getTimeseriesRowString(0), BoundaryBegin)));
	FBufferArchive Buffer;
	FImageUtils::ExportRenderTarget2DAsPNG(images[0], Buffer);
	CombinedContent.Append(FStringToUint8(AddData("img", FBase64::Encode(Buffer), BoundaryBegin)));

	// Finally, add a boundary at the end of the payload
	CombinedContent.Append(FStringToUint8(BoundaryEnd));

	// Set the request content
	HttpRequest->SetContent(CombinedContent);


	// Hook a lambda(anonymous function) to when we receive a response
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UDatasetGenerator::OnResponseTimeseriesReceived);

	/*HttpRequest->OnProcessRequestComplete().BindLambda(
		[this](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool connectedSuccessfully) mutable {
				AHTTPInterface::OnResponseReceived(pRequest, pResponse, connectedSuccessfully);
		});*/

		// Send the request 
	HttpRequest->ProcessRequest();




	return true;
}


bool UDatasetGenerator::sendSummaryRow()
{

	if (id == -1) {
		return false;
	}

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();
	// We set the api URL
	FString URLEndpoint = FString(SERVER_ENDPOINT) + FString("/summary");
	HttpRequest->SetURL(URLEndpoint);
	//HttpRequest->SetURL(URLEndpoint);
	HttpRequest->SetVerb(TEXT("POST"));

	// Create a boundary label, for the header
	FString BoundaryLabel = FString(TEXT("e543322540af456f9a3773049ca02529-")) + FString::FromInt(FMath::Rand());
	// boundary label for begining of every payload chunk 
	FString BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));
	// boundary label for the end of payload
	FString BoundaryEnd = FString(TEXT("\r\n--")) + BoundaryLabel + FString(TEXT("--\r\n"));

	// Set the content-type for server to know what are we going to send
	HttpRequest->SetHeader(TEXT("Content-Type"), FString(TEXT("multipart/form-data; boundary=")) + BoundaryLabel);

	// This is binary content of the request
	TArray<uint8> CombinedContent;
	TArray<uint8> FileRawData;
	FBufferArchive Buffer;

	CombinedContent.Append(FStringToUint8(AddData("id", FString::FromInt(id), BoundaryBegin)));
	CombinedContent.Append(FStringToUint8(AddData("row", getRowSummaryString(), BoundaryBegin)));

	// Finally, add a boundary at the end of the payload
	CombinedContent.Append(FStringToUint8(BoundaryEnd));

	// Set the request content
	HttpRequest->SetContent(CombinedContent);


	// Hook a lambda(anonymous function) to when we receive a response
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UDatasetGenerator::OnResponseSummaryReceived);

	/*HttpRequest->OnProcessRequestComplete().BindLambda(
		[this](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool connectedSuccessfully) mutable {
				AHTTPInterface::OnResponseReceived(pRequest, pResponse, connectedSuccessfully);
		});*/

		// Send the request 
	HttpRequest->ProcessRequest();




	return true;
}


void UDatasetGenerator::OnResponseTimeseriesReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	UE_LOG(LogTemp, Display, TEXT("Connection."));

	if (connectedSuccessfully) {

		UE_LOG(LogTemp, Display, TEXT("Response: %s"), *pResponse->GetContentAsString());
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Response: %s"), *pResponse->GetContentAsString()));
		}
		TSharedPtr<FJsonValue> JsonValue;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(pResponse->GetContentAsString());
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
			FString IdString = JsonValue->AsObject()->GetStringField("id");
			
			if (FCString::Atoi(*IdString) > -1){

				id = FCString::Atoi(*IdString);
				timeseries.RemoveAt(0);
				images.RemoveAt(0);
				semaforo = true;
				//sendSummaryRow();
		
			}else {
				//Error
				UE_LOG(LogTemp, Error, TEXT("ID error %d"), id);
				PSCREENE("ID Error");
		
			}

			
		}

	}
	else {
		switch (pRequest->GetStatus()) {
		case EHttpRequestStatus::Failed_ConnectionError:
			UE_LOG(LogTemp, Error, TEXT("Connection failed."));
			PSCREENE("Connection failed.");
		default:
			UE_LOG(LogTemp, Error, TEXT("Request failed."));
			PSCREENE("Request failed.");
		}

	}
}

void UDatasetGenerator::OnResponseSummaryReceived(FHttpRequestPtr pRequest, FHttpResponsePtr pResponse, bool connectedSuccessfully)
{
	UE_LOG(LogTemp, Display, TEXT("Connection."));

	if (connectedSuccessfully) {

		UE_LOG(LogTemp, Display, TEXT("Response: %s"), *pResponse->GetContentAsString());
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Response: %s"), *pResponse->GetContentAsString()));
		}

	}
	else {
		switch (pRequest->GetStatus()) {
		case EHttpRequestStatus::Failed_ConnectionError:
			UE_LOG(LogTemp, Error, TEXT("Connection failed."));
			PSCREENE("Connection failed.");
		default:
			UE_LOG(LogTemp, Error, TEXT("Request failed."));
			PSCREENE("Request failed.");
		}

	}

}

void UDatasetGenerator::StartCapture() {
	threadGen = new FThreadDataset(this);
	runnableThread = FRunnableThread::Create(threadGen, TEXT("DatasetThread"));
}

void UDatasetGenerator::StopCapture() {
	if (threadGen != nullptr)
	{
		threadGen->Stop();
		runnableThread->WaitForCompletion();
		delete threadGen;
		threadGen = nullptr;
	}
}