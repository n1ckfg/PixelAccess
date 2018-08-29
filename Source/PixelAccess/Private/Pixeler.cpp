#include "Pixeler.h"

APixeler::APixeler()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	
	if (Camera == nullptr) 
	{
		Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	}
	
	if (RenderTarget == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> finder_RenderTarget(TEXT("/PixelAccess/Textures/TestRenderTarget"));
		RenderTarget = DuplicateObject(finder_RenderTarget.Object, NULL);
		RenderTarget->InitAutoFormat(resolution, resolution);
	}

	Camera->TextureTarget = RenderTarget;
}

void APixeler::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
	Camera->TextureTarget = RenderTarget;
}

void APixeler::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	Camera->TextureTarget = RenderTarget;

	if (writeTestFile)
	{
		bool doWrite = false;

		if (floatColor)
		{
			TArray<FVector> pixelFloats = ReadPixelFloats(RenderTarget);

			for (int i = 0; i<pixelFloats.Num(); i++)
			{
				FVector p = pixelFloats[i];

				output.Add(FString() + FString::SanitizeFloat(p.X) + ", " + FString::SanitizeFloat(p.Y) + ", " + FString::SanitizeFloat(p.Z));
				pointsCounter++;
				if (pointsCounter > pointsMax) 
				{
					doWrite = true;
					break;
				}
			}
		}
		else 
		{
			TArray<FColor> pixels = ReadPixels(RenderTarget);

			for (int i = 0; i<pixels.Num(); i++)
			{
				FColor c = pixels[i];
				FVector p = FVector(c.R, c.G, c.B) / 255.0;

				output.Add(FString() + FString::SanitizeFloat(p.X) + ", " + FString::SanitizeFloat(p.Y) + ", " + FString::SanitizeFloat(p.Z));
				pointsCounter++;
				if (pointsCounter > pointsMax) 
				{
					doWrite = true;
					break;
				}
			}
		}

		if (doWrite)
		{
			WriteFile(FPaths::ProjectDir(), "points.asc", FString::Join(output, TEXT("\n")), true);
			
			UE_LOG(LogTemp, Warning, TEXT("~*~*~*~*~*~*~*~*~*~*~*~*~"));
			UE_LOG(LogTemp, Warning, TEXT("*~* TEST FILE WRITTEN *~*"));
			UE_LOG(LogTemp, Warning, TEXT("~*~*~*~*~*~*~*~*~*~*~*~*~"));
			
			writeTestFile = false;
		}
	}
}

// Faster method but you're limited to 8bpc RGBA
TArray<FColor> APixeler::ReadPixels(UTextureRenderTarget2D *rtex)
{
	TArray<FColor> pixels;

     auto RenderTarget = rtex->GameThread_GetRenderTargetResource();
     RenderTarget->ReadPixels(pixels);

	return pixels;
}

// Slower method but you can specify channels and bpc
TArray<FVector> APixeler::ReadPixelFloats(UTextureRenderTarget2D *rtex)
{
	TArray<FVector> pixelFloats;

	UTexture2D *Texture2D = rtex->ConstructTexture2D(this, FString("Tex2D"), EObjectFlags::RF_NoFlags);

	//int xx = Texture2D->GetSizeX();
	//int yy = Texture2D->GetSizeY();
	//UE_LOG(LogTemp, Warning, TEXT("Texture size: %d %d"), xx, yy);

	FTexturePlatformData *Data = Texture2D->PlatformData;
	//EPixelFormat Format = Data->PixelFormat;
	//UE_LOG(LogTemp, Warning, TEXT("Pixel format: %d"), (uint8)(Format));

	//int size = Data->Mips[0].BulkData.GetElementSize();
	int N = Data->Mips[0].BulkData.GetElementCount();
	//UE_LOG(LogTemp, Warning, TEXT("Number of elements: %d, size of one element: %d"), N, size);

	const void *Table = Data->Mips[0].BulkData.LockReadOnly();
	Data->Mips[0].BulkData.Unlock();

	const uint16 *Tab2 = StaticCast<const uint16*>(Table);
	// 1 pixel = 8 bytes of half-precision RGBA data; 1024 * 1024 pixels = 8 * 1024 * 1024 bytes

	for (int i = 0; i<N; i+=4) {
		float R = Tab2[i];
		float G = Tab2[i + 1];
		float B = Tab2[i + 2];
		//float A = Tab2[i + 3];

		pixelFloats.Add(FVector(R, G, B) / 65535.0);
	}

	return pixelFloats;
}

bool APixeler::WriteFile(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverWriting)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (SaveDirectory.Len() > 0)
	{
		FPaths::NormalizeDirectoryName(SaveDirectory);
		FPaths::NormalizeFilename(FileName);

		if (!PlatformFile.DirectoryExists(*SaveDirectory))
		{
			PlatformFile.CreateDirectory(*SaveDirectory);
			if (!PlatformFile.DirectoryExists(*SaveDirectory))
			{
				return false; // Failed to create the directory.
			}
		}

		SaveDirectory += "/" + FileName;
	}
	else
	{
		SaveDirectory = FPaths::ProjectDir() + "/" + FileName;
	}


	if (!AllowOverWriting)
	{
		if (PlatformFile.FileExists(*SaveDirectory))
		{
			return false; // Won't overwrite the file.
		}
	}

	return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory);
}