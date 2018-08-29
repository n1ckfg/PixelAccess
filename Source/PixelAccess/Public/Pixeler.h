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
	
	bool WriteFile(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverWriting = true);
	
	TArray<FColor> ReadPixels(UTextureRenderTarget2D *rtex);
	TArray<FVector> ReadPixelFloats(UTextureRenderTarget2D *rtex);

	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D *Camera;

	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D *RenderTarget;

	UPROPERTY(EditAnywhere)
	int resolution = 256;

	UPROPERTY(EditAnywhere)
	bool floatColor = false;

	UPROPERTY(EditAnywhere)
	bool writeTestFile = false;

private:

	int pointsMax = 200000;
	int pointsCounter = 0;
	TArray<FString> output;

};
