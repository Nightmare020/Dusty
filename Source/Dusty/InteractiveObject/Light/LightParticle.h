#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightParticle.generated.h"

class ADustyCharacter;
UCLASS()
class DUSTY_API ALightParticle : public AActor
{
	GENERATED_BODY()	
public:	
	ALightParticle();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

private:
	void moveToDusty();

public:	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		FVector FinalPosition = FVector::Zero();
	
	UPROPERTY()
		FVector InitialPosition;

	UPROPERTY(EditAnywhere)
		float Velocity;

	UPROPERTY(transient)
		ADustyCharacter* Character = nullptr;

};


