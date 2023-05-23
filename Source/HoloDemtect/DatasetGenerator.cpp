// Fill out your copyright notice in the Description page of Project Settings.


#include "DatasetGenerator.h"
#include "ImageUtils.h"
#include "RHI.h"
#include "Engine/Texture2D.h"
#include "Misc/ObjectThumbnail.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CubemapUnwrapUtils.h"
#include "Logging/MessageLog.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "DDSLoader.h"
#include "HDRLoader.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"

FPixelFormatInfo	GPixelFormatsLocal[PF_MAX] =
{
	// Name						BlockSizeX	BlockSizeY	BlockSizeZ	BlockBytes	NumComponents	PlatformFormat	Supported		UnrealFormat

	{ TEXT("unknown"),			0,			0,			0,			0,			0,				0,				0,				PF_Unknown			},
	{ TEXT("A32B32G32R32F"),	1,			1,			1,			16,			4,				0,				1,				PF_A32B32G32R32F	},
	{ TEXT("B8G8R8A8"),			1,			1,			1,			4,			4,				0,				1,				PF_B8G8R8A8			},
	{ TEXT("G8"),				1,			1,			1,			1,			1,				0,				1,				PF_G8				},
	{ TEXT("G16"),				1,			1,			1,			2,			1,				0,				1,				PF_G16				},
	{ TEXT("DXT1"),				4,			4,			1,			8,			3,				0,				1,				PF_DXT1				},
	{ TEXT("DXT3"),				4,			4,			1,			16,			4,				0,				1,				PF_DXT3				},
	{ TEXT("DXT5"),				4,			4,			1,			16,			4,				0,				1,				PF_DXT5				},
	{ TEXT("UYVY"),				2,			1,			1,			4,			4,				0,				0,				PF_UYVY				},
	{ TEXT("FloatRGB"),			1,			1,			1,			4,			3,				0,				1,				PF_FloatRGB			},
	{ TEXT("FloatRGBA"),		1,			1,			1,			8,			4,				0,				1,				PF_FloatRGBA		},
	{ TEXT("DepthStencil"),		1,			1,			1,			4,			1,				0,				0,				PF_DepthStencil		},
	{ TEXT("ShadowDepth"),		1,			1,			1,			4,			1,				0,				0,				PF_ShadowDepth		},
	{ TEXT("R32_FLOAT"),		1,			1,			1,			4,			1,				0,				1,				PF_R32_FLOAT		},
	{ TEXT("G16R16"),			1,			1,			1,			4,			2,				0,				1,				PF_G16R16			},
	{ TEXT("G16R16F"),			1,			1,			1,			4,			2,				0,				1,				PF_G16R16F			},
	{ TEXT("G16R16F_FILTER"),	1,			1,			1,			4,			2,				0,				1,				PF_G16R16F_FILTER	},
	{ TEXT("G32R32F"),			1,			1,			1,			8,			2,				0,				1,				PF_G32R32F			},
	{ TEXT("A2B10G10R10"),      1,          1,          1,          4,          4,              0,              1,				PF_A2B10G10R10		},
	{ TEXT("A16B16G16R16"),		1,			1,			1,			8,			4,				0,				1,				PF_A16B16G16R16		},
	{ TEXT("D24"),				1,			1,			1,			4,			1,				0,				1,				PF_D24				},
	{ TEXT("PF_R16F"),			1,			1,			1,			2,			1,				0,				1,				PF_R16F				},
	{ TEXT("PF_R16F_FILTER"),	1,			1,			1,			2,			1,				0,				1,				PF_R16F_FILTER		},
	{ TEXT("BC5"),				4,			4,			1,			16,			2,				0,				1,				PF_BC5				},
	{ TEXT("V8U8"),				1,			1,			1,			2,			2,				0,				1,				PF_V8U8				},
	{ TEXT("A1"),				1,			1,			1,			1,			1,				0,				0,				PF_A1				},
	{ TEXT("FloatR11G11B10"),	1,			1,			1,			4,			3,				0,				0,				PF_FloatR11G11B10	},
	{ TEXT("A8"),				1,			1,			1,			1,			1,				0,				1,				PF_A8				},
	{ TEXT("R32_UINT"),			1,			1,			1,			4,			1,				0,				1,				PF_R32_UINT			},
	{ TEXT("R32_SINT"),			1,			1,			1,			4,			1,				0,				1,				PF_R32_SINT			},

	// IOS Support
	{ TEXT("PVRTC2"),			8,			4,			1,			8,			4,				0,				0,				PF_PVRTC2			},
	{ TEXT("PVRTC4"),			4,			4,			1,			8,			4,				0,				0,				PF_PVRTC4			},

	{ TEXT("R16_UINT"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_UINT			},
	{ TEXT("R16_SINT"),			1,			1,			1,			2,			1,				0,				1,				PF_R16_SINT			},
	{ TEXT("R16G16B16A16_UINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_UINT},
	{ TEXT("R16G16B16A16_SINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_SINT},
	{ TEXT("R5G6B5_UNORM"),     1,          1,          1,          2,          3,              0,              1,              PF_R5G6B5_UNORM		},
	{ TEXT("R8G8B8A8"),			1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8			},
	{ TEXT("A8R8G8B8"),			1,			1,			1,			4,			4,				0,				1,				PF_A8R8G8B8			},
	{ TEXT("BC4"),				4,			4,			1,			8,			1,				0,				1,				PF_BC4				},
	{ TEXT("R8G8"),				1,			1,			1,			2,			2,				0,				1,				PF_R8G8				},

	{ TEXT("ATC_RGB"),			4,			4,			1,			8,			3,				0,				0,				PF_ATC_RGB			},
	{ TEXT("ATC_RGBA_E"),		4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_E		},
	{ TEXT("ATC_RGBA_I"),		4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_I		},
	{ TEXT("X24_G8"),			1,			1,			1,			1,			1,				0,				0,				PF_X24_G8			},
	{ TEXT("ETC1"),				4,			4,			1,			8,			3,				0,				0,				PF_ETC1				},
	{ TEXT("ETC2_RGB"),			4,			4,			1,			8,			3,				0,				0,				PF_ETC2_RGB			},
	{ TEXT("ETC2_RGBA"),		4,			4,			1,			16,			4,				0,				0,				PF_ETC2_RGBA		},
	{ TEXT("PF_R32G32B32A32_UINT"),1,		1,			1,			16,			4,				0,				1,				PF_R32G32B32A32_UINT},
	{ TEXT("PF_R16G16_UINT"),	1,			1,			1,			4,			4,				0,				1,				PF_R16G16_UINT},

	// ASTC support
	{ TEXT("ASTC_4x4"),			4,			4,			1,			16,			4,				0,				0,				PF_ASTC_4x4			},
	{ TEXT("ASTC_6x6"),			6,			6,			1,			16,			4,				0,				0,				PF_ASTC_6x6			},
	{ TEXT("ASTC_8x8"),			8,			8,			1,			16,			4,				0,				0,				PF_ASTC_8x8			},
	{ TEXT("ASTC_10x10"),		10,			10,			1,			16,			4,				0,				0,				PF_ASTC_10x10		},
	{ TEXT("ASTC_12x12"),		12,			12,			1,			16,			4,				0,				0,				PF_ASTC_12x12		},

	{ TEXT("BC6H"),				4,			4,			1,			16,			3,				0,				1,				PF_BC6H				},
	{ TEXT("BC7"),				4,			4,			1,			16,			4,				0,				1,				PF_BC7				},
	{ TEXT("R8_UINT"),			1,			1,			1,			1,			1,				0,				1,				PF_R8_UINT			},
	{ TEXT("L8"),				1,			1,			1,			1,			1,				0,				0,				PF_L8				},
	{ TEXT("XGXR8"),			1,			1,			1,			4,			4,				0,				1,				PF_XGXR8 			},
	{ TEXT("R8G8B8A8_UINT"),	1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_UINT	},
	{ TEXT("R8G8B8A8_SNORM"),	1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_SNORM	},

	{ TEXT("R16G16B16A16_UINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_UNORM },
	{ TEXT("R16G16B16A16_SINT"),1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_SNORM },
	{ TEXT("PLATFORM_HDR_0"),	0,			0,			0,			0,			0,				0,				0,				PF_PLATFORM_HDR_0   },
	{ TEXT("PLATFORM_HDR_1"),	0,			0,			0,			0,			0,				0,				0,				PF_PLATFORM_HDR_1   },
	{ TEXT("PLATFORM_HDR_2"),	0,			0,			0,			0,			0,				0,				0,				PF_PLATFORM_HDR_2   },

	// NV12 contains 2 textures: R8 luminance plane followed by R8G8 1/4 size chrominance plane.
	// BlockSize/BlockBytes/NumComponents values don't make much sense for this format, so set them all to one.
	{ TEXT("NV12"),				1,			1,			1,			1,			1,				0,				0,				PF_NV12             },

	{ TEXT("PF_R32G32_UINT"),   1,   		1,			1,			8,			2,				0,				1,				PF_R32G32_UINT      },

	{ TEXT("PF_ETC2_R11_EAC"),  4,   		4,			1,			8,			1,				0,				0,				PF_ETC2_R11_EAC     },
	{ TEXT("PF_ETC2_RG11_EAC"), 4,   		4,			1,			16,			2,				0,				0,				PF_ETC2_RG11_EAC    },
	{ TEXT("R8"),				1,			1,			1,			1,			1,				0,				1,				PF_R8				},
};


SIZE_T CalculateImageBytesLocal(uint32 SizeX, uint32 SizeY, uint32 SizeZ, uint8 Format)
{
	if (Format == PF_A1)
	{
		// The number of bytes needed to store all 1 bit pixels in a line is the width of the image divided by the number of bits in a byte
		uint32 BytesPerLine = SizeX / 8;
		// The number of actual bytes in a 1 bit image is the bytes per line of pixels times the number of lines
		return sizeof(uint8) * BytesPerLine * SizeY;
	}
	else if (SizeZ > 0)
	{
		return static_cast<SIZE_T>(SizeX / GPixelFormatsLocal[Format].BlockSizeX) * (SizeY / GPixelFormatsLocal[Format].BlockSizeY) * (SizeZ / GPixelFormatsLocal[Format].BlockSizeZ) * GPixelFormatsLocal[Format].BlockBytes;
	}
	else
	{
		return static_cast<SIZE_T>(SizeX / GPixelFormatsLocal[Format].BlockSizeX) * (SizeY / GPixelFormatsLocal[Format].BlockSizeY) * GPixelFormatsLocal[Format].BlockBytes;
	}
}

static bool GetRawDataLocal(UTextureRenderTarget2D* TexRT, TArray64<uint8>& RawData){

	FRenderTarget* RenderTarget = TexRT->GameThread_GetRenderTargetResource();
	EPixelFormat Format = TexRT->GetFormat();

	int32 ImageBytes = CalculateImageBytesLocal(TexRT->SizeX, TexRT->SizeY, 0, Format);
	RawData.AddUninitialized(ImageBytes);
	bool bReadSuccess = false;
	switch (Format){
	case PF_FloatRGBA:
	{
		TArray<FFloat16Color> FloatColors;
		bReadSuccess = RenderTarget->ReadFloat16Pixels(FloatColors);
		FMemory::Memcpy(RawData.GetData(), FloatColors.GetData(), ImageBytes);
	}
	break;
	case PF_B8G8R8A8:
		bReadSuccess = RenderTarget->ReadPixelsPtr((FColor*)RawData.GetData());
		break;
	}
	if (bReadSuccess == false)
	{
		RawData.Empty();
	}
	return bReadSuccess;
}


void FromRawDataToPNG(TArray64<uint8> RawData, FArchive& Ar) {

	IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

	TSharedPtr<IImageWrapper> PNGImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	//PNGImageWrapper->SetRaw(RawData.GetData(), RawData.GetAllocatedSize(), Size.X, Size.Y, ERGBFormat::BGRA, 8);
	PNGImageWrapper->SetRaw(RawData.GetData(), RawData.GetAllocatedSize(), 1504, 846, ERGBFormat::BGRA, 8);

	const TArray64<uint8>& PNGData = PNGImageWrapper->GetCompressed(100);

	Ar.Serialize((void*)PNGData.GetData(), PNGData.GetAllocatedSize());

}

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

	//FString output = FString::FromInt(id) + ";" + task_type + ";" + total_time + ";None;"+ FString::FromInt(timeseries.Last().TotalErrors) + ";" + FString::FromInt(total_items) + ";visual_evaluation;eval_id";
	FString output = FString::FromInt(id) + ";" + task_type + ";" + total_time + ";None;TotalErrors;" + FString::FromInt(total_items) + ";visual_evaluation;eval_id";

	return output;

}

void UDatasetGenerator::addImage(UTextureRenderTarget2D* TextureRenderTarget) {
	/*FBufferArchive Buffer;
	//Calculate time runing the function
	auto start = std::chrono::steady_clock::now();
	FImageUtils::ExportRenderTarget2DAsPNG(TextureRenderTarget, Buffer);
	auto end = std::chrono::steady_clock::now();
	float time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	UE_LOG(LogTemp, Warning, TEXT("Time to export: %f"), time);
	images.Add(Buffer);*/
}
/*void UDatasetGenerator::addImage(UTextureRenderTarget2D* TextureRenderTarget) {

	images.Add(TextureRenderTarget);
}*/


bool UDatasetGenerator::addRawRenderTarget(UTextureRenderTarget2D* TextureRenderTarget) {
	bool bSuccess = false;
	if (TextureRenderTarget->GetFormat() == PF_B8G8R8A8) {
		check(TextureRenderTarget != nullptr);
		FRenderTarget* RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
		FIntPoint Size = RenderTarget->GetSizeXY();
		//UE_LOG(LogTemp, Warning, TEXT("Render Size: %i ; %i"), Size.X, Size.Y);
		TArray64<uint8> RawData;
		bSuccess = GetRawDataLocal(TextureRenderTarget, RawData);
		rawImages.Add(RawData);
	}
	return bSuccess;
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
	if (rawImages.Num() > 0) {
		//FBufferArchive Buffer;
		//FImageUtils::ExportRenderTarget2DAsPNG(images[0], Buffer);

		FBufferArchive Buffer;
		FromRawDataToPNG(rawImages[0], Buffer);

		CombinedContent.Append(FStringToUint8(AddData("img", FBase64::Encode(Buffer), BoundaryBegin)));
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("No images in array"));
	}
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
			
			if (FCString::Atoi(*IdString) > -1) {

				id = FCString::Atoi(*IdString);
				if (timeseries.Num() > 0)
					timeseries.RemoveAt(0);
				if(images.Num() > 1)
					images.RemoveAt(0);
				if (rawImages.Num() > 1)
					rawImages.RemoveAt(0);
				
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