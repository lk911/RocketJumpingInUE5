// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
// Sets default values
ARocket::ARocket()
{
	PrimaryActorTick.bCanEverTick = false;
	//collision
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RootComponent = CollisionSphere;
	RocketMesh->SetupAttachment(CollisionSphere);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	/*CollisionSphere->SetUseCCD(true);*/
	CollisionSphere->OnComponentHit.AddDynamic(this, &ARocket::OnHit);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RocketMesh->SetMobility(EComponentMobility::Movable);
	//projectile movement
	Rotation = this->GetActorRotation();
	FVector ForwardDirection = Rotation.Vector();
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = Velocity;
	ProjectileMovement->MaxSpeed = Velocity;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ExplosionRadius = 500.0f;
	KnockbackStrength = 1000.0f;
	EdgeKnockbackScale = 0.5f;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	// don't let the rocket hit whoever fired it on the way out
	if (AActor* Shooter = GetOwner())
	{
		CollisionSphere->IgnoreActorWhenMoving(Shooter, true);
	}
}

void ARocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || (OtherActor == this) || (OtherActor == GetOwner()) || !OtherComp)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("ARocket hit: %s"), *OtherActor->GetName());
	Explode();
}

void ARocket::Explode()
{
	const FVector ExplosionLocation = GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->OverlapMultiByObjectType(Overlaps, ExplosionLocation, FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn), FCollisionShape::MakeSphere(ExplosionRadius), QueryParams);

	// a character can show up more than once if several of its components overlap
	TSet<ACharacter*> HitCharacters;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		if (ACharacter* HitCharacter = Cast<ACharacter>(Overlap.GetActor()))
		{
			HitCharacters.Add(HitCharacter);
		}
	}

	for (ACharacter* HitCharacter : HitCharacters)
	{
		const FVector ToCharacter = HitCharacter->GetActorLocation() - ExplosionLocation;
		// full strength at the center, fading linearly to EdgeKnockbackScale at the edge
		const float DistanceAlpha = FMath::Clamp(ToCharacter.Size() / ExplosionRadius, 0.0f, 1.0f);
		const float Strength = KnockbackStrength * FMath::Lerp(1.0f, EdgeKnockbackScale, DistanceAlpha);
		// if the explosion is right on top of the character, push straight up
		const FVector PushDirection = ToCharacter.IsNearlyZero() ? FVector::UpVector : ToCharacter;
		PushACharacter(HitCharacter, PushDirection, Strength);
	}

	Destroy();
}
void ARocket::PushACharacter(ACharacter* TargetCharacter, FVector PushDirection, float PushStrength)
{
	if (TargetCharacter)
	{
		// Calculate the launch velocity
		FVector LaunchVelocity = PushDirection.GetSafeNormal() * PushStrength;

		// Launch the character
		// bXYOverride: If true, replaces current XY velocity. If false, adds to it.
		// bZOverride: If true, replaces current Z velocity. If false, adds to it.
		TargetCharacter->LaunchCharacter(LaunchVelocity, false, false); // add to both so a jump and a rocket stack like in TF2
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid character reference"));
	}
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

