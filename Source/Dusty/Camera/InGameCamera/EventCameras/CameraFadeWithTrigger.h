#pragma once

#include "CoreMinimal.h"
#include "CameraFade.h"
#include "CameraFadeWithTrigger.generated.h"


UCLASS()
class DUSTY_API ACameraFadeWithTrigger : public ACameraFade
{
public:
	ACameraFadeWithTrigger();
	
	virtual void BeginPlay() override;

private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CanBeActivatedBy")
	TSubclassOf<AActor> m_ActorClassToCheck;
	
protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
