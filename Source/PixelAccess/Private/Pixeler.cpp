// Fill out your copyright notice in the Description page of Project Settings.

#include "Pixeler.h"

APixeler::APixeler()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> finder_RenderTarget(TEXT("/PixelAccess/Textures/TestRenderTarget"));
	RenderTarget = DuplicateObject(finder_RenderTarget.Object, NULL);
	RenderTarget->InitAutoFormat(1024, 1024);
	Camera->TextureTarget = RenderTarget;
}

void APixeler::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
	Camera->TextureTarget = RenderTarget;

	if (writeTestFile)
	{
		std::string url = std::string(TCHAR_TO_UTF8(*FPaths::ProjectDir())) + "/points.asc";
		pointsFile.open(url);
	}
}

void APixeler::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	Camera->TextureTarget = RenderTarget;

	ReadPoints();

	if (writeTestFile)
	{
		for (int i = 0; i<points.Num(); i++)
		{
			FVector p = points[i];

			pointsFile << p.X << ", " << p.Y << ", " << p.Z << "\n";
			pointsCounter++;
			if (pointsCounter > pointsMax) {
				pointsFile.close();
				UE_LOG(LogTemp, Warning, TEXT("*** TEST FILE WRITTEN ***"));
				writeTestFile = false;
				break;
			}
		}
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
	Texture2D = RenderTarget->ConstructTexture2D(this, FString("Tex2D"), EObjectFlags::RF_NoFlags);

	int xx = Texture2D->GetSizeX();
	int yy = Texture2D->GetSizeY();
	//UE_LOG(LogTemp, Warning, TEXT("Texture size: %d %d"), xx, yy);

	FTexturePlatformData *Data = Texture2D->PlatformData;
	EPixelFormat Format = Data->PixelFormat;
	//UE_LOG(LogTemp, Warning, TEXT("Pixel format: %d"), (uint8)(Format));
	// pixel format is PFloatRGBA

	int size = Data->Mips[0].BulkData.GetElementSize();
	int N = Data->Mips[0].BulkData.GetElementCount();
	//UE_LOG(LogTemp, Warning, TEXT("Number of elements: %d, size of one element: %d"), N, size);

	const void *Table = Data->Mips[0].BulkData.LockReadOnly();
	Data->Mips[0].BulkData.Unlock();

	const uint16 *Tab2 = StaticCast<const uint16*>(Table);
	// 1 pixel = 8 bytes of half-precision RGBA data; 1024 * 1024 pixels = 8 * 1024 * 1024 bytes

	for (int i = 0; i<xx; i += 1000)
	{
		for (int j = 0; j < yy; j++)
		{
			int k = 4 * (i * yy + j);
			int R = Tab2[k];
			int G = Tab2[k + 1];
			int B = Tab2[k + 2];
			int A = Tab2[k + 3];

			points.Add(FVector(R, G, B));
		}
	}
}

