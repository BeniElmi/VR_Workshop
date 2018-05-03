#pragma once

#include "CoreMinimal.h"
#include "OVR_Avatar.h"

class OvrAvatarTransform;

/**
 * Tracking state at a given absolute time (describes predicted HMD pose, etc.).
 */
class OVRAVATAR_API FTrackingState
{
public:
	FTrackingState();
	~FTrackingState();

	bool Update();

	void HeadPoseToAvatarTransform(ovrAvatarTransform& trans);
	void HandPoseToAvatarTransform(ovrAvatarTransform& trans, int hand);

	/// Predicted head pose (and derivatives) at the requested absolute time.
	ovrpPoseStatef  HeadPose;

	/// The most recent calculated pose for each hand when hand controller tracking is present.
	/// HandPoses[ovrHand_Left] refers to the left hand and HandPoses[ovrHand_Right] to the right hand.
	/// These values can be combined with ovrInputState for complete hand controller information.
	ovrpPoseStatef  HandPoses[2];
};