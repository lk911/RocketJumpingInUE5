// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "jumperPC.generated.h"

/**
 * 
 */
UCLASS()
class ROCKETJUMPGAME_API AjumperPC : public APlayerController
{
	GENERATED_BODY()
public:
	AjumperPC();

protected:
	virtual void BeginPlay() override;

	FVector2D GetViewportSize() const;
	
};
