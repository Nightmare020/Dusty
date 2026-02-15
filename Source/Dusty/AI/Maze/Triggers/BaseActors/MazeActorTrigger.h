#pragma once

#include "CoreMinimal.h"
#include "MazeActor.h"
#include "MazeActorTrigger.generated.h"


UCLASS()
class DUSTY_API AMazeActorTrigger : public AMazeActor
{
private:
	GENERATED_BODY()

public:
	AMazeActorTrigger();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design|Base", meta = (AllowPrivateAccess = "true"), DisplayName = "CanBeActivatedBy")
	TSubclassOf<AActor> m_ActorClassToCheck;

	bool IsValidOverlap = false; // way of controlling in child classes if the overlap is valid

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void DoBeginOverlap(AActor* OtherActor) {}
	virtual void DoEndOverlap(AActor* OtherActor) {}

private:
	bool m_IsBeginOverlap = false;
	bool m_CheckDusty = false;
	
};
