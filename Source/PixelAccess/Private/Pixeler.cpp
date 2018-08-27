// Fill out your copyright notice in the Description page of Project Settings.

#include "Pixeler.h"

APixeler::APixeler(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetAsset(TEXT("/Game/MyLittleRenderTarget"));
	//here you need to have prepared MyLittleRenderTarget asset, type RenderTarget2D. You can have one for many actors, it is duplicated. What is not resolved by me: i don't know if it is stable solution or it will make crash after many calls

	RenderTarget = DuplicateObject(RenderTargetAsset.Object, NULL);
	RenderTarget->InitAutoFormat(1024, 1024);
	Camera->TextureTarget = RenderTarget;
}

void APixeler::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);

	Camera->TextureTarget = RenderTarget;

	int X = RenderTarget->GetSurfaceHeight();
	int Y = RenderTarget->GetSurfaceWidth();
	GLog->Logf(TEXT("Size: %d %d"), X, Y);
	Texture2D = RenderTarget->ConstructTexture2D(this, FString("Tex2D"), EObjectFlags::RF_NoFlags);
	//FTextureRenderTargetResource *Resource = RenderTarget->GetRenderTargetResource();
	int xx = Texture2D->GetSizeX();
	int yy = Texture2D->GetSizeY();
	GLog->Logf(TEXT("Texture size: %d %d"), xx, yy);

	FTexturePlatformData *Data = Texture2D->PlatformData;
	EPixelFormat Format = Data->PixelFormat;
	GLog->Logf(TEXT("Pixel format: %d"), (uint8)(Format));
	//format of pixel is PFloatRGBA

	int size = Data->Mips[0].BulkData.GetElementSize();
	int N = Data->Mips[0].BulkData.GetElementCount();
	GLog->Logf(TEXT("Number of elements: %d, size of one element: %d"), N, size);
	//i've got 1 byte size of element

	const void *Table = Data->Mips[0].BulkData.LockReadOnly();
	Data->Mips[0].BulkData.Unlock();

	const uint16 *Tab2 = StaticCast<const uint16*>(Table);
	//ok, quick calculation. I get 8*1024*1024 bytes from 1024*1024 pixels, so one pixel got 8 bytes of data. Format is RGBA, so you can figure it yourself :)
	for (int i = 0; i<xx; i++)
	{
		for (int j = 0; j < yy; j++) 
		{
			int k = 4 * (i*yy + j);
			int R = Tab2[k];
			int G = Tab2[k + 1];
			int B = Tab2[k + 2];
			int A = Tab2[k + 3];
		}
	}

	if (writeTestFile)
	{
		//std::string url = std::string(TCHAR_TO_UTF8(*FPaths::ProjectDir())) + "/points.asc";
		//pointsFile.open(url);
	}
}

void APixeler::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	ReadPoints();
	WritePointData();

	if (writeTestFile)
	{
		/*
		for (int i = 0; i<LidarMeasurement.Points.Num(); i += 3)
		{
			float x = LidarMeasurement.Points[i];
			float y = LidarMeasurement.Points[i + 1];
			float z = LidarMeasurement.Points[i + 2];

			pointsFile << x << ", " << y << ", " << z << "\n";
			pointsCounter++;
			if (pointsCounter > pointsMax) {
				pointsFile.close();
				UE_LOG(LogTemp, Warning, TEXT("*** TEST FILE WRITTEN ***"));
				writeTestFile = false;
				break;
			}
		}
		*/
	}

	if (debugging)
	{
		//std::cout << LidarMeasurement.Points[0];

		// for printing to UE console
		//UE_LOG(LogTemp, Warning, TEXT("example"));
	}
}

void APixeler::ReadPoints()
{
	//
}

void APixeler::WritePointData()
{
	//
}