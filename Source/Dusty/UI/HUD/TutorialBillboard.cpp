#include "TutorialBillboard.h"
#include "../../Character/DustyCharacter.h"
#include "../../UI/HUD/HUDSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Dusty/Utils/DustyConst.h"
ATutorialBillboard::ATutorialBillboard()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootBillboardsAndTexts"));
	Mesh->SetupAttachment(RootComponent);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	for (size_t i = 0; i < m_BillboardsNum; i++)
	{
		FString ButtonName = FString::Printf(TEXT("Billboard %zu"), i);
		Billboards.Add(CreateDefaultSubobject<UInteractiveButtonInterface>(*ButtonName));
		Billboards[i]->SetupAttachment(Mesh);
	}

	for (size_t i = 0; i < m_TextsNum; i++)
	{
		FString ButtonName = FString::Printf(TEXT("Text %zu"), i);
		Texts.Add(CreateDefaultSubobject<UTextRenderComponent>(*ButtonName));
		Texts[i]->SetupAttachment(Mesh);
	}
}


void ATutorialBillboard::BeginPlay()
{
	Super::BeginPlay();

	for (size_t i = 0; i < Billboards.Num(); i++)
	{
		m_OriginalBillboardScale.Add(Billboards[i]->GetMaterialScale());
		m_VisibilityBillboard.Add(Billboards[i]->IsVisible());

		Billboards[i]->SetMaterialScale(FVector2D::Zero());
		Billboards[i]->SetVisibility(false);
	}

	for (size_t i = 0; i < Texts.Num(); i++)
	{
		m_OriginalTextsScale.Add(FVector2D(Texts[i]->XScale, Texts[i]->YScale));
		m_VisibilityTexts.Add(Texts[i]->IsVisible());

		m_DynamicMaterialsTexts.Add(UMaterialInstanceDynamic::Create(Texts[i]->GetMaterial(0), this));
		Texts[i]->SetMaterial(0,m_DynamicMaterialsTexts[i]);
		m_DynamicMaterialsTexts[i]->SetScalarParameterValue(AlphaParamHUD, 0);

		Texts[i]->SetXScale(0);
		Texts[i]->SetYScale(0);
		Texts[i]->SetVisibility(false);
	}
}

void ATutorialBillboard::Activate(bool _bActivation)
{
	if (_bActivation)
	{
		BP_Activate();
	}
}

void ATutorialBillboard::Deactivate()
{
}

void ATutorialBillboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the player is in the interaction range of the interactable, update the button image
	for (size_t i = 0; i < Billboards.Num(); i++)
	{
		if (HasPlayerOverlapped && Billboards[i])
		{
			Billboards[i]->UpdateButtonIcon();
		}

		if (m_SpawnAnimationBillboard && Billboards[i])
		{

			float lerpValue = FMath::Lerp(Billboards[i]->GetMaterialAlpha(), 1, SpawnDespawnAnimationSpeed * DeltaTime);
			lerpValue = FMath::Clamp(lerpValue, 0, 1);

			FVector2D scaleValue = FMath::Lerp(Billboards[i]->GetMaterialScale(), m_OriginalBillboardScale[i], SpawnDespawnAnimationSpeed * DeltaTime);

			Billboards[i]->SetMaterialAlpha(lerpValue);
			Billboards[i]->SetMaterialScale(scaleValue);

			if (lerpValue == 1)
			{
				Billboards[i]->SetMaterialScale(m_OriginalBillboardScale[i]);
				m_SpawnAnimationBillboard = false;
			}
		}

		if (m_DespawnAnimationBillboard && Billboards[i])
		{

			float lerpValue = FMath::Lerp(Billboards[i]->GetMaterialAlpha(), 0, SpawnDespawnAnimationSpeed * DeltaTime);
			lerpValue = FMath::Clamp(lerpValue, 0, 1);

			FVector2D scaleValue = FMath::Lerp(Billboards[i]->GetMaterialScale(), FVector2D::Zero(), SpawnDespawnAnimationSpeed * DeltaTime);

			Billboards[i]->SetMaterialAlpha(lerpValue);
			Billboards[i]->SetMaterialScale(scaleValue);

			if (lerpValue == 0)
			{
				Billboards[i]->SetMaterialScale(FVector2D::Zero());
				m_DespawnAnimationBillboard = false;
				Billboards[i]->SetVisibility(false);
			}

		}
	}

	for (size_t i = 0; i < Texts.Num(); i++)
	{
		if (m_SpawnAnimationBillboard && Texts[i])
		{
			float alpha = 0;
			m_DynamicMaterialsTexts[i]->GetScalarParameterValue(AlphaParamHUD, alpha);

			float lerpValue = FMath::Lerp(alpha, 1, SpawnDespawnAnimationSpeed * DeltaTime);
			lerpValue = FMath::Clamp(lerpValue, 0, 1);

			FVector2D scaleValue = FMath::Lerp(FVector2D(Texts[i]->XScale , Texts[i]->YScale), m_OriginalTextsScale[i], SpawnDespawnAnimationSpeed * DeltaTime);

			m_DynamicMaterialsTexts[i]->SetScalarParameterValue(AlphaParamHUD, lerpValue);
			Texts[i]->SetXScale(scaleValue.X);
			Texts[i]->SetYScale(scaleValue.Y);

			if (lerpValue == 1)
			{
				Texts[i]->SetXScale(m_OriginalTextsScale[i].X);
				Texts[i]->SetYScale(m_OriginalTextsScale[i].Y);
				m_SpawnAnimationTexts = false;
			}
		}

		if (m_DespawnAnimationBillboard && Texts[i])
		{

			float lerpValue = FMath::Lerp(Billboards[i]->GetMaterialAlpha(), 0, SpawnDespawnAnimationSpeed * DeltaTime);
			lerpValue = FMath::Clamp(lerpValue, 0, 1);

			FVector2D scaleValue = FMath::Lerp(FVector2D(Texts[i]->XScale, Texts[i]->YScale), FVector2D::Zero(), SpawnDespawnAnimationSpeed * DeltaTime);

			m_DynamicMaterialsTexts[i]->SetScalarParameterValue(AlphaParamHUD, lerpValue);
			Texts[i]->SetXScale(scaleValue.X);
			Texts[i]->SetYScale(scaleValue.Y);

			if (lerpValue == 0)
			{
				Texts[i]->SetXScale(0);
				Texts[i]->SetYScale(0);
				m_DespawnAnimationTexts = false;
				Texts[i]->SetVisibility(false);
			}

		}
	}


}

void ATutorialBillboard::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Check if the overlapping actor is Dusty
	ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor);

	if (Character)
	{
		DustyCharacter = Character;
		HasPlayerOverlapped = true;
		SpawnAnimation();
	}
}

void ATutorialBillboard::NotifyActorEndOverlap(AActor* OtherActor)
{
	// Check if the actor that stopped overlapping is Dusty
	if (DustyCharacter && OtherActor == DustyCharacter)
	{
		HasPlayerOverlapped = false;
		DespawnAnimation();
	}
}

void ATutorialBillboard::SpawnAnimation()
{
	if (m_VisibilityBillboard.Num() > 0)
	{
		for (size_t i = 0; i < Billboards.Num(); i++)
		{
			Billboards[i]->SetVisibility(m_VisibilityBillboard[i]);
		}

		for (size_t i = 0; i < Texts.Num(); i++)
		{
			Texts[i]->SetVisibility(m_VisibilityTexts[i]);
		}
		m_SpawnAnimationBillboard = true;
		m_DespawnAnimationBillboard = false;

		m_SpawnAnimationTexts = true;
		m_DespawnAnimationTexts = false;
	}
}

void ATutorialBillboard::DespawnAnimation()
{
	for (size_t i = 0; i < Billboards.Num(); i++)
	{
		Billboards[i]->SetVisibility(m_VisibilityBillboard[i]);
	}
	for (size_t i = 0; i < Texts.Num(); i++)
	{
		Texts[i]->SetVisibility(m_VisibilityTexts[i]);
	}
	m_SpawnAnimationBillboard = false;
	m_DespawnAnimationBillboard = true;

	m_SpawnAnimationTexts = false;
	m_DespawnAnimationTexts = true;
}

