// Fill out your copyright notice in the Description page of Project Settings.


#include "jumperPC.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "math/Vector2D.h"


AjumperPC::AjumperPC()
{
}

void AjumperPC::BeginPlay()
{
    Super::BeginPlay();


    APlayerController* PC = this;
    if (!PC)
    {
        PC = GetWorld()->GetFirstPlayerController(); // Fallback
    }

    if (PC)
    {
        // 1. Hide the OS mouse cursor
        PC->bShowMouseCursor = false;

        // 2. Set the Input Mode to Game Only
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);

        //    This ensures the mouse capture is truly active.
        PC->SetMouseLocation(GetViewportSize().X / 2, GetViewportSize().Y / 2); // Center mouse initially
    }
}

FVector2D AjumperPC::GetViewportSize() const
{
    FVector2D ViewportSize = FVector2D::ZeroVector;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    }
    return ViewportSize;
}
