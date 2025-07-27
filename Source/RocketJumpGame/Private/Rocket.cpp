// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"
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
	ExplosionStrength = 50000.0f; 
	ExplosionFalloff = ERadialImpulseFalloff::RIF_Linear;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
}

void ARocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || (OtherActor == this) || !OtherComp)
	{
		return;
	}
	FVector ExplosionLocation = GetActorLocation();
	float QueryRadius = ExplosionRadius;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1); // PhysicsBody
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2); // Destructible
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); // Pawn
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);
	if (ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor))
	{
		PushACharacter(OtherCharacter,FVector(0.0,0.0,1.0),1000.0);
	}
	UE_LOG(LogTemp, Warning, TEXT("ARocket hit: %s"), *OtherActor->GetName());
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
		TargetCharacter->LaunchCharacter(LaunchVelocity, false, true); // Often, you want to add to XY but replace Z for jumps/knocks
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

