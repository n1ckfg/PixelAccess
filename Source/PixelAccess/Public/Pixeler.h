#pragma once

#include "PixelAccess.h"

#include "Pixeler.generated.h"

UCLASS()
class PIXELACCESS_API APixeler : public AActor
{
	GENERATED_BODY()

public:

	APixeler();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	TArray<FVector> ReadPixels(UTextureRenderTarget2D *rtex);

	USceneCaptureComponent2D *Camera;
	UTextureRenderTarget2D *RenderTarget;

	UPROPERTY(EditAnywhere)
	int resolution = 256;

	bool writeTestFile = true;
	int pointsMax = 200000;
	int pointsCounter = 0;
	std::ofstream pointsFile;

};
