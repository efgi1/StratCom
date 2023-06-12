// Fill out your copyright notice in the Description page of Project Settings.


#include "StratComPawn.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "StratCom/Gameplay/Planet.h"
#include "StratCom/Player/StratComPlayerState.h"

// Sets default values
AStratComPawn::AStratComPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(RootComponent);
	
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AStratComPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStratComPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyCameraTranslation(DeltaTime);
}

// Called to bind functionality to input
void AStratComPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(ZoomAction.Get(), ETriggerEvent::Triggered, this, &AStratComPawn::Zoom);

	Input->BindAction(SelectAction.Get(), ETriggerEvent::Started, this, &AStratComPawn::HandleSelect);
	Input->BindAction(SelectAction.Get(), ETriggerEvent::Ongoing, this, &AStratComPawn::HandleDrag);
	Input->BindAction(SelectAction.Get(), ETriggerEvent::Triggered, this, &AStratComPawn::EndDrag);

}


void AStratComPawn::ApplyCameraTranslation(float DeltaTime)
{
	FVector2D MousePos;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePos.X, MousePos.Y);

	if (MousePos.IsNearlyZero())
	{
		return;
	}

	int32 SizeX, SizeY;
	GetWorld()->GetFirstPlayerController()->GetViewportSize(SizeX, SizeY);

	FVector UpdatedLocation = GetActorLocation();
	if (MousePos.X <= ScreenBorderThickness)
	{
		UpdatedLocation.Y -= TranslationSpeed * DeltaTime;
	}
	else if (SizeX - MousePos.X <= ScreenBorderThickness)
	{
		UpdatedLocation.Y += TranslationSpeed * DeltaTime;
	}

	if (MousePos.Y <= ScreenBorderThickness)
	{
		UpdatedLocation.Z += TranslationSpeed * DeltaTime;
	}
	else if (SizeY - MousePos.Y <= ScreenBorderThickness)
	{
		UpdatedLocation.Z -= TranslationSpeed * DeltaTime;
	}

	if (UpdatedLocation != GetActorLocation())
	{
		SetActorLocation(UpdatedLocation);
	}
}

void AStratComPawn::Zoom(const FInputActionInstance& Instance)
{
	float ZoomValue = Instance.GetValue().Get<float>();
	auto CurrentLocation = GetActorLocation();
	CurrentLocation.X -= ZoomSpeed * ZoomValue;
	SetActorLocation(CurrentLocation);
}

void AStratComPawn::HandleSelect(const FInputActionInstance& Instance)
{
	if (APlanet* HitPlanet = CheckPlanetTraceHit())
	{
			BeginDragPlanet = HitPlanet;
	}
	else 
	{
		if (AStratComPlayerState* PS = GetPlayerState<AStratComPlayerState>())
		{
			PS->DeselectCurrentlySelected();
		}
	}
}


void AStratComPawn::HandleDrag(const FInputActionInstance& Instance)
{
	FVector MouseLocation, MouseDirection;
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	if (BeginDragPlanet)
	{
		float IntersectionLength = (BeginDragPlanet->GetActorLocation().X - MouseLocation.X) / MouseDirection.X;
		FVector CurrentDragPos = MouseLocation + MouseDirection * IntersectionLength;
		DrawDebugDirectionalArrow(GetWorld(), BeginDragPlanet->GetActorLocation(),  CurrentDragPos, 10.f, FColor::Blue, false, 0.1f);
	}
}

void AStratComPawn::EndDrag(const FInputActionInstance& Instance)
{
	APlanet* HitPlanet = CheckPlanetTraceHit();
	if (HitPlanet && BeginDragPlanet)
	{
		if (HitPlanet == BeginDragPlanet)
		{
			if (AStratComPlayerState* PS = GetPlayerState<AStratComPlayerState>())
			{
				if (PS->GetSelected() != HitPlanet)
				{
					PS->SetSelected(HitPlanet);
					if (HitPlanet == PS->GetHomePlanet())
					{
						UE_LOG(LogTemp, Error, TEXT("SELECTED MY HOME PLANET!!!"));
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Send ships from %s to %s"), *BeginDragPlanet->PlanetName,*HitPlanet->PlanetName);
			DrawDebugDirectionalArrow(GetWorld(), BeginDragPlanet->GetActorLocation(), HitPlanet->GetActorLocation(), 10.f, FColor::Blue, false, 10.f);
		}
	}
	else
	{
		if (AStratComPlayerState* PS = GetPlayerState<AStratComPlayerState>())
		{
			PS->DeselectCurrentlySelected();
		}
	}
	BeginDragPlanet = nullptr;
}

APlanet* AStratComPawn::CheckPlanetTraceHit()
{
	FHitResult Hit;
	
	FVector MousePos, MouseDir;
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(MousePos, MouseDir);
	
	FVector TraceStart = MousePos;
	FVector TraceEnd = MousePos + MouseDir * 10000.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);
	
	
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		if (APlanet* HitPlanet = Cast<APlanet>(Hit.GetActor()))
		{
			return HitPlanet;
		}
	}
	return nullptr;
}


