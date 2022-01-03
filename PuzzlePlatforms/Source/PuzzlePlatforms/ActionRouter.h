#pragma once

#include "Delegates/DelegateCombinations.h"
#include "Containers/UnrealString.h"

DECLARE_DELEGATE(FToggleEvent);
DECLARE_DELEGATE_OneParam(FJoinEvent, const FString& Address);

namespace ActionRouter
{
	extern FToggleEvent OnHost;
	extern FJoinEvent OnJoin;
	extern FToggleEvent OnLeave;
	extern FToggleEvent OnQuit;

	extern FToggleEvent OnWidgetConstruct;
	extern FToggleEvent OnWidgetDestruct;
}