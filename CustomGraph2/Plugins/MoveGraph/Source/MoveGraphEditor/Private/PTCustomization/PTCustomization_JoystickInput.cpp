// Fill out your copyright notice in the Description page of Project Settings.

#include "PTCustomization_JoystickInput.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "JoystickInput.h"

#define LOCTEXT_NAMESPACE "PropertyTypeCustomization_JoystickInput.h"

FText UndeterminedText = LOCTEXT("UndeterminedText", "???");

FText ReleasedText = LOCTEXT("ReleasedText", "R");
FText PressedText = LOCTEXT("PressedText", "P");
FText HoldText = LOCTEXT("HoldText", "H");

TSharedRef<IPropertyTypeCustomization> FPTCustomization_JoystickInput::MakeInstance()
{
	return MakeShareable(new FPTCustomization_JoystickInput);
}

void FPTCustomization_JoystickInput::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//FPTCustomization_Base::CustomizeHeader(PropertyHandle, HeaderRow, CustomizationUtils);
}

void FPTCustomization_JoystickInput::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) {
	StructPropertyHandle = PropertyHandle;

	ChildBuilder.AddCustomRow(LOCTEXT("SearchString", "Joystick Input"))[
		SNew(SGridPanel)
			.FillColumn(0, 1)

			.FillColumn(1, 1)
			.FillColumn(2, 1)
			.FillColumn(3, 1)

			.FillColumn(4, 1)
			.FillColumn(5, 1)
			.FillColumn(6, 1)

			.FillColumn(7, 1)

			.FillColumn(8, 1)
			.FillColumn(9, 1)

			.FillRow(0, 1)
			.FillRow(1, 1)
			.FillRow(2, 1)
			.FillRow(3, 1)
			.FillRow(4, 1)

			// LEFT TRIGGERS

			+SGridPanel::Slot(0, 0).HAlign(HAlign_Center)[SNew(STextBlock).Text(FText::FromString("Left triggers:"))]

			+ SGridPanel::Slot(0, 1).HAlign(HAlign_Center)[SNew(SButton)
			.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftUpperTrigger)
			.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftUpperTrigger)]

			+ SGridPanel::Slot(0, 2).HAlign(HAlign_Center)[SNew(SButton)
			.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftTrigger)
			.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftTrigger)]

			// LEFT STICK

			+SGridPanel::Slot(1, 0).ColumnSpan(3).HAlign(HAlign_Center)[SNew(STextBlock).Text(FText::FromString("Left stick:"))]

			+SGridPanel::Slot(1, 1)[SNew(STextBlock).Text(FText::FromString("315"))]
			+ SGridPanel::Slot(2, 1)[SNew(STextBlock).Text(FText::FromString("0"))]
			+ SGridPanel::Slot(3, 1)[SNew(STextBlock).Text(FText::FromString("45"))]

			+ SGridPanel::Slot(1, 2)[SNew(STextBlock).Text(FText::FromString("270"))]
			+ SGridPanel::Slot(3, 2)[SNew(STextBlock).Text(FText::FromString("90"))]

			+ SGridPanel::Slot(1, 3)[SNew(STextBlock).Text(FText::FromString("225"))]
			+ SGridPanel::Slot(2, 3)[SNew(STextBlock).Text(FText::FromString("180"))]
			+ SGridPanel::Slot(3, 3)[SNew(STextBlock).Text(FText::FromString("135"))]

			+ SGridPanel::Slot(1, 4).ColumnSpan(3).HAlign(HAlign_Center)[
				SNew(SCheckBox)
					.Visibility_Raw(this, &FPTCustomization_JoystickInput::Visibility_LeftStick)
					.OnCheckStateChanged(this, &FPTCustomization_JoystickInput::OnCheckStateChanged_LeftStick)
					.IsChecked_Raw(this, &FPTCustomization_JoystickInput::IsChecked_LeftStick)
					.Content()[SNew(STextBlock).Text(FText::FromString("HOLD"))]
			]

			// RIGHT STICK
					
			+SGridPanel::Slot(4, 0).ColumnSpan(3).HAlign(HAlign_Center)[SNew(STextBlock).Text(FText::FromString("Right stick:"))]

			+SGridPanel::Slot(4, 1)[SNew(STextBlock).Text(FText::FromString("315"))]
			+ SGridPanel::Slot(5, 1)[SNew(STextBlock).Text(FText::FromString("0"))]
			+ SGridPanel::Slot(6, 1)[SNew(STextBlock).Text(FText::FromString("45"))]

			+ SGridPanel::Slot(4, 2)[SNew(STextBlock).Text(FText::FromString("270"))]
			+ SGridPanel::Slot(6, 2)[SNew(STextBlock).Text(FText::FromString("90"))]

			+ SGridPanel::Slot(4, 3)[SNew(STextBlock).Text(FText::FromString("225"))]
			+ SGridPanel::Slot(5, 3)[SNew(STextBlock).Text(FText::FromString("180"))]
			+ SGridPanel::Slot(6, 3)[SNew(STextBlock).Text(FText::FromString("135"))]

			+ SGridPanel::Slot(4, 4).ColumnSpan(3).HAlign(HAlign_Center)[
				SNew(SCheckBox)
					.Visibility_Raw(this, &FPTCustomization_JoystickInput::Visibility_RightStick)
					.OnCheckStateChanged(this, &FPTCustomization_JoystickInput::OnCheckStateChanged_RightStick)
					.IsChecked_Raw(this, &FPTCustomization_JoystickInput::IsChecked_RightStick)
					.Content()[SNew(STextBlock).Text(FText::FromString("HOLD"))]
			]

			// RIGHT TRIGGERS

			+SGridPanel::Slot(7, 0).HAlign(HAlign_Center)[SNew(STextBlock).Text(FText::FromString("Right triggers:"))]

			+ SGridPanel::Slot(7, 1).HAlign(HAlign_Center)[SNew(SButton)
			.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightUpperTrigger)
			.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightUpperTrigger)]

			+ SGridPanel::Slot(7, 2).HAlign(HAlign_Center)[SNew(SButton)
			.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightTrigger)
			.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightTrigger)]

			// XYAB

			+SGridPanel::Slot(8, 0).ColumnSpan(2).HAlign(HAlign_Center)[SNew(STextBlock).Text(FText::FromString("Buttons:"))]

			+SGridPanel::Slot(8, 1)[SNew(STextBlock).Text(FText::FromString("X"))]
			+ SGridPanel::Slot(8, 2)[SNew(STextBlock).Text(FText::FromString("A"))]
			+ SGridPanel::Slot(9, 1)[SNew(STextBlock).Text(FText::FromString("Y"))]
			+ SGridPanel::Slot(9, 2)[SNew(STextBlock).Text(FText::FromString("B"))]
	];
}

FText FPTCustomization_JoystickInput::Text_LeftUpperTrigger() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftUpperTrigger_HOLD) return HoldText;

		if (joystickInput->LeftUpperTrigger) return PressedText;

		return ReleasedText;
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftUpperTrigger() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftUpperTrigger_HOLD_IN_PARENT) {
			if (!joystickInput->LeftUpperTrigger_HOLD) {
				joystickInput->LeftUpperTrigger_HOLD = true;
			}
			else {
				joystickInput->LeftUpperTrigger_HOLD = false;
			}
		}
		else {
			if (!joystickInput->LeftUpperTrigger)
			{
				joystickInput->LeftUpperTrigger = true;
			}
			else if (!joystickInput->LeftUpperTrigger_HOLD) {
				joystickInput->LeftUpperTrigger_HOLD = true;
			}
			else {
				joystickInput->LeftUpperTrigger = false;
				joystickInput->LeftUpperTrigger_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

FText FPTCustomization_JoystickInput::Text_LeftTrigger() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftTrigger_HOLD) return HoldText;

		if (joystickInput->LeftTrigger) return PressedText;

		return ReleasedText;
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftTrigger() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftTrigger_HOLD_IN_PARENT)
		{
			if (!joystickInput->LeftTrigger_HOLD)
			{
				joystickInput->LeftTrigger_HOLD = true;
			}
			else
			{
				joystickInput->LeftTrigger_HOLD = false;
			}
		}
		else
		{
			if (!joystickInput->LeftTrigger)
			{
				joystickInput->LeftTrigger = true;
			}
			else if (!joystickInput->LeftTrigger_HOLD)
			{
				joystickInput->LeftTrigger_HOLD = true;
			}
			else
			{
				joystickInput->LeftTrigger = false;
				joystickInput->LeftTrigger_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

FText FPTCustomization_JoystickInput::Text_RightUpperTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightUpperTrigger_HOLD) return HoldText;

		if (joystickInput->RightUpperTrigger) return PressedText;

		return ReleasedText;
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightUpperTrigger()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightUpperTrigger_HOLD_IN_PARENT)
		{
			if (!joystickInput->RightUpperTrigger_HOLD)
			{
				joystickInput->RightUpperTrigger_HOLD = true;
			}
			else
			{
				joystickInput->RightUpperTrigger_HOLD = false;
			}
		}
		else
		{
			if (!joystickInput->RightUpperTrigger)
			{
				joystickInput->RightUpperTrigger = true;
			}
			else if (!joystickInput->RightUpperTrigger_HOLD)
			{
				joystickInput->RightUpperTrigger_HOLD = true;
			}
			else
			{
				joystickInput->RightUpperTrigger = false;
				joystickInput->RightUpperTrigger_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

FText FPTCustomization_JoystickInput::Text_RightTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightTrigger_HOLD) return HoldText;

		if (joystickInput->RightTrigger) return PressedText;

		return ReleasedText;
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightTrigger()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightTrigger_HOLD_IN_PARENT)
		{
			if (!joystickInput->RightTrigger_HOLD)
			{
				joystickInput->RightTrigger_HOLD = true;
			}
			else
			{
				joystickInput->RightTrigger_HOLD = false;
			}
		}
		else
		{
			if (!joystickInput->RightTrigger)
			{
				joystickInput->RightTrigger = true;
			}
			else if (!joystickInput->RightTrigger_HOLD)
			{
				joystickInput->RightTrigger_HOLD = true;
			}
			else
			{
				joystickInput->RightTrigger = false;
				joystickInput->RightTrigger_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

EVisibility FPTCustomization_JoystickInput::Visibility_LeftStick() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>()) {
		return joystickInput->HasLeftStick() || joystickInput->LeftStick_HOLD_IN_PARENT ? EVisibility::Visible : EVisibility::Hidden;
	}

	return EVisibility::Hidden;
}

void FPTCustomization_JoystickInput::OnCheckStateChanged_LeftStick(ECheckBoxState NewState) {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		joystickInput->LeftStick_HOLD = NewState == ECheckBoxState::Checked;
	}
}

ECheckBoxState FPTCustomization_JoystickInput::IsChecked_LeftStick() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return joystickInput->LeftStick_HOLD ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	return ECheckBoxState::Undetermined;
}

EVisibility FPTCustomization_JoystickInput::Visibility_RightStick() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return joystickInput->HasRightStick() || joystickInput->RightStick_HOLD_IN_PARENT ? EVisibility::Visible : EVisibility::Hidden;
	}

	return EVisibility::Hidden;
}

void FPTCustomization_JoystickInput::OnCheckStateChanged_RightStick(ECheckBoxState NewState) {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		joystickInput->RightStick_HOLD = NewState == ECheckBoxState::Checked;
	}
}

ECheckBoxState FPTCustomization_JoystickInput::IsChecked_RightStick() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return joystickInput->RightStick_HOLD ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	return ECheckBoxState::Undetermined;
}

#undef LOCTEXT_NAMESPACE