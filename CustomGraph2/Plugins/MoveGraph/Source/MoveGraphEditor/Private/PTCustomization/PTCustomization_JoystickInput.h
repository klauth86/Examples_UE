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

	FText GetStickButtonText(FString label, bool isHold) const;

	FSlateColor ColorAndOpacity_LeftUpperTrigger() const;
	FText Text_LeftUpperTrigger() const;
	FReply OnClicked_LeftUpperTrigger();
	bool IsEnabled_LeftUpperTrigger() const;

	FSlateColor ColorAndOpacity_LeftTrigger() const;
	FText Text_LeftTrigger() const;
	FReply OnClicked_LeftTrigger();
	bool IsEnabled_LeftTrigger() const;

	bool IsEnabled_LeftStick() const;
	void SwitchHold_LeftStick();

	FSlateColor ColorAndOpacity_LeftStick_315() const;
	FText Text_LeftStick_315() const;
	FReply OnClicked_LeftStick_315();

	FSlateColor ColorAndOpacity_LeftStick_0() const;
	FText Text_LeftStick_0() const;
	FReply OnClicked_LeftStick_0();

	FSlateColor ColorAndOpacity_LeftStick_45() const;
	FText Text_LeftStick_45() const;
	FReply OnClicked_LeftStick_45();

	FSlateColor ColorAndOpacity_LeftStick_270() const;
	FText Text_LeftStick_270() const;
	FReply OnClicked_LeftStick_270();

	FSlateColor ColorAndOpacity_LeftStick_90() const;
	FText Text_LeftStick_90() const;
	FReply OnClicked_LeftStick_90();

	FSlateColor ColorAndOpacity_LeftStick_225() const;
	FText Text_LeftStick_225() const;
	FReply OnClicked_LeftStick_225();

	FSlateColor ColorAndOpacity_LeftStick_180() const;
	FText Text_LeftStick_180() const;
	FReply OnClicked_LeftStick_180();

	FSlateColor ColorAndOpacity_LeftStick_135() const;
	FText Text_LeftStick_135() const;
	FReply OnClicked_LeftStick_135();

	bool IsEnabled_RightStick() const;
	void SwitchHold_RightStick();

	FSlateColor ColorAndOpacity_RightStick_315() const;
	FText Text_RightStick_315() const;
	FReply OnClicked_RightStick_315();

	FSlateColor ColorAndOpacity_RightStick_0() const;
	FText Text_RightStick_0() const;
	FReply OnClicked_RightStick_0();

	FSlateColor ColorAndOpacity_RightStick_45() const;
	FText Text_RightStick_45() const;
	FReply OnClicked_RightStick_45();

	FSlateColor ColorAndOpacity_RightStick_270() const;
	FText Text_RightStick_270() const;
	FReply OnClicked_RightStick_270();

	FSlateColor ColorAndOpacity_RightStick_90() const;
	FText Text_RightStick_90() const;
	FReply OnClicked_RightStick_90();

	FSlateColor ColorAndOpacity_RightStick_225() const;
	FText Text_RightStick_225() const;
	FReply OnClicked_RightStick_225();

	FSlateColor ColorAndOpacity_RightStick_180() const;
	FText Text_RightStick_180() const;
	FReply OnClicked_RightStick_180();

	FSlateColor ColorAndOpacity_RightStick_135() const;
	FText Text_RightStick_135() const;
	FReply OnClicked_RightStick_135();

	FSlateColor ColorAndOpacity_RightUpperTrigger() const;
	FText Text_RightUpperTrigger() const;
	FReply OnClicked_RightUpperTrigger();
	bool IsEnabled_RightUpperTrigger() const;

	FSlateColor ColorAndOpacity_RightTrigger() const;
	FText Text_RightTrigger() const;
	FReply OnClicked_RightTrigger();
	bool IsEnabled_RightTrigger() const;

	FSlateColor ColorAndOpacity_X() const;
	FText Text_X() const;
	FReply OnClicked_X();
	bool IsEnabled_X() const;

	FSlateColor ColorAndOpacity_A() const;
	FText Text_A() const;
	FReply OnClicked_A();
	bool IsEnabled_A() const;

	FSlateColor ColorAndOpacity_Y() const;
	FText Text_Y() const;
	FReply OnClicked_Y();
	bool IsEnabled_Y() const;

	FSlateColor ColorAndOpacity_B() const;
	FText Text_B() const;
	FReply OnClicked_B();
	bool IsEnabled_B() const;
};