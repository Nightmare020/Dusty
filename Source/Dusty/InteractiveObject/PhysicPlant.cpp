// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicPlant.h"

#include "Dusty/Character/DustyCharacter.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "../../Dusty/Character/DustyMovementComponent.h"


// Sets default values
APhysicPlant::APhysicPlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_mesh->SetupAttachment(RootComponent);
	m_mesh->SetSimulatePhysics(true);

	m_constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("Constraint");
	m_constraint->SetupAttachment(m_mesh);

}

// Called when the game starts or when spawned
void APhysicPlant::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void APhysicPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhysicPlant::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	// if (ADustyCharacter* dusty = Cast <ADustyCharacter>(OtherActor))
	// {
	// 	float force = FMath::Clamp(dusty->GetCharacterMovement()->Velocity.Size(), 0, 400);
	// 	DustyUtils::PrintMessage("Force",FMath::Clamp(dusty->GetCharacterMovement()->Velocity.Size(), 0, 400), FColor::Yellow, 5.0f);
	// 	DustyUtils::PrintMessage("Force", dusty->GetCharacterMovement()->Velocity.GetSafeNormal().Size(), FColor::Yellow, 5.0f);
	// 	m_mesh->AddImpulse(dusty->GetActorForwardVector() * m_force * force/400);
	// }
}

void APhysicPlant::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	// if (ADustyCharacter* dusty = Cast <ADustyCharacter>(Other))
	// {
	// 	float force = FMath::Clamp(dusty->GetCharacterMovement()->Velocity.Size(), 0, 400);
	// 	m_mesh->AddImpulse(dusty->GetActorForwardVector() * m_force);
	// }
}

