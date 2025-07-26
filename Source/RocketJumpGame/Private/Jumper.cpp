// Fill out your copyright notice in the Description page of Project Settings.


#include "Jumper.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
AJumper::AJumper()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 0.0f;
    SpringArm->bUsePawnControlRotation = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void AJumper::BeginPlay()
{
    Super::BeginPlay();
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
	
}

// Called every frame
void AJumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJumper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind input actions to functions
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AJumper::Move);
        EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AJumper::Jump);
        EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Started, this, &AJumper::Shoot);
        //EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Started, this, &AJumper::Look);

    }

    // Add the input mapping context
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

}

void AJumper::Move(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();
    if (Controller)
    {
        const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

        const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(Forward, Input.X);
        AddMovementInput(Right, Input.Y);
    }
}
//void AJumper::Look(const FInputActionValue& Value)
//{
//    FVector2D LookInput = Value.Get<FVector2D>();
//    AddControllerYawInput(LookInput.X);
//    AddControllerPitchInput(LookInput.Y);
//}
void AJumper::Shoot(const FInputActionValue& Value) {
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        return;
    }
    FVector CameraLocation,SpawnLocation;
    FRotator CameraRotation,SpawnRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    SpawnLocation = (CameraRotation.Vector())*100.0;
    SpawnRotation = CameraRotation+FRotator(-90.0f,0.0f,0.0f);
    if (RocketClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        ARocket* NewRocket = GetWorld()->SpawnActor<ARocket>(RocketClass, (CameraLocation + SpawnLocation), SpawnRotation,SpawnParams);
        if (NewRocket && NewRocket->ProjectileMovement)
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawned actor %s at %s"), *NewRocket->GetName(), *CameraLocation.ToString());
            NewRocket->ProjectileMovement->Velocity = NewRocket->ProjectileMovement->InitialSpeed * CameraRotation.Vector();

        }
    }
    else {
        UE_LOG(LogTemp,Warning,TEXT("RocketClass is not set"))
    }
}