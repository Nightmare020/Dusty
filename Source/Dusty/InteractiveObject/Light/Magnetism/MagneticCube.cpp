#include "MagneticCube.h"

#include "InfiniteProvider.h"
#include "MagnetismComponent.h"
#include "NiagaraComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "../../../Utils/DustyUtils.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SceneRenderTargetParameters.h"
#include "../../../PhysicsSubsystem.h"
#include "../../../Character/DustyCharacter.h"
#include "../../../SaveGame/DustySaveGame.h"
#include "../../../DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Components/AudioComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"


UNiagaraComponent* AttractSystem;
UNiagaraComponent* RepulseSystem;;
FVector prevPos;
float Accumulator = 0.0f;
AMagneticCube::AMagneticCube()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	//SetRootComponent(Mesh);

	Mesh->SetSimulatePhysics(true);
	m_atracted = false;
	Mesh->SetNotifyRigidBodyCollision(true);

	magnetismComponent = CreateDefaultSubobject<UMagnetismComponent>("TestComponent");
	m_audio = CreateDefaultSubobject<UAudioComponent>("audio");
	m_dragAudio = CreateDefaultSubobject<UAudioComponent>("Drag Audio");

	m_iddleSound = CreateDefaultSubobject<UAudioComponent>("Iddle Audio");
}


void AMagneticCube::AddVelocity(FVector _vel)
{
	m_velocity += _vel;
}

void AMagneticCube::AddForce(FVector _force)
{
	force += _force;
}

void AMagneticCube::BeginPlay()
{
	Super::BeginPlay();
	//DustyUtils::PrintMessage("Lighted", Lighted);
	magnetismComponent->ChangeMagnetism(Lighted);
	if (magnetismComponent->GetLightStatus())
	{
		Activate(true);
		DustyUtils::SetEmissiveMaterialValues(Mesh, m_MaxLightIntensityRed, m_MaxLightIntensityGreen,
		                                      m_MaxLightIntensityBlue);
		m_effectActivated = true;
		SetActorTickEnabled(true);
		PlayMetasound("PlayIddleSound");
	}
	else
	{
		Activate(false);
		DustyUtils::SetEmissiveMaterialValues(Mesh, 0, 0, 0);
		SetActorTickEnabled(false);
		m_effectActivated = false;
		PlayMetasound("StopIddleSound");
	}

	if (m_dragAudio)
	{
		m_dragAudio->SetTriggerParameter("PlaySound");
	}

	SetActorTickEnabled(true);
	force = FVector(0, 0, 0);
}


void AMagneticCube::Tick(float DeltaTime)
{
	FVector previousPosition = GetActorLocation();
	if (isLoaded) //Execute all magnetism logic only if actor is loaded
	{
		Super::Tick(DeltaTime);

		Accumulator += DeltaTime;
		const float MaxSubsteps = 5;
		const float MaxTimeAccumulated = MaxSubsteps * FixedDeltaTime;
		Accumulator = FMath::Min(Accumulator, MaxTimeAccumulated);

		//if (Accumulator < FixedDeltaTime)
		//{
		//	return;
		//}

		prevPos = GetActorLocation();

		//DustyUtils::PrintMessage("Lighted", Lighted, FColor::Yellow, DeltaTime);

		m_colliding = false;

		//Check if the magnet is colliding with some surface in bottom
		FVector Start = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 65);
		FVector End = Start + (-GetActorUpVector() * GetActorScale3D().Z);
		FVector BoxHalfSize = FVector(GetActorScale3D().X * 70.0f, GetActorScale3D().Y * 70.0f, GetActorScale3D().Z * 10.0f);
		FQuat Rotation = GetActorRotation().Quaternion();

		FCollisionShape Box = FCollisionShape::MakeBox(BoxHalfSize);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FHitResult HitResult;
		m_colliding = GetWorld()->SweepSingleByChannel(
			HitResult,
			Start,
			End,
			Rotation,
			ECC_Visibility,
			Box,
			Params);


#if WITH_EDITORONLY_DATA
		if (GetActorLabel() == "MagnetismActor39")
		{
			// DustyUtils::PrintMessage("Velocity: ", m_velocity, FColor::Yellow, 5);
		}
#endif

		if (m_dragAudio)
		{
			if (ADustyCharacter* AuxPlayer = DustyUtils::GetDustyCharacter())
				//DustyUtils::PrintMessage("Distance", FMath::Clamp((GetActorLocation() - DustyUtils::GetDustyCharacter()->GetActorLocation()).Size(), 0, 1000), FColor::Yellow, GetWorld()->GetDeltaSeconds());
				m_dragAudio->SetFloatParameter("PlayerDistance",
				                               FMath::Clamp((GetActorLocation() - AuxPlayer->GetActorLocation()).Size(),
				                                            0, 1000));
		}

		if (m_colliding && !m_atracted)
		{
			if (!Cast<AMagneticCube>(HitResult.GetActor()))
			{
				if (!m_repulsed || m_pushed)
				{
					if(abs(m_velocity.Z) >= m_crashVelocity)
					{
						GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_crashSound, GetActorLocation());
					}
					force.Z = 0;
					m_velocity.Z = 0;
				}
			}

			else
			{
				m_colliding = false;
			}
		}
		//magnetismComponent->TickComponent(DeltaTime, LEVELTICK_All, nullptr);
		magnetismComponent->ExecuteMagnetism(DeltaTime);

		if (m_lightEffect)
		{
			if(Lighted)
			{
				m_lightEffect->SetWorldLocation(GetActorLocation());
			}
			m_particleLifetime += DeltaTime;
				
			if (m_particleLifetime >= 0.5f)
			{
				if (!Lighted)
				{
					DustyUtils::SetEmissiveMaterialValues(Mesh, 0, 0, 0);
					magnetismComponent->ChangeMagnetism(false); //Deactivate magnetism when light goes out
					//DustyUtils::PrintMessage("Deactivating", FColor::Yellow, 5.0f);
				}

				else
				{
					DustyUtils::SetEmissiveMaterialValues(Mesh, m_MaxLightIntensityRed, m_MaxLightIntensityGreen, m_MaxLightIntensityBlue);
					magnetismComponent->ChangeMagnetism(true); //Activate magnetism when light goes in
					//DustyUtils::PrintMessage("Activating", FColor::Yellow, 5.0f);
				}
				m_particleLifetime = 0.0f;
				m_lightEffect = nullptr;
			}
		}



		// DrawDebugBox(GetWorld(), Start, BoxHalfSize, Rotation, FColor::Green, false, 2.0f);
		// DrawDebugBox(GetWorld(), End, BoxHalfSize, Rotation, FColor::Blue, false, 2.0f);

		if (!m_parent)
		{
			UPhysicsSubsystem* physicsSubsystem = GetGameInstance()->GetSubsystem<UPhysicsSubsystem>();
			//DustyUtils::DustyDrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (-GetActorUpVector() * GetActorScale().Z * 60), FColor::Black, false, 0.0f);
			if (!m_colliding)
			{
				physicsSubsystem->AddGravity(force, mass);
			}
			else
			{
				//Add friction force to the magnet
				force += physicsSubsystem->CalcFricForce(m_velocity, mass);
			}

			//Add airDrag to the magnet
			FVector airDrag = physicsSubsystem->
				CalcAirDrag(m_velocity, 6 * (GetActorScale3D().X * GetActorScale3D().X));
			force += airDrag;

			//Calc acceleration of the magnet
			AddVelocity((force / mass) * FixedDeltaTime);

			//Calc newPos of the magnet
			FVector newPos = (GetActorLocation()) + m_velocity * FixedDeltaTime;

			if(m_colliding && bCanMove)
			{
				if(Mesh && Mesh->IsRegistered())
				{
					SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.ImpactPoint.Z + 75), true);
					//return;
				}
			}
			
			if (bCanMove)
			{
				if(Mesh && Mesh->IsRegistered())
				{
					SetActorLocation(newPos, true);
				}

				if(previousPosition != GetActorLocation())
				{
					float clampedValueX = FMath::Clamp(m_velocity.Size(), 0, 10000);
					float NormalizedValueX = ((clampedValueX - 0) / 10000);
					NormalizedValueX = FMath::Clamp(NormalizedValueX, 0, 0.2f);
					if (NormalizedValueX < 0.0f)
					{
						NormalizedValueX = 0;
					}
					if (m_dragAudio && m_colliding && !m_repulsed)
					{
						m_dragAudio->SetFloatParameter("Volume", NormalizedValueX);
					}
				}
				else
				{
					m_dragAudio->SetFloatParameter("Volume", 0);
				}
				// if((prevPos != GetActorLocation()))
				// {
				// 	float clampedValueX = FMath::Clamp(m_velocity.Size(), 0, 10000);
				// 	float NormalizedValueX = ((clampedValueX - 0) / 10000);
				// 	NormalizedValueX = FMath::Clamp(NormalizedValueX, 0, 0.2f);
				// 	if (NormalizedValueX < 0.0f)
				// 	{
				// 		NormalizedValueX = 0;
				// 	}
				// 	if (m_dragAudio && m_colliding && !m_repulsed)
				// 	{
				// 		m_dragAudio->SetFloatParameter("Volume", NormalizedValueX);
				// 	}
				// }
			}

			else
			{
				m_velocity = FVector(0, 0, 0);
			}
			


		}
		
		
		force = FVector(0, 0, 0);
		m_atracted = false;
		m_repulsed = false;
		m_pushed = false;

		Accumulator -= FixedDeltaTime;
	}
}

void AMagneticCube::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                              bool SelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                              const FHitResult& Hit)
{
	if (Cast<AMagneticCube>(Other))
	{
		m_velocity = FVector(0, 0, 0);
		return;
	}

	else if (Cast<ADustyCharacter>(Other))
	{
		//m_collidingForce.Z -= 100;
		return;
	}

	else
	{
		if (HitNormal != FVector::UpVector && !m_atracted)
		{
			float wallAngle = FVector::DotProduct(HitNormal, -m_velocity.GetSafeNormal());
			if (wallAngle == 1)
			{
				return;
			}
			FVector newDir = FVector::CrossProduct(FVector::UpVector, -m_velocity.GetSafeNormal());
			wallAngle = DustyUtils::GetAngleBetweenVectors(HitNormal, newDir);
			float TestX = newDir.X * 25.0f;
			float TestY = newDir.Y * 25.0f;
			float TestZ = newDir.Z * 25.0f;
			if (wallAngle > 90.0f)
			{
				if(!m_mustSound)
				{
					m_velocity = FVector(-TestX, -TestY, -TestZ);
					return;
				}
				
			}
			else
			{
				if(!m_mustSound)
				{
					m_velocity = FVector(TestX, TestY, TestZ);
					return;
				}
			}
		}
		if(abs(m_velocity.Z) > m_crashVelocity)
		{
			 UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
			if(audioSubsystem && m_crashSound)
			{
				audioSubsystem->PlaySound3D(m_crashSound, GetActorLocation());
			}
		}
		m_velocity = FVector(0, 0, 0);
		Super::NotifyHit(MyComp, Other, OtherComp, SelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	}
}


bool AMagneticCube::GetLightStatus()
{
	return magnetismComponent->GetLightStatus();
}

UMagnetismComponent* AMagneticCube::GetMagnetComponent()
{
	return magnetismComponent;
}

void AMagneticCube::LoadActor()
{
	Super::LoadActor();
	isLoaded = true;
}

void AMagneticCube::OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent)
{
	//m_lightEffect = nullptr;
}

void AMagneticCube::PlayMetasound_Implementation(const FName& _text)
{
}


void AMagneticCube::Activate(bool Activating)
{
	Super::Activate(Activating);
	SetActorTickEnabled(true);
	
	// if (!Lighted)
	// {
	// 	DustyUtils::SetEmissiveMaterialValues(Mesh, 0, 0, 0);
	// 	PlayMetasound("StopIddleSound");
	// }
	// else
	// {
	// 	DustyUtils::SetEmissiveMaterialValues(Mesh, m_MaxLightIntensityRed, m_MaxLightIntensityGreen, m_MaxLightIntensityBlue);
	// 	PlayMetasound("PlayIddleSound");
	// }
}

void AMagneticCube::Activate(bool _bActivation, AActor* _actor)
{
	Activate(_bActivation);

	//Play light FX
	if (m_niagaraSystem)
	{
		if (_bActivation && !m_effectActivated)
		{
			//Play Particle Sound
			if(m_particleSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_particleSound, GetActorLocation());
			}
			
			if (AInfiniteProvider* provider = Cast<AInfiniteProvider>(_actor)) //Check if the other actor is a provider
			{
				m_lightEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, GetActorLocation(), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_lightEffect->SetVariableVec3(FName("Origin"), provider->GetBallComponent()->GetComponentLocation());
				m_lightEffect->SetVariableFloat(FName("Lifetime"), 1.0f);
				m_lightEffect->OnSystemFinished.AddDynamic(this, &AMagneticCube::OnNiagaraSystemFinished);

				m_effectActivated = true;
			}

			else if(ADustyCharacter* dusty = Cast<ADustyCharacter>(_actor))
			{
				m_lightEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, GetActorLocation(), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_lightEffect->SetVariableVec3(FName("Origin"), dusty->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace));
				m_lightEffect->SetVariableFloat(FName("Lifetime"), 1.0f);
				m_lightEffect->OnSystemFinished.AddDynamic(this, &AMagneticCube::OnNiagaraSystemFinished);

				m_effectActivated = true;
			}

			else
			{
				m_lightEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, GetActorLocation(), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_lightEffect->SetVariableVec3(FName("Origin"), _actor->GetActorLocation());
				m_lightEffect->SetVariableFloat(FName("Lifetime"), 1.0f);
				m_lightEffect->OnSystemFinished.AddDynamic(this, &AMagneticCube::OnNiagaraSystemFinished);

				m_effectActivated = true;
			}
		}

		else if (!_bActivation && m_effectActivated)
		{
			//Play Particle Sound
			if(m_particleSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_particleSound, GetActorLocation());
			}
			if (AInfiniteProvider* provider = Cast<AInfiniteProvider>(_actor)) //Check if the other actor is a provider
			{
				m_lightEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, provider->GetBallComponent()->GetComponentLocation(),FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_lightEffect->SetVariableVec3(FName("Origin"), GetActorLocation());
				m_lightEffect->SetVariableFloat(FName("Lifetime"), 1.0f);
				DustyUtils::SetEmissiveMaterialValues(Mesh, 0, 0, 0);
				PlayMetasound("StopIddleSound");

				m_effectActivated = false;
			}

			else if(ADustyCharacter* dusty = Cast<ADustyCharacter>(_actor))
			{
				m_lightEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, dusty->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_lightEffect->SetVariableVec3(FName("Origin"), GetActorLocation());
				m_lightEffect->SetVariableFloat(FName("Lifetime"), 1.0f);
				DustyUtils::SetEmissiveMaterialValues(Mesh, 0, 0, 0);
				PlayMetasound("StopIddleSound");
				m_lightEffect->OnSystemFinished.AddDynamic(this, &AMagneticCube::OnNiagaraSystemFinished);

				m_effectActivated = false;
			}

			else
			{
				m_lightEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, _actor->GetActorLocation(), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_lightEffect->SetVariableVec3(FName("Origin"), GetActorLocation());
				m_lightEffect->SetVariableFloat(FName("Lifetime"), 1.0f);
				m_lightEffect->OnSystemFinished.AddDynamic(this, &AMagneticCube::OnNiagaraSystemFinished);

				m_effectActivated = false;
			}
		}

		if (m_lightEffect)
		{
			if ((m_lightEffect->GetComponentLocation() - GetActorLocation()).Size() < 50.0f)
			{
				if (!Lighted)
				{
					//DustyUtils::SetEmissiveMaterialValues(Mesh, 0, 0, 0);
					PlayMetasound("StopIddleSound");
				}

				else
				{
					//DustyUtils::SetEmissiveMaterialValues(Mesh, m_MaxLightIntensityRed, m_MaxLightIntensityGreen, m_MaxLightIntensityBlue);
					PlayMetasound("PlayIddleSound");
				}
			}
		}
	}
}

void AMagneticCube::SetAtracted(bool _value)
{
	m_atracted = _value;
}
