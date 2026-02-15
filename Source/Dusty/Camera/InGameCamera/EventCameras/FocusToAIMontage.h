#pragma once

#include "CoreMinimal.h"
#include "CameraFocusToEvent.h"
#include "FocusToAIMontage.generated.h"


class AMazeNPC;

UCLASS()
class DUSTY_API AFocusToAIMontage : public ACameraFocusToEvent
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CanBeActivatedBy")
	TSubclassOf<AActor> m_ActorClassToCheck;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Montage")
	UAnimMontage* m_Montage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MazeNPC")
	AMazeNPC* m_MazeNPC = nullptr;

public:
	AFocusToAIMontage();
	
	virtual void BeginPlay() override;
	virtual void Reached() override;
	
	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
