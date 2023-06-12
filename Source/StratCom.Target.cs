// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class StratComTarget : TargetRules
{
	public StratComTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("StratCom");
        ExtraModuleNames.Add("EnhancedInput");
	}
}
