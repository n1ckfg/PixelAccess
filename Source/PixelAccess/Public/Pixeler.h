#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

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

	void ReadPoints();

	USceneCaptureComponent2D *Camera;
	UTextureRenderTarget2D *RenderTarget;
	UTexture2D *Texture2D;

	bool debugging = false;
	bool drawRays = true;
	bool writeTestFile = true;
	int pointsMax = 200000;
	int pointsCounter = 0;
	std::ofstream pointsFile;
	TArray<FVector> points; 

};
