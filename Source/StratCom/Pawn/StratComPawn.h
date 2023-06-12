// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StratComPawn.generated.h"

class UInputAction;
struct FInputActionInstance;
class UCameraComponent;
class APlanet;

UCLASS()
class STRATCOM_API AStratComPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStratComPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="Player")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditAnywhere, Category="Input")
	float TranslationSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category="Input")
	float ZoomSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category="Input")
	float ScreenBorderThickness = 15.f;

private:
	void ApplyCameraTranslation(float DeltaTime);
	
	void Zoom(const FInputActionInstance& Instance);

	void HandleSelect(const FInputActionInstance& Instance);

	void HandleDrag(const FInputActionInstance& Instance);
	void EndDrag(const FInputActionInstance& Instance);

	UPROPERTY()
	TObjectPtr<APlanet> BeginDragPlanet = nullptr;

	APlanet* CheckPlanetTraceHit();
	

};
