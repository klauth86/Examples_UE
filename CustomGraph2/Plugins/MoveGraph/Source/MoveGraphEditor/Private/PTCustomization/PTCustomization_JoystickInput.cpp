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

FText ReleasedText = LOCTEXT("ReleasedText", "RELEASED");
FText PressedText = LOCTEXT("PressedText", "PRESSED");
FText HoldText = LOCTEXT("HoldText", "HOLD");

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

			// LEFT TRIGGERS

			+SGridPanel::Slot(0, 0)[SNew(SButton)
			.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftUpperTrigger)
			.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftUpperTrigger)]
			
			+ SGridPanel::Slot(0, 1)[SNew(SButton)
			.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftTrigger)
			.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftTrigger)]

			// LEFT STICK

			+SGridPanel::Slot(1, 0)[SNew(STextBlock).Text(FText::FromString("315"))]
			+ SGridPanel::Slot(2, 0)[SNew(STextBlock).Text(FText::FromString("0"))]
			+ SGridPanel::Slot(3, 0)[SNew(STextBlock).Text(FText::FromString("45"))]

			+ SGridPanel::Slot(1, 1)[SNew(STextBlock).Text(FText::FromString("270"))]
			+ SGridPanel::Slot(3, 1)[SNew(STextBlock).Text(FText::FromString("90"))]

			+ SGridPanel::Slot(1, 2)[SNew(STextBlock).Text(FText::FromString("225"))]
			+ SGridPanel::Slot(2, 2)[SNew(STextBlock).Text(FText::FromString("180"))]
			+ SGridPanel::Slot(3, 2)[SNew(STextBlock).Text(FText::FromString("135"))]

			+ SGridPanel::Slot(1, 3).ColumnSpan(3).HAlign(HAlign_Center)[
				SNew(SCheckBox)
					.Visibility_Raw(this, &FPTCustomization_JoystickInput::Visibility_LeftStick)
					.OnCheckStateChanged(this, &FPTCustomization_JoystickInput::OnCheckStateChanged_LeftStick)
					.IsChecked_Raw(this, &FPTCustomization_JoystickInput::IsChecked_LeftStick)
					.Content()[SNew(STextBlock).Text(FText::FromString("HOLD"))]
			]

			// RIGHT STICK

			+SGridPanel::Slot(4, 0)[SNew(STextBlock).Text(FText::FromString("315"))]
			+ SGridPanel::Slot(5, 0)[SNew(STextBlock).Text(FText::FromString("0"))]
			+ SGridPanel::Slot(6, 0)[SNew(STextBlock).Text(FText::FromString("45"))]

			+ SGridPanel::Slot(4, 1)[SNew(STextBlock).Text(FText::FromString("270"))]
			+ SGridPanel::Slot(6, 1)[SNew(STextBlock).Text(FText::FromString("90"))]

			+ SGridPanel::Slot(4, 2)[SNew(STextBlock).Text(FText::FromString("225"))]
			+ SGridPanel::Slot(5, 2)[SNew(STextBlock).Text(FText::FromString("180"))]
			+ SGridPanel::Slot(6, 2)[SNew(STextBlock).Text(FText::FromString("135"))]

			+ SGridPanel::Slot(4, 3).ColumnSpan(3).HAlign(HAlign_Center)[
				SNew(SCheckBox)
					.Visibility_Raw(this, &FPTCustomization_JoystickInput::Visibility_RightStick)
					.OnCheckStateChanged(this, &FPTCustomization_JoystickInput::OnCheckStateChanged_RightStick)
					.IsChecked_Raw(this, &FPTCustomization_JoystickInput::IsChecked_RightStick)
					.Content()[SNew(STextBlock).Text(FText::FromString("HOLD"))]
			]

			// RIGHT TRIGGERS

			+SGridPanel::Slot(7, 0)[SNew(STextBlock).Text(FText::FromString("RB"))]
			+ SGridPanel::Slot(7, 1)[SNew(STextBlock).Text(FText::FromString("RT"))]

			// XYAB

			+SGridPanel::Slot(8, 0)[SNew(STextBlock).Text(FText::FromString("X"))]
			+ SGridPanel::Slot(8, 1)[SNew(STextBlock).Text(FText::FromString("A"))]
			+ SGridPanel::Slot(9, 0)[SNew(STextBlock).Text(FText::FromString("Y"))]
			+ SGridPanel::Slot(9, 1)[SNew(STextBlock).Text(FText::FromString("B"))]
	];
}

FText FPTCustomization_JoystickInput::Text_LeftUpperTrigger() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftUpperTrigger_HOLD) return HoldText;

		if (!joystickInput->LeftUpperTrigger) return PressedText;

		return ReleasedText;
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftUpperTrigger() {

	return FReply::Handled();
}

FText FPTCustomization_JoystickInput::Text_LeftTrigger() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftTrigger_HOLD) return HoldText;

		if (!joystickInput->LeftTrigger) return PressedText;

		return ReleasedText;
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftTrigger() {

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