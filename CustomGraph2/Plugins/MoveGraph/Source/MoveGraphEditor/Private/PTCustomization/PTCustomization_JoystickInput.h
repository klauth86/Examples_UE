// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PTCustomization_Base.h"

class FPTCustomization_JoystickInput : public FPTCustomization_Base
{
public:

	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	// End of IPropertyTypeCustomization interface

protected:

	FText Text_LeftUpperTrigger() const;

	FReply OnClicked_LeftUpperTrigger();

	FText Text_LeftTrigger() const;

	FReply OnClicked_LeftTrigger();

	FText Text_RightUpperTrigger() const;

	FReply OnClicked_RightUpperTrigger();

	FText Text_RightTrigger() const;

	FReply OnClicked_RightTrigger();

	EVisibility Visibility_LeftStick() const;

	void OnCheckStateChanged_LeftStick(ECheckBoxState NewState);

	ECheckBoxState IsChecked_LeftStick() const;

	EVisibility Visibility_RightStick() const;

	void OnCheckStateChanged_RightStick(ECheckBoxState NewState);

	ECheckBoxState IsChecked_RightStick() const;
};