#include "GlyphColumn.h"
#include "Components/WidgetComponent.h"
#include "GlyphController.h"
#include "../../UI/Hieroglyphics/GlyphWidget.h"
#include "UObject/UObjectGlobals.h"


AGlyphColumn::AGlyphColumn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	m_columnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Column"));
	m_columnMesh->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);

	m_syllableMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("1st Syllable Mesh"));
	m_syllableMesh1->AttachToComponent(m_columnMesh, FAttachmentTransformRules::KeepRelativeTransform);

	m_syllable1_glyph1 = CreateDefaultSubobject<UWidgetComponent>(TEXT("Syllable 1 Glyph 1"));
	m_syllable1_glyph1->AttachToComponent(m_syllableMesh1, FAttachmentTransformRules::KeepRelativeTransform);
	m_syllable1_glyph1->SetWidgetClass(UGlyphWidget::StaticClass());

	m_syllableMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("2nd Syllable Mesh"));
	m_syllableMesh2->AttachToComponent(m_columnMesh, FAttachmentTransformRules::KeepRelativeTransform);

	m_syllable2_glyph1 = CreateDefaultSubobject<UWidgetComponent>(TEXT("Syllable 2 Glyph 1"));
	m_syllable2_glyph1->AttachToComponent(m_syllableMesh2, FAttachmentTransformRules::KeepRelativeTransform);
	m_syllable2_glyph1->SetWidgetClass(UGlyphWidget::StaticClass());


	m_syllableMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("3rd Syllable Mesh"));
	m_syllableMesh3->AttachToComponent(m_columnMesh, FAttachmentTransformRules::KeepRelativeTransform);

	m_syllable3_glyph1 = CreateDefaultSubobject<UWidgetComponent>(TEXT("Syllable 3 Glyph 1"));
	m_syllable3_glyph1->AttachToComponent(m_syllableMesh3, FAttachmentTransformRules::KeepRelativeTransform);
	m_syllable3_glyph1->SetWidgetClass(UGlyphWidget::StaticClass());
}

// Called when the game starts or when spawned
void AGlyphColumn::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	CreateGlyphsForSyllables();
	if (m_controllerInstance)
	{
		//m_controllerInstance->IncreaseColumn();
	}

	m_rotSyllable1 = false;
	m_rotSyllable2 = false;
	m_rotSyllable3 = false;
}

void AGlyphColumn::Tick(float DeltaTime)
{
	if (m_rotSyllable1 || m_rotSyllable2 || m_rotSyllable3)
	{
		RotateSyllables(DeltaTime);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AGlyphColumn::SyllableHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComponent == m_syllableMesh1 && !m_rotSyllable1)
	{
		m_rotSyllable1 = true;
		m_crntRot1 = 0;
		SetActorTickEnabled(true);
	}
	else if (HitComponent == m_syllableMesh2 && !m_rotSyllable2)
	{
		m_rotSyllable2 = true;
		m_crntRot2 = 0;
		SetActorTickEnabled(true);
	}
	else if (HitComponent == m_syllableMesh3 && !m_rotSyllable3)
	{
		m_rotSyllable3 = true;
		m_crntRot3 = 0;
		SetActorTickEnabled(true);
	}
}

void AGlyphColumn::PoleSyllableHit(const UPrimitiveComponent* HitComponent)
{
	if (HitComponent == m_syllableMesh1 && !m_rotSyllable1)
	{
		m_rotSyllable1 = true;
		m_crntRot1 = 0;
		SetActorTickEnabled(true);
	}
	else if (HitComponent == m_syllableMesh2 && !m_rotSyllable2)
	{
		m_rotSyllable2 = true;
		m_crntRot2 = 0;
		SetActorTickEnabled(true);
	}
	else if (HitComponent == m_syllableMesh3 && !m_rotSyllable3)
	{
		m_rotSyllable3 = true;
		m_crntRot3 = 0;
		SetActorTickEnabled(true);
	}
}

void AGlyphColumn::RotateSyllables(float _deltaTime)
{
	if (m_rotSyllable1)
	{
		if (m_crntRot1 < m_stepAngle1)
		{
			float const RotInTick = m_rotSpeed * _deltaTime;
			FRotator CurrentRotation = m_syllableMesh1->GetRelativeRotation();
			m_crntRot1 += RotInTick;
			CurrentRotation.Yaw -= RotInTick;

			m_syllableMesh1->SetRelativeRotation(CurrentRotation);
		}
		else 
		{
			UpdateWord(1);
			m_rotSyllable1 = false;
		}
	}
	if (m_rotSyllable2)
	{
		if (m_crntRot2 < m_stepAngle2)
		{
			float const RotInTick = m_rotSpeed * _deltaTime;
			FRotator CurrentRotation = m_syllableMesh2->GetRelativeRotation();
			m_crntRot2 += RotInTick;
			CurrentRotation.Yaw -= RotInTick;

			m_syllableMesh2->SetRelativeRotation(CurrentRotation);
		}
		else
		{
			UpdateWord(2);
			m_rotSyllable2 = false;
		}
	}
	if (m_rotSyllable3)
	{
		if (m_crntRot3 < m_stepAngle3)
		{
			float const RotInTick = m_rotSpeed * _deltaTime;
			FRotator CurrentRotation = m_syllableMesh3->GetRelativeRotation();
			m_crntRot3 += RotInTick;
			CurrentRotation.Yaw -= RotInTick;

			m_syllableMesh3->SetRelativeRotation(CurrentRotation);
		}
		else
		{
			UpdateWord(3);
			m_rotSyllable3 = false;
		}
	}
}

void AGlyphColumn::UpdateWord(int _indexToIncrease)
{
	switch (_indexToIncrease)
	{
	case 1:
		++m_index1;
		if (m_index1 >= m_syllableGlyphs1.Num())
		{
			m_index1 = 0;
		}
		m_currentWord.m_glyph1 = m_syllableGlyphs1[m_index1];
		break;
	case 2:
		++m_index2;
		if (m_index2 >= m_syllableGlyphs2.Num())
		{
			m_index2 = 0;
		}
		m_currentWord.m_glyph2 = m_syllableGlyphs2[m_index2];
		break;
	case 3:
		++m_index3;
		if (m_index3 >= m_syllableGlyphs3.Num())
		{
			m_index3 = 0;
		}
		m_currentWord.m_glyph3 = m_syllableGlyphs3[m_index3];
		break;
	default:
		break;
	}
	CheckSolution();
}

void AGlyphColumn::CheckSolution()
{
	if (m_currentWord.m_glyph1 == m_solution.m_glyph1 &&
		m_currentWord.m_glyph2 == m_solution.m_glyph2 &&
		m_currentWord.m_glyph3 == m_solution.m_glyph3 &&
		m_controllerInstance)
	{
		m_solved = true;
		//m_controllerInstance->AColumnIsSolved();
	}
	else if (m_solved == true && m_controllerInstance)
	{
		m_solved = false;
		//m_controllerInstance->IncreaseColumn();
	}
}

void AGlyphColumn::CreateGlyphsForSyllables()
{
	//Create all possible glyph options for the 1st syllable
	if (m_syllableGlyphs1.Num() > 0)
	{
		FVector const Direction1 = m_syllable1_glyph1->GetRelativeLocation() - m_syllableMesh1->GetRelativeLocation();
		m_stepAngle1 = 360 / m_syllableGlyphs1.Num();

		Cast<UGlyphWidget>(m_syllable1_glyph1->GetWidget())->SetGlyph(m_syllableGlyphs1[0]);
		m_index1 = 0;
		m_currentWord.m_glyph1 = m_syllableGlyphs1[m_index1];
		for (int i = 0; i < m_syllableGlyphs1.Num(); i++)
		{
			UWidgetComponent* NewComp = NewObject<UWidgetComponent>(this, FName(FString::Printf(TEXT("Syllable 1 Glyph %d"), i)));
			NewComp->RegisterComponent();

			NewComp->AttachToComponent(m_syllableMesh1, FAttachmentTransformRules::KeepRelativeTransform);
			NewComp->SetWidgetClass(m_GlyphWidgetClass);

			UGlyphWidget* GlyphWidget = Cast<UGlyphWidget>(NewComp->GetWidget());
			GlyphWidget->SetGlyph(m_syllableGlyphs1[i]);
			NewComp->SetDrawSize(FVector2D(1920, 1080));

			FVector const NewPosition = Direction1.RotateAngleAxis(m_stepAngle1 * i, GetActorUpVector());
			
			NewComp->SetRelativeLocation(NewPosition);
			NewComp->SetRelativeScale3D(m_syllable1_glyph1->GetRelativeScale3D());
			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			FRotator RotationVector = (NewComp->GetRelativeLocation() - m_syllableMesh1->GetRelativeLocation()).Rotation();
			RotationVector.Roll = 0;
			RotationVector.Pitch = 0;
			NewComp->SetRelativeRotation(RotationVector);
		}
	}

	if (m_syllableGlyphs2.Num() > 0)
	{
		FVector const Direction2 = m_syllable2_glyph1->GetRelativeLocation() - m_syllableMesh2->GetRelativeLocation();
		m_stepAngle2 = 360 / m_syllableGlyphs2.Num();

		Cast<UGlyphWidget>(m_syllable2_glyph1->GetWidget())->SetGlyph(m_syllableGlyphs2[0]);
		m_index2 = 0;
		m_currentWord.m_glyph2 = m_syllableGlyphs2[m_index2];
		for (int i = 0; i < m_syllableGlyphs2.Num(); i++)
		{
			UWidgetComponent* NewComp = NewObject<UWidgetComponent>(this, FName(FString::Printf(TEXT("Syllable 2 Glyph %d"), i)));
			NewComp->RegisterComponent();

			NewComp->AttachToComponent(m_syllableMesh2, FAttachmentTransformRules::KeepRelativeTransform);
			NewComp->SetWidgetClass(m_GlyphWidgetClass);

			UGlyphWidget* GlyphWidget = Cast<UGlyphWidget>(NewComp->GetWidget());
			GlyphWidget->SetGlyph(m_syllableGlyphs2[i]);
			NewComp->SetDrawSize(FVector2D(1920, 1080));

			FVector const NewPosition = Direction2.RotateAngleAxis(m_stepAngle2 * i, GetActorUpVector());

			NewComp->SetRelativeLocation(NewPosition);
			NewComp->SetRelativeScale3D(m_syllable2_glyph1->GetRelativeScale3D());
			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			FRotator RotationVector = (NewComp->GetRelativeLocation() - m_syllableMesh2->GetRelativeLocation()).Rotation();
			RotationVector.Roll = 0;
			RotationVector.Pitch = 0;
			NewComp->SetRelativeRotation(RotationVector);
		}
	}

	if (m_syllableGlyphs3.Num() > 0)
	{
		FVector const Direction3 = m_syllable3_glyph1->GetRelativeLocation() - m_syllableMesh3->GetRelativeLocation();
		m_stepAngle3 = 360 / m_syllableGlyphs3.Num();

		Cast<UGlyphWidget>(m_syllable3_glyph1->GetWidget())->SetGlyph(m_syllableGlyphs3[0]);
		m_index3 = 0;
		m_currentWord.m_glyph3 = m_syllableGlyphs3[m_index3];
		for (int i = 0; i < m_syllableGlyphs3.Num(); i++)
		{
			UWidgetComponent* NewComp = NewObject<UWidgetComponent>(this, FName(FString::Printf(TEXT("Syllable 3 Glyph %d"), i)));
			NewComp->RegisterComponent();

			NewComp->AttachToComponent(m_syllableMesh3, FAttachmentTransformRules::KeepRelativeTransform);
			NewComp->SetWidgetClass(m_GlyphWidgetClass);

			UGlyphWidget* GlyphWidget = Cast<UGlyphWidget>(NewComp->GetWidget());
			GlyphWidget->SetGlyph(m_syllableGlyphs3[i]);
			NewComp->SetDrawSize(FVector2D(1920, 1080));

			FVector const NewPosition = Direction3.RotateAngleAxis(m_stepAngle3 * i, GetActorUpVector());

			NewComp->SetRelativeLocation(NewPosition);
			NewComp->SetRelativeScale3D(m_syllable3_glyph1->GetRelativeScale3D());
			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			FRotator RotationVector = (NewComp->GetRelativeLocation() - m_syllableMesh3->GetRelativeLocation()).Rotation();
			RotationVector.Roll = 0;
			RotationVector.Pitch = 0;
			NewComp->SetRelativeRotation(RotationVector);
		}
	}
}
