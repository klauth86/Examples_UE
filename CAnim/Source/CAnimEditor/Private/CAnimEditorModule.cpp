// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CAnimEditorModule.h"
#include "CAnimEditor.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"

#include "CAnimGraphFactory.h"
#include "Animation/Base/CAnimBlueprintBase.h"
#include "Animation/Base/CAnimInstanceBase.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Developer/MessageLog/Public/MessageLogModule.h"

IMPLEMENT_MODULE( FCAnimEditorModule, CAnimEditor);

#define LOCTEXT_NAMESPACE "CAnimEditorModule"

void FCAnimEditorModule::StartupModule()
{
	MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

	AnimGraphNodeFactory = MakeShareable(new FCAnimGraphNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(AnimGraphNodeFactory);

	AnimGraphPinFactory = MakeShareable(new FCAnimGraphPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(AnimGraphPinFactory);

	AnimGraphPinConnectionFactory = MakeShareable(new FCAnimGraphPinConnectionFactory());
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(AnimGraphPinConnectionFactory);

	FKismetEditorUtilities::RegisterOnBlueprintCreatedCallback(this, UCAnimInstanceBase::StaticClass(), FKismetEditorUtilities::FOnBlueprintCreated::CreateRaw(this, &FCAnimEditorModule::OnNewBlueprintCreated));
	
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowFilters = true;
	InitOptions.bShowPages = true;
	MessageLogModule.RegisterLogListing("CAnimBlueprintLog", LOCTEXT("CAnimBlueprintLog", "CAnim Blueprint Log"), InitOptions);
}

void FCAnimEditorModule::ShutdownModule()
{
	FKismetEditorUtilities::UnregisterAutoBlueprintNodeCreation(this);

	FEdGraphUtilities::UnregisterVisualNodeFactory(AnimGraphNodeFactory);
	FEdGraphUtilities::UnregisterVisualPinFactory(AnimGraphPinFactory);
	FEdGraphUtilities::UnregisterVisualPinConnectionFactory(AnimGraphPinConnectionFactory);

	MenuExtensibilityManager.Reset();
	ToolBarExtensibilityManager.Reset();
}

TSharedRef<ICAnimEditor> FCAnimEditorModule::CreateCAnimEditor( const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, class UCAnimBlueprintBase* InAnimBlueprint)
{
	TSharedRef< FCAnimEditor > NewAnimationBlueprintEditor( new FCAnimEditor() );
	NewAnimationBlueprintEditor->InitCAnimEditor( Mode, InitToolkitHost, InAnimBlueprint);
	return NewAnimationBlueprintEditor;
}

void FCAnimEditorModule::OnNewBlueprintCreated(UBlueprint* InBlueprint)
{
	if (ensure(InBlueprint->UbergraphPages.Num() > 0))
	{
		UEdGraph* EventGraph = InBlueprint->UbergraphPages[0];

		int32 SafeXPosition = 0;
		int32 SafeYPosition = 0;

		if (EventGraph->Nodes.Num() != 0)
		{
			SafeXPosition = EventGraph->Nodes[0]->NodePosX;
			SafeYPosition = EventGraph->Nodes[EventGraph->Nodes.Num() - 1]->NodePosY + EventGraph->Nodes[EventGraph->Nodes.Num() - 1]->NodeHeight + 100;
		}

		// add try get owner node
		//UK2Node_CallFunction* GetOwnerNode = NewObject<UK2Node_CallFunction>(EventGraph);
		//UFunction* MakeNodeFunction = UCAnimInstance::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UCAnimInstance, TryGetPawnOwner));
		//GetOwnerNode->CreateNewGuid();
		//GetOwnerNode->PostPlacedNewNode();
		//GetOwnerNode->SetFromFunction(MakeNodeFunction);
		//GetOwnerNode->SetFlags(RF_Transactional);
		//GetOwnerNode->AllocateDefaultPins();
		//GetOwnerNode->NodePosX = SafeXPosition;
		//GetOwnerNode->NodePosY = SafeYPosition;
		//UEdGraphSchema_K2::SetNodeMetaData(GetOwnerNode, FNodeMetadata::DefaultGraphNode);
		//GetOwnerNode->MakeAutomaticallyPlacedGhostNode();

		//EventGraph->AddNode(GetOwnerNode);
	}
}

#undef LOCTEXT_NAMESPACE
