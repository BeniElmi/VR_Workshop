#pragma once

#include "GameFramework/Pawn.h"
#include "OVR_Avatar.h"
#include "Set.h"
#include "OVR_Plugin.h"
#include "OVR_Microphone.h"
#include "Engine/SkeletalMesh.h"
#include "Components/MeshComponent.h"
#include "Components/ActorComponent.h"

#include "OvrAvatar.generated.h"

#define INPUT_COMMAND_TUPLE\
	INPUT_ENTRY(AvatarLeftThumbstick,		ovrpHand_Left,   ovrAvatarButton_Joystick)\
	INPUT_ENTRY(AvatarRightThumbstick,		ovrpHand_Right,  ovrAvatarButton_Joystick)\
	INPUT_ENTRY(AvatarLeftFaceButton1,		ovrpHand_Left,	ovrAvatarButton_One)\
	INPUT_ENTRY(AvatarRightFaceButton1,		ovrpHand_Right,	ovrAvatarButton_One)\
	INPUT_ENTRY(AvatarLeftFaceButton2,		ovrpHand_Left,	ovrAvatarButton_Two)\
	INPUT_ENTRY(AvatarRightFaceButton2,		ovrpHand_Right,	ovrAvatarButton_Two)

#define AXIS_INPUT_TUPLE\
	AXIS_ENTRY(AvatarLeftThumbstick,		ovrpHand_Left,	ovrAvatarTouch_Joystick)\
	AXIS_ENTRY(AvatarRightThumbstick,		ovrpHand_Right,	ovrAvatarTouch_Joystick)\
	AXIS_ENTRY(AvatarLeftTrigger,			ovrpHand_Left,	ovrAvatarTouch_Index)\
	AXIS_ENTRY(AvatarRightTrigger,			ovrpHand_Right,	ovrAvatarTouch_Index)\
	AXIS_ENTRY(AvatarLeftFaceButton1,		ovrpHand_Left,	ovrAvatarTouch_One)\
	AXIS_ENTRY(AvatarRightFaceButton1,		ovrpHand_Right,	ovrAvatarTouch_One)\
	AXIS_ENTRY(AvatarLeftFaceButton2,		ovrpHand_Left,	ovrAvatarTouch_Two)\
	AXIS_ENTRY(AvatarRightFaceButton2,		ovrpHand_Right,	ovrAvatarTouch_Two)\
	AXIS_ENTRY(AvatarLeftIndexPointing,		ovrpHand_Left,	ovrAvatarTouch_Pointing )\
	AXIS_ENTRY(AvatarRightIndexPointing,	ovrpHand_Right,	ovrAvatarTouch_Pointing )\
	AXIS_ENTRY(AvatarLeftThumbUp,			ovrpHand_Left,	ovrAvatarTouch_ThumbUp)\
	AXIS_ENTRY(AvatarRightThumbUp,			ovrpHand_Right,	ovrAvatarTouch_ThumbUp)

#define CUSTOM_AXIS_TUPLE\
	CUSTOM_ENTRY( AvatarLeftTriggerAxis,	ovrpHand_Left,	indexTrigger,	false )\
	CUSTOM_ENTRY( AvatarRightTriggerAxis,	ovrpHand_Right,	indexTrigger,	false )\
	CUSTOM_ENTRY( AvatarLeftGrip1Axis,		ovrpHand_Left,	handTrigger,	false )\
	CUSTOM_ENTRY( AvatarRightGrip1Axis,		ovrpHand_Right,	handTrigger,	false )\
	CUSTOM_ENTRY( AvatarLeftThumbstickX,	ovrpHand_Left,	joystickX,		false )\
	CUSTOM_ENTRY( AvatarRightThumbstickX,	ovrpHand_Right,	joystickX,		false )\
	CUSTOM_ENTRY( AvatarLeftThumbstickY,	ovrpHand_Left,	joystickY,		true )\
	CUSTOM_ENTRY( AvatarRightThumbstickY,	ovrpHand_Right,	joystickY,		true )\

class UPoseableMeshComponent;

UCLASS()
class OVRAVATAR_API UOvrAvatar : public UActorComponent
{
	GENERATED_BODY()

public:
	enum class ePlayerType
	{
		Local,
		Remote
	};

	UOvrAvatar();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void RequestAvatar(uint64_t userID);

	void HandleAvatarSpecification(const ovrAvatarMessage_AvatarSpecification* message);
	void HandleAssetLoaded(const ovrAvatarMessage_AssetLoaded* message);

	void SetVisibilityType(ovrAvatarVisibilityFlags flag) { VisibilityMask = flag; }
	void SetPlayerType(ePlayerType type) { PlayerType = type; }
	void SetPlayerHeightOffset(float offset) { PlayerHeightOffset = offset; }

	USceneComponent* DetachHand(ovrpHand hand);
	void ReAttachHand(ovrpHand hand);

	void SetRightHandPose(ovrAvatarHandGesture pose);
	void SetLeftHandPose(ovrAvatarHandGesture pose);
	void SetCustomGesture(ovrpHand hand, ovrAvatarTransform* joints, uint32_t numJoints);
	void SetControllerVisibility(ovrpHand hand, bool visible);

	//LEGACY PACKETS***
	ovrAvatarPacket* EndPacketRecording();
	void StartPacketRecording();
	void UpdateFromPacket(ovrAvatarPacket* packet, const float time);

	// First create a pointer to MeshComponents
	const TMap<ovrAvatarAssetID, TWeakObjectPtr<UPoseableMeshComponent>> *MeshComponentsPtr = &MeshComponents;

	// Second function that converts MeshComponents to an array of UPoseableMeshComponents
	TArray<TWeakObjectPtr<UPoseableMeshComponent>> ConvertMeshComponents();

#define INPUT_ENTRY(entry, hand, flag) \
	void entry##_Pressed();\
	void entry##_Released();
	INPUT_COMMAND_TUPLE
#undef INPUT_ENTRY

#define AXIS_ENTRY(entry, hand, flag) \
	void entry##_Value( float value);
		AXIS_INPUT_TUPLE
#undef AXIS_ENTRY

#define CUSTOM_ENTRY(entry, hand, field, invert) \
	void entry##_Value( float value);
		CUSTOM_AXIS_TUPLE
#undef CUSTOM_ENTRY

protected:
	
	static FString HandNames[ovrpHand_Count];

	void UpdateTransforms(float DeltaTime);

	void DebugDrawSceneComponents();

	void AddMeshComponent(ovrAvatarAssetID id, UPoseableMeshComponent* mesh);
	void AddDepthMeshComponent(ovrAvatarAssetID id, UPoseableMeshComponent* mesh);

	UPoseableMeshComponent* GetMeshComponent(ovrAvatarAssetID id) const;
	UPoseableMeshComponent* GetDepthMeshComponent(ovrAvatarAssetID id) const;

	UPoseableMeshComponent* CreateMeshComponent(USceneComponent* parent, ovrAvatarAssetID assetID, const FString& name);
	UPoseableMeshComponent* CreateDepthMeshComponent(USceneComponent* parent, ovrAvatarAssetID assetID, const FString& name);

	void LoadMesh(USkeletalMesh* SkeletalMesh, const ovrAvatarMeshAssetData* data);

	void UpdateSDK(float DeltaTime);
	void UpdatePostSDK();
	void UpdateVoiceData(float DeltaTime);

	void UpdateMeshComponent(USceneComponent& mesh, const ovrAvatarTransform& transform);
	void UpdateMaterial(UMeshComponent& mesh, const ovrAvatarMaterialState& material);
	void UpdateMaterialPBR(UPoseableMeshComponent& mesh, const ovrAvatarRenderPart_SkinnedMeshRenderPBS& data);
	void UpdateMaterialProjector(UPoseableMeshComponent& mesh, const ovrAvatarRenderPart_ProjectorRender& data, const USceneComponent& OvrComponent);

	void UpdateSkeleton(UPoseableMeshComponent& mesh, const ovrAvatarSkinnedMeshPose& pose);

	void DebugLogAvatarSDKTransforms(const FString& wrapper);
	void DebugLogMaterialData(const ovrAvatarMaterialState& material, const FString& name);

	uint64_t OnlineUserID = 0;

	TSet<ovrAvatarAssetID> AssetIds;
	TMap<ovrAvatarAssetID, TWeakObjectPtr<UPoseableMeshComponent>> MeshComponents;
	TMap<ovrAvatarAssetID, TWeakObjectPtr<UPoseableMeshComponent>> DepthMeshComponents;

	ovrAvatar* Avatar = nullptr;

	TMap<FString, TWeakObjectPtr<USceneComponent>> RootAvatarComponents;

	ovrAvatarHandInputState HandInputState[ovrpHand_Count];
	ovrAvatarTransform BodyTransform;

	bool LeftControllerVisible = false;
	bool RightControllerVisible = false;
	uint32_t VisibilityMask = ovrAvatarVisibilityFlag_ThirdPerson;

	ePlayerType PlayerType = ePlayerType::Local;
	float PlayerHeightOffset = 0.f;

	ovrMicrophoneHandle Microphone = nullptr;

	ovrAvatarAssetID ProjectorMeshID = 0;
	TWeakObjectPtr<UPoseableMeshComponent> ProjectorMeshComponent;

	TWeakObjectPtr<USceneComponent> AvatarHands[ovrpHand_Count];
};