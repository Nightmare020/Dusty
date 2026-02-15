#pragma once

#include "CoreMinimal.h"
#include "Dusty/Utils/DustyConst.h"
#include "GameFramework/Actor.h"

#include "GlyphColumn.generated.h"

class UWidgetComponent;
class UGlyphWidget;
class AGlyphController;

USTRUCT()
struct FGlyphWord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EGlyphs m_glyph1 = EGlyphs::House;
	UPROPERTY(EditAnywhere)
	EGlyphs m_glyph2 = EGlyphs::Animal;
	UPROPERTY(EditAnywhere)
	EGlyphs m_glyph3 = EGlyphs::Darkness;
};

UCLASS()
class DUSTY_API AGlyphColumn : public AActor
{
	GENERATED_BODY()

#pragma region Variables_Private
private:

	UPROPERTY()
	bool m_rotSyllable1 = false;
	UPROPERTY()
	bool m_rotSyllable2 = false;
	UPROPERTY()
	bool m_rotSyllable3 = false;
	UPROPERTY()
	float m_stepAngle1 = 0;
	UPROPERTY()
	float m_stepAngle2 = 0;
	UPROPERTY()
	float m_stepAngle3 = 0;
	UPROPERTY()
	float m_crntRot1 = 0;
	UPROPERTY()
	float m_crntRot2 = 0;
	UPROPERTY()
	float m_crntRot3 = 0;
	UPROPERTY()
	int m_index1 = 0;
	UPROPERTY()
	int m_index2 = 0;
	UPROPERTY()
	int m_index3 = 0;

	bool m_solved = false;
#pragma endregion

#pragma region Components
protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* m_columnMesh;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* m_syllableMesh1;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* m_syllableMesh2;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* m_syllableMesh3;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* m_syllable1_glyph1;
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* m_syllable2_glyph1;
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* m_syllable3_glyph1;
#pragma endregion

protected:
	UPROPERTY(EditInstanceOnly, category = "Glyphs", Transient, meta=(DisplayName="Controller Instance"))
	AGlyphController* m_controllerInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Glyphs", DisplayName = "Syllable 1 Glyphs", meta = (Tooltip = "Include the 3 available GLyphs for this syllable"))
	TArray<EGlyphs> m_syllableGlyphs1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Glyphs", DisplayName = "Syllable 2 Glyphs", meta = (Tooltip = "Include the 3 available GLyphs for this syllable"))
	TArray<EGlyphs> m_syllableGlyphs2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Glyphs", DisplayName = "Syllable 3 Glyphs", meta = (Tooltip = "Include the 3 available GLyphs for this syllable"))
	TArray<EGlyphs> m_syllableGlyphs3;

	UPROPERTY(EditAnywhere, category = "Glyphs", DisplayName = "Rotation Speed", meta = (ToolTip = "Rotation speed in degrees per second"))
	float m_rotSpeed = 60;

	UPROPERTY(EditAnywhere, category = "Glyphs", DisplayName = "Solution Word")
	FGlyphWord m_solution;

	UPROPERTY()
	FGlyphWord m_currentWord;

	UPROPERTY(EditDefaultsOnly, category = "Glyphs | EditorOnly", DisplayName = "Glyph Widget Class")
	TSubclassOf<UGlyphWidget> m_GlyphWidgetClass;

protected:
	virtual void BeginPlay() override;

	void RotateSyllables(float _deltaTime);
	
	void CreateGlyphsForSyllables();
	
	void CheckSolution();
	
	void UpdateWord(int _indexToIncrease);
	
	UFUNCTION()
	void SyllableHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	

public:
	AGlyphColumn();
	
	virtual void Tick(float DeltaTime) override;
	
	void PoleSyllableHit(const UPrimitiveComponent* HitComponent);
};
