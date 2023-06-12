// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StratComPlayerController.generated.h"


class UInputMappingContext;

/**
 * 
 */
UCLASS()
class STRATCOM_API AStratComPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
};
