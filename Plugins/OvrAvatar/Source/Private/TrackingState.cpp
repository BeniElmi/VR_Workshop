#include "TrackingState.h"
#include "OvrAvatarPCH.h"
#include "OvrAvatarHelpers.h"
#include "OculusAvatarFunctionLibrary.h"

FTrackingState::FTrackingState()
{
	OvrAvatarHelpers::OvrPoseStatefIdentity(HeadPose);
	OvrAvatarHelpers::OvrPoseStatefIdentity(HandPoses[ovrpHand_Left]);
	OvrAvatarHelpers::OvrPoseStatefIdentity(HandPoses[ovrpHand_Right]);
}

FTrackingState::~FTrackingState()
{
}

bool FTrackingState::Update()
{
	return UOculusAvatarFunctionLibrary::GetCurrentTrackingState(this);
}

void FTrackingState::HeadPoseToAvatarTransform(ovrAvatarTransform& trans)
{
	OvrAvatarHelpers::OvrPoseToAvatarTransform(trans, HeadPose.Pose);
}

void FTrackingState::HandPoseToAvatarTransform(ovrAvatarTransform& trans, int hand)
{
	OvrAvatarHelpers::OvrPoseToAvatarTransform(trans, HandPoses[hand].Pose);
}