#pragma once

#include "CoreMinimal.h"
#include "CameraFocusToEvent.h"
#include "CameraFocusWithTrigger.generated.h"



UCLASS()
class DUSTY_API ACameraFocusWithTrigger : public ACameraFocusToEvent
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CanBeActivatedBy")
	TSubclassOf<AActor> m_ActorClassToCheck;

public:
	ACameraFocusWithTrigger();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
