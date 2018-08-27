// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "ConstructorHelpers.h"

#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>

// https://answers.unrealengine.com/questions/25594/accessing-pixel-values-of-texture2d.html#answer-73443

class FPixelAccessModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};