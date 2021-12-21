// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PTCustomization_Base.h"

class SJoytsickInputElement;
class SButton;
class SGridPanel;

struct FJoystickInput;

class FPTCustomization_JoystickInput : public FPTCustomization_Base
{
public:

	~FPTCustomization_JoystickInput() { grid.Reset(); }

	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	// End of IPropertyTypeCustomization interface

protected:

	FReply Add();

	FReply Remove(TSharedPtr<SJoytsickInputElement> element, TSharedPtr<SButton> button);

	void AddWidget(FJoystickInput* joystickInput, int32 index);

protected:

	TSharedPtr<SGridPanel> grid;
};