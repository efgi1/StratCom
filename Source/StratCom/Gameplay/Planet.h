// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

UENUM()
enum EPlanetResourceGrade
{
	Rich	UMETA(DisplayName= "Rich"),
	Great	UMETA(DisplayName= "Great"),
	Good	UMETA(DisplayName= "Good"),
	Average UMETA(DisplayName= "Average"),
	Poor	UMETA(DisplayName= "Poor"),
	Empty	UMETA(DisplayName= "Empty")
};

class UTextRenderComponent;
class UPostProcessComponent;
class USphereComponent;

UCLASS()
class STRATCOM_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanet();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet")
	TObjectPtr<UStaticMeshComponent> PlanetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet")
	TObjectPtr<UStaticMeshComponent> HighlightMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet")
	TObjectPtr<UTextRenderComponent> InfoText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Planet")
	TEnumAsByte<EPlanetResourceGrade> Grade; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet")
	FVector2D MinMaxSlots = {3, 10};

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void HandleSelect();
	
	UFUNCTION()
	void HandleDeselect();

	FVector GetPlanetScale();

	void SetMaxSlots(uint8 NewMaxSlots);
	void SetGrade(EPlanetResourceGrade NewGrade);
		
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Planet")
	FString PlanetName = "TESTING";
private:
	
	UPROPERTY(Replicated)
	uint8 CurrentFactories = 0;
	
	UPROPERTY(Replicated)
	uint8 CurrentResearch = 0;

	UPROPERTY(Replicated)
	uint8 MaxSlots = 0;

	FVector OriginalLocation;

	UFUNCTION()
	void BeginCursorOver(AActor* Actor);

	UFUNCTION()
	void EndCursorOver(AActor* Actor);
};
