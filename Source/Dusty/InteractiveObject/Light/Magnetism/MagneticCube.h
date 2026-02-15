#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../LightActor.h"
#include "MagneticCube.generated.h"

class UMagnetismComponent;
class USphereComponent;
class UNiagaraSystem;
class UPhysicsSubsystem;
class UPointLightComponent;
class UDustySaveGame;
class UAudioComponent;
class UNiagaraComponent;

struct FActorSaveData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPositionChanged);

UCLASS()
class DUSTY_API AMagneticCube : public ALightActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly)
	bool RestartOnRestart1 = false;
	UPROPERTY(EditInstanceOnly)
	bool RestartOnRestart2 = false;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPositionChanged OnPositionChangedDelegate;
	AMagneticCube();
	
	virtual void Tick(float DeltaTime) override;

	virtual void Activate(bool _bActivating) override;
	virtual void Activate(bool _bActivation, AActor* _actor) override;

	void SetAtracted(bool _value);
	UPROPERTY()
		UPhysicsSubsystem* PhysicsSubsystem = nullptr;

	UPROPERTY(EditAnywhere)
	float mass;

	void AddVelocity(FVector _vel);
	void AddForce(FVector _force);

	bool m_magnetAttached = false;

protected:
	virtual void BeginPlay() override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

private:
		bool m_attached;

		
public:
	//Audio
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAudioComponent* m_audio;
		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAudioComponent* m_dragAudio;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAudioComponent* m_iddleSound;


	UPROPERTY(EditAnywhere)
	USoundBase* m_impactSound;
	UPROPERTY(EditAnywhere)
	USoundBase* m_crashSound;
	UPROPERTY(EditAnywhere)
	USoundBase* m_particleSound;
	
	UPROPERTY(EditAnywhere);
		bool m_colliding;
	UPROPERTY(EditAnywhere);
		AActor* m_parent;

	UPROPERTY(EditAnywhere)
	UMagnetismComponent* magnetismComponent;
	
	FVector m_collidingForce;
	UPROPERTY(EditAnywhere)
	bool isInCrane = false;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* m_niagaraSystem;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_lightEffect;

	bool m_effectActivated;
	float m_particleLifetime;

	UPROPERTY(EditAnywhere)
	float m_crashVelocity = 100.0f;

	UPROPERTY(EditAnywhere)
	bool m_mustSound = false;


#pragma region physicsVariables
public:
	UPROPERTY(EditAnywhere, Category = "Design | Magnet Variables", DisplayName = "atracted")
	bool m_atracted;
	UPROPERTY(EditAnywhere, Category = "Design | Magnet Variables", DisplayName = "atracted")
	bool m_repulsed;
	UPROPERTY(EditAnywhere, Category = "Design | Magnet Variables", DisplayName = "Can Move?")
	bool bCanMove;
	UPROPERTY(EditAnywhere, Category = "Design | Magnet Variables", DisplayName = "velocity")
	FVector m_velocity = FVector(0,0,0);
	FVector m_acceleration = FVector(10,10,10);
	UPROPERTY(EditAnywhere, Category = "Design | Magnet Variables", DisplayName = "force")
	FVector force = FVector(0,0,0);
	UPROPERTY(EditAnywhere, Category = "Design | Magnet Variables", DisplayName = "gravity?")
	bool bGravity;
	UPROPERTY(EditAnywhere, Category = "Design | Magnet Variables", DisplayName = "angle?")
	bool angle;

	bool m_pushed;
#pragma endregion
	
#pragma region magnetVariables
	
	UPROPERTY(EditAnywhere, Category = "Design | Magnet | Light", DisplayName = "MaxLightIntensityRed")
		float m_MaxLightIntensityRed = 8;
	
	UPROPERTY(EditAnywhere, Category = "Design | Magnet | Light", DisplayName = "MaxLightIntensityGreen")
		float m_MaxLightIntensityGreen = 8;
	
	UPROPERTY(EditAnywhere, Category = "Design | Magnet | Light", DisplayName = "MaxLightIntensityBlue")
		float m_MaxLightIntensityBlue = 8;
#pragma endregion

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	UStaticMeshComponent* Mesh = nullptr;

	FVector finalPos;

	virtual bool GetLightStatus() override;
	UMagnetismComponent* GetMagnetComponent();
	
	UFUNCTION(BlueprintNativeEvent)
	void PlayMetasound(const FName& _text);

	virtual void LoadActor() override;
	bool isLoaded;
	
	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent);

	const float FixedDeltaTime = 0.016f;
	
};
