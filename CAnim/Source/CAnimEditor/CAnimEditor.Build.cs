using UnrealBuildTool;

public class CAnimEditor : ModuleRules {
    public CAnimEditor(ReadOnlyTargetRules Target) : base(Target) {

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("CAnimEditor/Private");

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetRegistry", 
				"MainFrame",
				"DesktopPlatform",
                "SkeletonEditor",
                "ContentBrowser",
                "AssetTools",
                "AnimationEditor",
            }
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
                "AppFramework",
				"Core", 
				"CoreUObject", 
				"Slate", 
				"SlateCore",
                "EditorStyle",
				"Engine", 
				"UnrealEd", 
				"GraphEditor", 
                "InputCore",
				"KismetWidgets",
				"AnimGraph",
                "PropertyEditor",
				"EditorWidgets",
                "BlueprintGraph",
                "RHI",
                "KismetCompiler",
            }
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"ContentBrowser",
				"Documentation",
				"MainFrame",
				"DesktopPlatform",
                "SkeletonEditor",
                "AssetTools",
                "AnimationEditor",
            }
		);

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Kismet",
                "Persona",
            }
        );

		PublicIncludePaths.Add("CAnimEditor/Public");
		
        PublicDependencyModuleNames.Add("CAnim");
    }
}