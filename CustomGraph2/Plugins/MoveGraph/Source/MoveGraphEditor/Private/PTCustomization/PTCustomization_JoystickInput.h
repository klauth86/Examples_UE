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

	FText Text_LeftStick_315() const;
	FReply OnClicked_LeftStick_315();

	FText Text_LeftStick_0() const;
	FReply OnClicked_LeftStick_0();

	FText Text_LeftStick_45() const;
	FReply OnClicked_LeftStick_45();

	FText Text_LeftStick_270() const;
	FReply OnClicked_LeftStick_270();

	FText Text_LeftStick_90() const;
	FReply OnClicked_LeftStick_90();

	FText Text_LeftStick_225() const;
	FReply OnClicked_LeftStick_225();

	FText Text_LeftStick_180() const;
	FReply OnClicked_LeftStick_180();

	FText Text_LeftStick_135() const;
	FReply OnClicked_LeftStick_135();

	FText Text_RightStick_315() const;
	FReply OnClicked_RightStick_315();

	FText Text_RightStick_0() const;
	FReply OnClicked_RightStick_0();

	FText Text_RightStick_45() const;
	FReply OnClicked_RightStick_45();

	FText Text_RightStick_270() const;
	FReply OnClicked_RightStick_270();

	FText Text_RightStick_90() const;
	FReply OnClicked_RightStick_90();

	FText Text_RightStick_225() const;
	FReply OnClicked_RightStick_225();

	FText Text_RightStick_180() const;
	FReply OnClicked_RightStick_180();

	FText Text_RightStick_135() const;
	FReply OnClicked_RightStick_135();

	EVisibility Visibility_LeftStick() const;
	void OnCheckStateChanged_LeftStick(ECheckBoxState NewState);
	ECheckBoxState IsChecked_LeftStick() const;

	EVisibility Visibility_RightStick() const;
	void OnCheckStateChanged_RightStick(ECheckBoxState NewState);
	ECheckBoxState IsChecked_RightStick() const;
};