// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class StratComEditorTarget : TargetRules
{
	public StratComEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("StratCom");
        ExtraModuleNames.Add("EnhancedInput");
	}
}
