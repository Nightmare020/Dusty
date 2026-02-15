#pragma once

#include "CoreMinimal.h"
#include "CameraFocusToEvent.h"
#include "CameraFocusToAISpawning.generated.h"


class AMazePlayerActivator;

UCLASS()
class DUSTY_API ACameraFocusToAISpawning : public ACameraFocusToEvent
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CanBeActivatedBy")
	TSubclassOf<AActor> m_ActorClassToCheck;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MazePlayerActivator")
	AMazePlayerActivator* m_MazePlayerActivator = nullptr;
	

public:
	ACameraFocusToAISpawning();
	virtual void BeginPlay() override;
	virtual void Reached() override;
	
	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
