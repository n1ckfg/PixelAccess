// Fill out your copyright notice in the Description page of Project Settings.

#include "Pixeler.h"

APixeler::APixeler(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APixeler::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);

	if (writeTestFile)
	{
		std::string url = std::string(TCHAR_TO_UTF8(*FPaths::ProjectDir())) + "/points.asc";
		pointsFile.open(url);
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