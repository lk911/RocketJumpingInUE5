// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Jumper.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Rocket.generated.h"

UCLASS()
class ROCKETJUMPGAME_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocket();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Velocity = 1100.0;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Movement")
	FVector Location;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* CollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* RocketMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference")
	FRotator CameraRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	ERadialImpulseFalloff ExplosionFalloff;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void PushACharacter(ACharacter* TargetCharacter, FVector PushDirection, float PushStrength);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
