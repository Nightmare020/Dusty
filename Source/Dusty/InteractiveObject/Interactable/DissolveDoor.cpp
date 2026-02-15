// Fill out your copyright notice in the Description page of Project Settings.


#include "../Interactable/DissolveDoor.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Camera/CameraModifier_CameraShake.h"
#include <Kismet/GameplayStatics.h>
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void ADissolveDoor::BeginPlay()
{
	Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(true);

    m_cameraManager = Cast<ADustyCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));

    Init(true, TimeDissolve);
    

    if (PreviewAnimationDebug)
    {
        OpenDoor();
    }
}

void ADissolveDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    OpenDoorAnimation(DeltaTime);
}

void ADissolveDoor::OpenDoor()
{
	m_openDoor = true;

}

void ADissolveDoor::Activate(bool _bActivation)
{
    Super::Activate(_bActivation);
    if (_bActivation)
    {
        OpenDoor();
    }
}

void ADissolveDoor::LoadActor()
{
    FActorSaveData ActorSaveData = GetGameInstance<UDustyGameInstance>()->GetActorSaveData(GetFName(), GetLevel()->GetOuter()->GetFName());

    if (ActorSaveData.Transform.GetLocation() == FVector::ZeroVector)
    {
        return;
    }
    SetActorTransform(ActorSaveData.Transform);

    FMemoryReader MemReader(ActorSaveData.ByteData);
    FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
    Ar.ArIsSaveGame = true;
    // Convert binary array back into actor's variables
    Ar << m_openDoor;

    if (m_openDoor)
    {
        m_OpenWhenLoaded = true;
    }

}

void ADissolveDoor::OnActorSaved_Implementation()
{
    FActorSaveData ActorData;

    ActorData.ActorName = GetFName();
    ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
    ActorData.Transform = GetActorTransform();
    FMemoryWriter MemWriter(ActorData.ByteData);

    FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
    Ar.ArIsSaveGame = true;
    // Convert binary array back into actor's variables
    Ar << m_openDoor;

    GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

void ADissolveDoor::OpenDoorAnimation(float DeltaTime)
{
    if (m_OpenWhenLoaded)
    {
        SetActorHiddenInGame(true);
        SetActorEnableCollision(ECollisionEnabled::NoCollision);
    }
    if (m_OpenWhenLoaded || (!m_openDoor && !PreviewAnimationDebug))
    {
        return;
    }

    float lerpValue = 0;
    float velModify = 0;

    //Animation
    if (m_currentTimeStartOffset <= TimeStartOpenOffset)
    {
        m_currentTimeStartOffset += DeltaTime;
    }
    else if (m_currentTimeStartOpen <= TimeStartOpen)
    {
        lerpValue = FMath::Lerp(0.f, 1.f, m_currentTimeStartOpen / TimeStartOpen);
        velModify = easeInCubic(lerpValue);

        Mesh->AddRelativeRotation(FRotator(0, VelocityStartOpen * velModify * DeltaTime, 0));
        m_currentTimeStartOpen += DeltaTime;
    }
    else if (m_currentTimeBlockDoor <= TimeBlockDoor)
    {
        lerpValue = FMath::Lerp(0.f, 1.f, m_currentTimeBlockDoor / TimeBlockDoor);
        float lerpValue2 = FMath::Lerp(1.f, 0.f, m_currentTimeBlockDoor / TimeBlockDoor);
        velModify = easeOutElastic(lerpValue);
        int dir = m_lastBlockValue < velModify ? 1 : -1;
        m_lastBlockValue = velModify;

        Mesh->AddRelativeRotation(FRotator(0, VelocityBlockDoor * lerpValue2 * dir * DeltaTime, 0));
        m_currentTimeBlockDoor += DeltaTime;

        if (!m_shakeOnce && CameraShake && m_currentTimeBlockDoor / TimeBlockDoor >= 0.15f)
        {
            m_shakeOnce = true;
            m_cameraManager->StartCameraShake(CameraShake, 1);
            m_cameraManager->EnableShakeModifier();
        }
    }
    else if (m_currentTimeStaticDoor <= TimeStaticDoor)
    {
        m_currentTimeStaticDoor += DeltaTime;
    }
    else if (m_currentTimeReOpenDoor <= TimeReOpen)
    {
        if(!soundPlayed)
        {
            UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
            audioSubsystem->PlaySound3D(m_sound, GetActorLocation());
            soundPlayed = true;
        }
        lerpValue = FMath::Lerp(0.f, 1.f, m_currentTimeReOpenDoor / TimeReOpen);
        velModify = easeInCubic(lerpValue);

        Mesh->AddRelativeRotation(FRotator(0, VelocityReOpen * velModify * DeltaTime, 0));
        m_currentTimeReOpenDoor += DeltaTime;
    }
    else if (m_currentTimeDissolve <= TimeDissolve)
    {

        if (!IsDissolveDoor)
        {
            lerpValue = FMath::Lerp(0.f, 1.f, m_currentTimeDissolve / TimeDissolve);
            velModify = easeOutCubic(1 - lerpValue);

            Mesh->AddRelativeRotation(FRotator(0, VelocityReOpen * velModify * DeltaTime, 0));
            m_currentTimeDissolve += DeltaTime;
        }
        else 
        {
            StartDissolve();

            Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Mesh->AddRelativeRotation(FRotator(0, VelocityReOpen * DeltaTime, 0));
            m_currentTimeDissolve += DeltaTime;
        }
    }
    else
    {
        m_cameraManager->DisableShakeModifier();
        //Destroy();
        //Not destroy, save system things
        if (IsDissolveDoor)
        {
            SetActorHiddenInGame(true);
            SetActorEnableCollision(ECollisionEnabled::NoCollision);
        }

    }
}

float ADissolveDoor::easeOutElastic(float x)
{
    float c4 = (2 * 3.14f) / 3;

    return x == 0 ?  1: FMath::Pow(2, -10 * x) * FMath::Sin((x * 10 - 0.75f) * c4) + 1;
}

float ADissolveDoor::easeInCubic(float x)
{
    return x * x * x;
}

float ADissolveDoor::easeOutCubic(float x)
{
    return 1 - FMath::Pow(1 - x, 3);
}