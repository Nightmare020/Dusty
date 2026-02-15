// Fill out your copyright notice in the Description page of Project Settings.


#include "MagnetismComponent.h"

#include "MagneticCube.h"
#include "NiagaraComponent.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Engine/EngineTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Components/AudioComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Dusty/InteractiveObject/Interactable/MovableCrane.h"


UNiagaraSystem* magnetismSystem;
// Sets default values for this component's properties
UMagnetismComponent::UMagnetismComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMagnetismComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//Add directions to the component
	if(right)
	{
		// FMagnetDirections rightDirection;
		// rightDirection.system = m_attachEffect;
		// directions.Add(rightDirection);
		m_directions.Add(GetOwner()->GetActorRightVector());
	}
	if(left)
	{
		m_directions.Add(-GetOwner()->GetActorRightVector());
	}
	if(top)
	{
		m_directions.Add(GetOwner()->GetActorUpVector());
	}
	if(bottom)
	{
		m_directions.Add(-GetOwner()->GetActorUpVector());
	}
	if(front)
	{
		m_directions.Add(GetOwner()->GetActorForwardVector());
	}
	if(back)
	{
		m_directions.Add(-GetOwner()->GetActorForwardVector());
	}

	SetComponentTickEnabled(false);
	
	
}


// Called every frame
void UMagnetismComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// if(GetOwner())
	// {
	// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 	
	// 	m_timer += DeltaTime;
	// 	if(m_lighted) //If magnetism is active, throw raycasts in directions
	// 	{
	// 		for(FVector direction : m_directions) 
	// 		{
	// 			Raycast(direction);
	// 		}
	// 	}
	// }
}

void UMagnetismComponent::Raycast(FVector _direction)
{
	FHitResult hit;
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(DustyUtils::GetDustyCharacter());
	if(GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), (GetOwner()->GetActorLocation() + _direction * m_magnetDistance), ECC_Visibility, Params))
	{
		AMagneticCube* magnet = Cast<AMagneticCube>(hit.GetActor());
		if(magnet)
		{
			TArray<AActor*> attachedMagnets;
			GetOwner()->GetAttachedActors(attachedMagnets); //Check if the magnet is attached
			

			for(AActor* attachedMagnet : attachedMagnets)
			{
				if(attachedMagnet == magnet)
				{
					return;
				}
			}
			FVector MagnetProjection = DustyUtils::Projection(magnet->GetActorLocation() - GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + (_direction * m_magnetDistance)  - GetOwner()->GetActorLocation());
			if(m_lighted && !magnet->magnetismComponent->GetLightStatus() && !magnet->m_parent) //If other magnet is not lighted, attract
			{
				DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + _direction * m_magnetDistance, FColor::Green, false, GetWorld()->GetDeltaSeconds());
				//DustyUtils::PrintMessage("HIT", FColor::Yellow, GetWorld()->GetDeltaSeconds());
				magnet->force += (_direction * (-m_magnetAcceleration * 1000));
				magnet->finalPos = _direction * ((GetOwner()->GetActorScale3D().X * 100) + 50);
				magnet->m_atracted = true;

				if(m_timer > m_effectTimer)
				{
					m_timer = 0;
					FVector offset = (magnet->finalPos - GetOwner()->GetActorLocation()).GetSafeNormal();
					FRotator const AttachRotation = (magnet->GetActorLocation() - GetOwner()->GetActorLocation()).Rotation();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_attractEffect, GetOwner()->GetActorLocation() - (offset * GetOwner()->GetActorScale().X * 150 * _direction), AttachRotation, FVector(0.75f,0.75f,0.75f), true);
				}
				
				
				if(MagnetProjection.Size() <= (GetOwner()->GetActorScale().X * 100) + 50)
				{
					//DustyUtils::PrintMessage("ATTACH", FColor::Yellow, GetWorld()->GetDeltaSeconds());
					magnet->SetActorLocation(GetOwner()->GetActorLocation() + magnet->finalPos);

					FRotator const AttachRotation = (magnet->GetActorLocation() - GetOwner()->GetActorLocation()).Rotation();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_attachEffect, magnet->GetActorLocation() - _direction * ((GetOwner()->GetActorScale3D().X * 100)), AttachRotation, FVector(0.75f,0.75f,0.75f), true);
					magnet->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
					GetOwner()->GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_attractSound, GetOwner()->GetActorLocation());
					
					magnet->m_magnetAttached = true;
					magnet->m_parent = GetOwner();
					magnet->m_velocity = FVector(0,0,0);
					magnet->m_dragAudio->SetFloatParameter("Volume", 0);
				}
			}

			else if(m_lighted && !magnet->magnetismComponent->GetLightStatus() && magnet->m_parent)
			{
				return;
			}
			else //If other magnet is lighted, repulse
			{
				magnet->m_repulsed = true;
				TArray<AActor*> otherMagnetAtachedMagnets;
				DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + _direction * m_magnetDistance, FColor::Green, false, GetWorld()->GetDeltaSeconds());
				//DustyUtils::PrintMessage("HIT", FColor::Yellow, GetWorld()->GetDeltaSeconds());

				if(_direction == FVector::UpVector) //Apply spring-damper force
				{
					//DustyUtils::PrintMessage("PARENT: ", magnet->m_parent->GetActorLabel(), FColor::Yellow, GetWorld()->GetDeltaSeconds());
					AMagneticCube* magnetOwner = Cast<AMagneticCube>(GetOwner());
					float r = m_magnetDistance - (GetOwner()->GetActorScale().Z * 50);
					float L = MagnetProjection.Size() - GetOwner()->GetActorScale().Z * 100;
					FVector springForce = FVector(0, 0,  m_springConstant * (L - r));
					float damperForce =  m_damperConstant * FVector::DotProduct(( magnet->m_velocity - 0), FVector(0,0,m_magnetDistance));

					FVector finalForce = springForce + (damperForce / m_magnetDistance);
					magnet->force += FVector(0, 0, -finalForce.Z);
					if(m_timer > m_effectTimer)
					{
						m_timer = 0;
						FVector offset = (magnet->finalPos - GetOwner()->GetActorLocation()).GetSafeNormal();
						FRotator const AttachRotation = (magnet->GetActorLocation() - GetOwner()->GetActorLocation()).Rotation();
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_repulseEffect, GetOwner()->GetActorLocation() + (100 * _direction), AttachRotation, FVector(0.75f,0.75f,0.75f), true);
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_repulseEffect, magnet->GetActorLocation() - (100 * _direction), AttachRotation*-1, FVector(0.75f,0.75f,0.75f), true);
						GetOwner()->GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_repulseSound, GetOwner()->GetActorLocation());

					}

					if(Cast<AMovablePlatform>(GetOwner()->GetAttachParentActor()) || Cast<AMovablePlatform>(magnet->GetAttachParentActor()))
					{
						return;
					}
					
					if ((magnet->m_velocity.X < 50 || magnet->m_velocity.Y < 50) && (magnet->force.X < 5 || magnet->force.Y < 5) && !magnet->m_pushed)
					{
						magnet->SetActorLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, magnet->GetActorLocation().Z), true);
						magnet->m_velocity.X = 0;
						magnet->m_velocity.Y = 0;
					}
					return;
				}
				if(m_timer > m_effectTimer && _direction != FVector::DownVector)
				{
					m_timer = 0;
					FVector offset = (magnet->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
					FRotator const AttachRotation = (magnet->GetActorLocation() - GetOwner()->GetActorLocation()).Rotation();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_repulseEffect, GetOwner()->GetActorLocation() + GetOwner()->GetActorScale().X * 150 * _direction /*- (offset * GetOwner()->GetActorScale().X * 150 * _direction)*/,AttachRotation, FVector(0.75f,0.75f,0.75f), true);
					GetOwner()->GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_repulseSound, GetOwner()->GetActorLocation());

				}
				magnet->GetAttachedActors(otherMagnetAtachedMagnets);
				if(otherMagnetAtachedMagnets.Num() <= 0)
				{
				    magnet->force += (_direction * (m_magnetAcceleration * 1000));
				}
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + _direction * m_magnetDistance, FColor::Yellow, false, GetWorld()->GetDeltaSeconds());
	}
}

void UMagnetismComponent::ChangeMagnetism(bool _value)
{
	TArray<AActor*> attachedMagnets;
	GetOwner()->GetAttachedActors(attachedMagnets); //Check if the magnet is attached
	
	m_lighted = _value;

	if(!m_lighted) //Magnet off
	{
		for(AActor* attachedMagnet : attachedMagnets)
		{
			if(Cast<AMovablePlatform>(attachedMagnet))
			{
				return;
			}
			//GetOwner()->GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_repulseSound, GetOwner()->GetActorLocation());
			SeparateMagnet(attachedMagnet);
		}
	}

	else
	{

		AMagneticCube* magnet = Cast<AMagneticCube>(GetOwner());
		AMagneticCube* magnetParent = Cast<AMagneticCube>(magnet);
		if(Cast<AMovablePlatform>(magnetParent->GetAttachParentActor()))
		{
			return;
		}
		magnetParent->magnetismComponent->SeparateMagnet(GetOwner());
		magnet->m_parent = nullptr;

		
	}
}

void UMagnetismComponent::SeparateMagnet(AActor* _magnet)
{
	_magnet->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AMagneticCube* attachedMagnet = Cast<AMagneticCube>(_magnet);
	attachedMagnet->m_parent = nullptr;
}

bool UMagnetismComponent::GetLightStatus() const
{
	return m_lighted;
}

void UMagnetismComponent::ExecuteMagnetism(float _deltaTime)
{
	if(GetOwner())
	{
		m_timer += _deltaTime;
		if(m_lighted) //If magnetism is active, throw raycasts in directions
		{
			for(FVector direction : m_directions) 
			{
				Raycast(direction);
			}
		}
	}
}

