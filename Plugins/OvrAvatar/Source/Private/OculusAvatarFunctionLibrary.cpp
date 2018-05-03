#include "OculusAvatarFunctionLibrary.h"
#include "OvrAvatarPCH.h"
#include "OculusHMD.h"
#include "TrackingState.h"

bool UOculusAvatarFunctionLibrary::GetCurrentTrackingState(FTrackingState* TrackingState)
{
	bool bRetVal = false;

	check(IsInGameThread());

	OculusHMD::FOculusHMD* OculusHMD = GetOculusHMD();

	if (TrackingState && OculusHMD && OculusHMD->GetHMDDeviceType() == EHMDDeviceType::DT_OculusRift)
	{
		if (OVRP_SUCCESS(ovrp_GetNodePoseState2(ovrpStep_Game, OculusHMD::ToOvrpNode(ETrackedDeviceType::HMD), &(TrackingState->HeadPose))))
		{
			bRetVal = true;
		}

		OVRP_SUCCESS(ovrp_GetNodePoseState2(ovrpStep_Game, OculusHMD::ToOvrpNode(ETrackedDeviceType::LTouch), TrackingState->HandPoses + ovrpHand_Left));

		OVRP_SUCCESS(ovrp_GetNodePoseState2(ovrpStep_Game, OculusHMD::ToOvrpNode(ETrackedDeviceType::RTouch), TrackingState->HandPoses + ovrpHand_Right));
	}

	return bRetVal;
}

bool UOculusAvatarFunctionLibrary::GetUserProfileEx(FHmdUserProfile& Profile)
{
	if (GetUserProfile(Profile))
	{
		Profile.PlayerHeight = Profile.EyeHeight + (Profile.NeckToEyeDistance.X * 2) / 3;
		return true;
	}

	return false;
}