// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"

#include "Components/PostProcessComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "StratCom/Player/StratComPlayerState.h"

class AStratComPlayerState;
// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanetMesh"));
	PlanetMesh->SetupAttachment(RootComponent);

	HighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMesh"));
	HighlightMesh->SetupAttachment(RootComponent);

	InfoText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("InfoText"));
	InfoText->SetupAttachment(RootComponent);
	bReplicates = true;
	

}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();

	OnBeginCursorOver.AddUniqueDynamic(this, &APlanet::BeginCursorOver);
	OnEndCursorOver.AddUniqueDynamic(this, &APlanet::EndCursorOver);

	if (HasAuthority())
	{
		MaxSlots = FMath::RandRange(MinMaxSlots.X, MinMaxSlots.Y);
		Grade = static_cast<EPlanetResourceGrade>(FMath::RandRange(0, 5));
	}

	PlanetMesh->SetVisibility(true);

	OriginalLocation = InfoText->GetRelativeLocation();
	InfoText->Text = FText::FromString("");
	InfoText->SetVisibility(false);
	HighlightMesh->SetVisibility(false);
}

void APlanet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlanet, PlanetName);
	DOREPLIFETIME(APlanet, Grade);
	DOREPLIFETIME(APlanet, MaxSlots);
	DOREPLIFETIME(APlanet, CurrentResearch);
	DOREPLIFETIME(APlanet, CurrentFactories);
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlanet::BeginCursorOver(AActor* Actor)
{
	if (InfoText->Text.IsEmpty())
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlanetResourceGrade"), true);
		InfoText->SetText(FText::FromString("Name: " + PlanetName
			+ "\nGrade: " + EnumPtr->GetDisplayNameTextByValue(Grade).ToString()));
	}
	InfoText->SetVisibility(true);
	HighlightMesh->SetVisibility(true);
}

void APlanet::EndCursorOver(AActor* Actor)
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (AStratComPlayerState* PlayerState = PlayerController->GetPlayerState<AStratComPlayerState>())
			{
				if (PlayerState->GetSelected() != this)
				{
					InfoText->SetVisibility(false);
					HighlightMesh->SetVisibility(false);
				}
			}
		}
	}
}

void APlanet::HandleSelect()
{
	FText CurrentText = InfoText->Text;

	InfoText->SetText(FText::FromString(CurrentText.ToString() + 
	"\nSlots: " + FString::FromInt(MaxSlots)));
	InfoText->SetRelativeLocation(InfoText->GetRelativeLocation() + FVector{0,100,0});
}

void APlanet::HandleDeselect()
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlanetResourceGrade"), true);
	InfoText->SetText(FText::FromString("Name: " + PlanetName
		+ "\nGrade: " + EnumPtr->GetDisplayNameTextByValue(Grade).ToString()));
	InfoText->SetRelativeLocation(OriginalLocation);
	InfoText->SetVisibility(false);
	HighlightMesh->SetVisibility(false);
}

FVector APlanet::GetPlanetScale()
{
	return PlanetMesh->GetRelativeScale3D();
}

void APlanet::SetMaxSlots(uint8 NewMaxSlots)
{
	MaxSlots = NewMaxSlots;
}

void APlanet::SetGrade(EPlanetResourceGrade NewGrade)
{
	Grade = NewGrade;
}

