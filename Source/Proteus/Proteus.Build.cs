// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System;
using System.IO;

public class Proteus : ModuleRules
{
	public Proteus(ReadOnlyTargetRules Target) : base (Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
        string thirdParty = Path.Combine(BaseDirectory, "Plugins", "OvrAvatar", "ThirdParty");

        PublicDependencyModuleNames.Add("Proteus");

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils", "Networking", "Sockets" });
        PrivateDependencyModuleNames.AddRange(new string[] { "OvrAvatar", "OnlineSubsystem", "Sockets", "Networking", "OnlineSubsystemUtils" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemOculus");

        PublicIncludePaths.AddRange(new string[] { });
        PrivateIncludePaths.AddRange(new string[] { thirdParty + "/include" });

        PublicIncludePathModuleNames.AddRange(new string[] { });
        PrivateIncludePathModuleNames.AddRange(new string[] { });
    }
}
