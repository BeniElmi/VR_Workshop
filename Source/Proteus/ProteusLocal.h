// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Online.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OvrAvatar.h"
#include "Engine/LocalPlayer.h"
//#include "OvrAvatarManager.h"
#include "ProteusLocal.generated.h"

UCLASS()
class PROTEUS_API AProteusLocal : public APawn
{
	GENERATED_BODY()

public:

	struct SerializedPacketBuffer
	{
		uint32_t Size;
		uint8_t* Buffer;
	};
	struct AvatarPacketQueue
	{
		TQueue<SerializedPacketBuffer> PacketQueue;
		uint32_t PacketQueueSize = 0;
	};
	TMap<FString, AvatarPacketQueue*> AvatarPacketQueues;

	UFUNCTION(BlueprintCallable, Category = "ProteusAvatar")
		void FetchAvatarID(int PartA, int PartB, int PartC);

	UFUNCTION(BlueprintCallable, Category = "ProteusAvatar")
		bool RegisterRemoteTalker(FString OculusID);

	UFUNCTION(BlueprintCallable, Category = "ProteusAvatar")
		bool UnregisterRemoteTalker(FString OculusID);

	UFUNCTION(BlueprintCallable, Category = "ProteusAvatar")
		void DisableRecordPackets();

	// Constructor
	AProteusLocal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(UInputComponent* Input) override;

	void RegisterRemoteAvatar(const FString& key);
	ovrAvatarPacket* RequestAvatarPacket(const FString& key);
	void UnregisterRemoteAvatar(const FString& key);

private:

	//VARIABLES
	int32 Part1;
	int32 Part2;
	int32 Part3;
	int64 finalAvatar = 0;
	bool ControllersVisible = true;
	bool bIsPlayingSameGame;
	FVector2D StickPosition[ovrpHand_Count];

	//STRUCTS
	struct FPacketRecordSettings
	{
		bool Initialized = false;
		bool RecordingFrames = false;
		float UpdateRate = 0.2f;  // Lower rate = bigger packets. Try to optimize against fidelity vs Network Overhead.
		float AccumulatedTime = 1.f;
	};
	FPacketRecordSettings PacketSettings;

	//ENUMS
	enum class eHandPoseState
	{
		Default,
		Sphere,
		Cube,
		Custom,
		Controller,
		Detached,
	};
	eHandPoseState LeftHandPoseIndex = eHandPoseState::Default;
	eHandPoseState RightHandPoseIndex = eHandPoseState::Default;

	//DELEGATES
	FDelegateHandle OnLoginCompleteDelegateHandle;

	//POINTERS
	UOvrAvatar* AvatarComponent = nullptr;
	//The TWeakObjectPtr assignment operator takes a raw C++ pointer to its template type,
	//so for a TWeakObjectPtr<AActor> the assignable type would be
	//AvatarHands array of 2 elements is declared as 
	TWeakObjectPtr<USceneComponent> AvatarHands[ovrpHand_Count];

	//FUNCTIONS
	int64 ConvertAvatarID(int32 Part1, int32 Part2, int32 Part3);
	void AdditionalSetup();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void EnableRecordPackets();
	void UpdatePacketRecording(float DeltaTime);
	void DriveHand(ovrpHand hand, float DeltaTime);
	void LeftThumbstickX_Value(float value);
	void LeftThumbstickY_Value(float value);
	void RightThumbstickX_Value(float value);
	void RightThumbstickY_Value(float value);
	void QueueAvatarPacket(ovrAvatarPacket* packet);

	//DEFINES
	#define CUSTOM_ENTRY(entry, hand, field, invert) \
    void entry##_Value( float value);
	CUSTOM_AXIS_TUPLE
	#undef CUSTOM_ENTRY

	#define INPUT_ENTRY(entry, hand, flag) \
    void entry##_Pressed();\
    void entry##_Released();
		INPUT_COMMAND_TUPLE
	#undef INPUT_ENTRY

	#define AXIS_ENTRY(entry, hand, flag) \
    void entry##_Value( float value);
		AXIS_INPUT_TUPLE
	#undef AXIS_ENTRY

		//SUPPLEMENTAL
		//void SpawnNewRemoteAvatar();
		//void DestroyRemoteAvatar();
		//void CycleRightHandAttach();
		//void CycleLeftHandAttach();
};