// Fill out your copyright notice in the Description page of Project Settings.


#include "StratComPlayerController.h"

#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"

void AStratComPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
	SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
	    if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	    {
	        if (!InputMapping.IsNull())
	        {
	            InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
	        }
	    }
	}
}

