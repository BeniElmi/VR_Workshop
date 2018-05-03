#pragma once

#include "OculusFunctionLibrary.h"

#include "OculusAvatarFunctionLibrary.generated.h"

class FTrackingState;


UCLASS(ClassGroup = Utilities)
class OVRAVATAR_API UOculusAvatarFunctionLibrary : public UOculusFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//UFUNCTION(BlueprintCallable, Category = "HMD")
	static bool GetCurrentTrackingState(FTrackingState* TrackingState);

	UFUNCTION(BlueprintCallable, Category = "Input|OculusLibrary")
	static bool GetUserProfileEx(FHmdUserProfile& Profile);
};
