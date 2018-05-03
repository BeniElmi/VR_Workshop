using UnrealBuildTool;
using System;
using System.IO;
using System.Reflection;

public class OvrAvatar : ModuleRules
{
    public OvrAvatar(ReadOnlyTargetRules Target) : base (Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, ".."));
        string thirdParty = Path.Combine(BaseDirectory, "ThirdParty");

        PublicIncludePaths.AddRange(
    new string[] {
                BaseDirectory + "/OvrAvatar/Source/Public",
                thirdParty + "/include"
    }
);

        string baseEngineDirectory = Path.Combine(Path.GetDirectoryName(Assembly.GetEntryAssembly().Location), @"../../");
        //baseEngineDirectory = @"D:\Epic Games\UE_4.18\Engine";

        PrivateIncludePaths.AddRange(
    new string[] {
                        BaseDirectory + "OvrAvatar/Private",
                        Path.Combine(baseEngineDirectory, @"Plugins/Runtime/Oculus/OculusVR/Source/OculusHMD/Private"),
                        Path.Combine(baseEngineDirectory, @"Plugins/Runtime/Oculus/OculusVR/Source/OculusInput/Private"),
                        Path.Combine(baseEngineDirectory, @"Source/Runtime/Renderer/Private")
    }
);

        // Online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Sockets",
				"Projects",
                "RenderCore",
                "RHI",
                "Voice"
            }   
        );

        PrivateIncludePathModuleNames.AddRange(
            new string[]
            {
                "OVRPlugin",
                "LibOVRPlatform",
                "OculusHMD",			// For IOculusHMDModule.h, before IOculusRiftPlugin.h
                "OculusInput",			// For IOculusInputModule.h, before IOculusInputPlugin.h
                "InputDevice",			// For IInputDevice.h
                "HeadMountedDisplay",   // For IMotionController.h
            }
        );

        PublicDependencyModuleNames.AddRange(new string[] { "LibOVRPlatform", "OnlineSubsystem", "OculusHMD", "OVRPlugin"});

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemOculus");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(thirdParty + "/lib/libovravatar.lib");
            PublicDelayLoadDLLs.Add("libovravatar.dll");
        }
        else
        {
            PrecompileForTargets = PrecompileTargetsType.None;
        }
    }
}