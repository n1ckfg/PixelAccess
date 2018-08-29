#include "Pixeler.h"

APixeler::APixeler()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> finder_RenderTarget(TEXT("/PixelAccess/Textures/TestRenderTarget"));
	RenderTarget = DuplicateObject(finder_RenderTarget.Object, NULL);
	RenderTarget->InitAutoFormat(resolution, resolution);
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

	if (writeTestFile)
	{
		TArray<FVector> pixels = ReadPixels(RenderTarget);

		for (int i = 0; i<pixels.Num(); i++)
		{
			FVector p = pixels[i];

			pointsFile << p.X << ", " << p.Y << ", " << p.Z << "\n";
			pointsCounter++;
			if (pointsCounter > pointsMax) {
				pointsFile.close();
				UE_LOG(LogTemp, Warning, TEXT("~*~*~*~*~*~*~*~*~*~*~*~*~"));
				UE_LOG(LogTemp, Warning, TEXT("*~* TEST FILE WRITTEN *~*"));
				UE_LOG(LogTemp, Warning, TEXT("~*~*~*~*~*~*~*~*~*~*~*~*~"));
				writeTestFile = false;
				break;
			}
		}
	}
}

TArray<FVector> APixeler::ReadPixels(UTextureRenderTarget2D *rtex)
{
	TArray<FVector> pixels;

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
		int R = Tab2[i];
		int G = Tab2[i + 1];
		int B = Tab2[i + 2];
		//int A = Tab2[i + 3];

		pixels.Add(FVector(R, G, B));
	}

	return pixels;
}

