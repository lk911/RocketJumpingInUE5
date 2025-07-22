// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RootComponent = RocketMesh;
	CollisionSphere->SetupAttachment(RocketMesh);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->OnComponentHit.AddDynamic(this, &ARocket::OnHit);

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
	UE_LOG(LogTemp, Warning, TEXT("ARocket hit: %s"), *OtherActor->GetName());
	Destroy();
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Location = this->GetActorLocation();
	Rotation = (this->GetActorRotation())+FRotator(90.0f, 0.0f, 0.0f);;
	FVector ForwardDirection = Rotation.Vector();
	FVector DistanceMade = ForwardDirection * Velocity * DeltaTime;
	SetActorLocation(Location + DistanceMade);
}

