#include "OvrAvatarModule.h"
#include "OvrAvatarPCH.h"
#include "IPluginManager.h"
#include "OvrAvatarManager.h"


void OvrAvatarModule::StartupModule()
{
	FOvrAvatarManager::Get().InitializeSDK();
}
 
void OvrAvatarModule::ShutdownModule()	
{
	FOvrAvatarManager::Get().ShutdownSDK();
	FOvrAvatarManager::Destroy();
}
 
IMPLEMENT_MODULE(OvrAvatarModule, OvrAvatar)