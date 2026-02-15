// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsSubsystem.h"

#include "Utils/DustyUtils.h"

void UPhysicsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Warning, TEXT("Physics Subsystem Initialized"));
}

float UPhysicsSubsystem::GetMagnetForce()
{
	return m_magnetForce;
}

void UPhysicsSubsystem::SetMagnetForce(float _magnetForce)
{
	m_magnetForce = _magnetForce;
}

FVector UPhysicsSubsystem::AddGravity(FVector& _force)
{
	_force += FVector(0,0,-980);
	return _force;
	
}

FVector UPhysicsSubsystem::AddGravity(FVector& _force, float _mass)
{
	_force += FVector(0,0,-980) * _mass;
	return _force;
}


FVector UPhysicsSubsystem::CalcAirDrag(FVector _velocity, float _area)
{
	FVector vDrag = -_velocity;
	vDrag.Normalize();
	FVector vel2 = FVector(_velocity.X, _velocity.Y, 0);
	float speed = _velocity.Size() * 0.01f;
	float fDrag = 0.5f * (1.23f * (speed * speed) * _area * 0.01f * 0.6f);
	if(vel2.Size() > 300)
	{
		fDrag *= 500;
	}
	vDrag *= fDrag;
	
	return vDrag;
}

FVector UPhysicsSubsystem::CalcFricForce(FVector _velocity, float _mass)
{
	FVector vDrag = -_velocity;
	vDrag.Normalize();
	float speed = _velocity.Size();
	float force = 8 * _mass * speed;
	vDrag *= force;

	return vDrag;
}
