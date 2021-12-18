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

FText PressedText = LOCTEXT("PressedText", "P");
FText HoldText = LOCTEXT("HoldText", "H");

FLinearColor InactiveColor(0.25f, 0.25f, 0.25f, 1);

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
			.FillColumn(0, 0)
			.FillColumn(1, 1)
			.FillColumn(2, 0)
			.FillColumn(3, 1)
			.FillColumn(4, 0)
			.FillColumn(5, 1)
			.FillColumn(6, 0)
			.FillColumn(7, 1)
			.FillColumn(8, 0)

			+ SGridPanel::Slot(0, 0).VAlign(VAlign_Center)[
				SNew(SGridPanel)

					+ SGridPanel::Slot(0, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftUpperTrigger)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftUpperTrigger)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftUpperTrigger)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftUpperTrigger)]

					+ SGridPanel::Slot(0, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftTrigger)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftTrigger)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftTrigger)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftTrigger)]
			]

			+ SGridPanel::Slot(2, 0).VAlign(VAlign_Center)[
				SNew(SGridPanel)

					+ SGridPanel::Slot(0, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_315)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_315)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_315)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]
					+ SGridPanel::Slot(1, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_0)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_0)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_0)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]
					+ SGridPanel::Slot(2, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_45)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_45)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_45)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]

					+ SGridPanel::Slot(0, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_270)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_270)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_270)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]
					+ SGridPanel::Slot(2, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_90)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_90)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_90)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]

					+ SGridPanel::Slot(0, 2)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_225)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_225)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_225)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]
					+ SGridPanel::Slot(1, 2)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_180)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_180)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_180)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]
					+ SGridPanel::Slot(2, 2)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_135)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_LeftStick_135)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_LeftStick_135)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_LeftStick)]
			]

			+ SGridPanel::Slot(4, 0).VAlign(VAlign_Center)[
				SNew(SGridPanel)

					+ SGridPanel::Slot(0, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_315)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_315)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_315)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]
					+ SGridPanel::Slot(1, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_0)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_0)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_0)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]
					+ SGridPanel::Slot(2, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_45)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_45)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_45)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]

					+ SGridPanel::Slot(0, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_270)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_270)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_270)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]
					+ SGridPanel::Slot(2, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_90)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_90)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_90)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]

					+ SGridPanel::Slot(0, 2)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_225)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_225)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_225)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]
					+ SGridPanel::Slot(1, 2)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_180)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_180)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_180)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]
					+ SGridPanel::Slot(2, 2)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_135)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightStick_135)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightStick_135)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightStick)]
			]

			+ SGridPanel::Slot(6, 0).VAlign(VAlign_Center)[
				SNew(SGridPanel)

					+ SGridPanel::Slot(0, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightUpperTrigger)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightUpperTrigger)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightUpperTrigger)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightUpperTrigger)]

					+ SGridPanel::Slot(0, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_RightTrigger)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_RightTrigger)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_RightTrigger)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_RightTrigger)]
			]

			+ SGridPanel::Slot(8, 0).VAlign(VAlign_Center)[
				SNew(SGridPanel)

					+ SGridPanel::Slot(0, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_X)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_X)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_X)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_X)]
					+ SGridPanel::Slot(0, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_A)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_A)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_A)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_A)]
					+ SGridPanel::Slot(1, 0)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_Y)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_Y)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_Y)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_Y)]
					+ SGridPanel::Slot(1, 1)[SNew(SButton)
					.ButtonColorAndOpacity_Raw(this, &FPTCustomization_JoystickInput::ColorAndOpacity_B)
					.Text_Raw(this, &FPTCustomization_JoystickInput::Text_B)
					.OnClicked_Raw(this, &FPTCustomization_JoystickInput::OnClicked_B)
					.IsEnabled_Raw(this, &FPTCustomization_JoystickInput::IsEnabled_B)]
			]
	];
}

FText FPTCustomization_JoystickInput::GetStickButtonText(FString label, bool isHold) const {
	return FText::FromString(label + (isHold ? "(H)" : ""));
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftUpperTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftUpperTrigger) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftUpperTrigger() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("LB", joystickInput->LeftUpperTrigger_HOLD);
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

bool FPTCustomization_JoystickInput::IsEnabled_LeftUpperTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->LeftUpperTrigger_HOLD_IN_PARENT;
	}

	return false;
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftTrigger) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftTrigger() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("LT", joystickInput->LeftTrigger_HOLD);
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

bool FPTCustomization_JoystickInput::IsEnabled_LeftTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->LeftTrigger_HOLD_IN_PARENT;
	}

	return false;
}

bool FPTCustomization_JoystickInput::IsEnabled_LeftStick() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->LeftStick_HOLD_IN_PARENT;
	}

	return false;
}

void FPTCustomization_JoystickInput::SwitchHold_LeftStick()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_HOLD)
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_HOLD = false;
		}
		else
		{
			joystickInput->LeftStick_HOLD = true;
		}
	}
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_315() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_315) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_315() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("315", joystickInput->LeftStick_315 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_315() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_315;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_315 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_0() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_0) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_0() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("0", joystickInput->LeftStick_0 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_0() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_0;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_0 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_45() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_45) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_45() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("45", joystickInput->LeftStick_45 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_45() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_45;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_45 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_270() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_270) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_270() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("270", joystickInput->LeftStick_270 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_270() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_270;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_270 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_90() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_90) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_90() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("90", joystickInput->LeftStick_90 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_90() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_90;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_90 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_225() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_225) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_225() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("225", joystickInput->LeftStick_225 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_225() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_225;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_225 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_180() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_180) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_180() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("180", joystickInput->LeftStick_180 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_180() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_180;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_180 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_LeftStick_135() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->LeftStick_135) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_LeftStick_135() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("135", joystickInput->LeftStick_135 && joystickInput->LeftStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_LeftStick_135() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->LeftStick_135;
		if (prevState)
		{
			SwitchHold_LeftStick();
		}
		else
		{
			joystickInput->ResetLeftStick();
			joystickInput->LeftStick_135 = true;
		}
	}

	return FReply::Handled();
}

bool FPTCustomization_JoystickInput::IsEnabled_RightStick() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->RightStick_HOLD_IN_PARENT;
	}

	return false;
}

void FPTCustomization_JoystickInput::SwitchHold_RightStick() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_HOLD)
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_HOLD = false;
		}
		else
		{
			joystickInput->RightStick_HOLD = true;
		}
	}
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_315() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_315) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_315() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("315", joystickInput->RightStick_315 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_315()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_315;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_315 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_0() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_0) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_0() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("0", joystickInput->RightStick_0 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_0()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_0;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_0 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_45() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_45) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_45() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("45", joystickInput->RightStick_45 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_45()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_45;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_45 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_270() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_270) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_270() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("270", joystickInput->RightStick_270 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_270()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_270;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_270 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_90() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_90) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_90() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("90", joystickInput->RightStick_90 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_90()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_90;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_90 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_225() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_225) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_225() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("225", joystickInput->RightStick_225 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_225()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_225;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_225 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_180() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_180) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_180() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("180", joystickInput->RightStick_180 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_180()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_180;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_180 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightStick_135() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightStick_135) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightStick_135() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("135", joystickInput->RightStick_135 && joystickInput->RightStick_HOLD);
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_RightStick_135()
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		const bool prevState = joystickInput->RightStick_135;
		if (prevState)
		{
			SwitchHold_RightStick();
		}
		else
		{
			joystickInput->ResetRightStick();
			joystickInput->RightStick_135 = true;
		}
	}

	return FReply::Handled();
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightUpperTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightUpperTrigger) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightUpperTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("LB", joystickInput->RightUpperTrigger_HOLD);
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

bool FPTCustomization_JoystickInput::IsEnabled_RightUpperTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->RightUpperTrigger_HOLD_IN_PARENT;
	}

	return false;
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_RightTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->RightTrigger) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_RightTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return GetStickButtonText("LT", joystickInput->RightTrigger_HOLD);
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

bool FPTCustomization_JoystickInput::IsEnabled_RightTrigger() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->RightTrigger_HOLD_IN_PARENT;
	}

	return false;
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_X() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->X) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_X() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->X_HOLD) return HoldText;

		if (joystickInput->X) return PressedText;

		return FText::FromString("X");
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_X() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->X_HOLD_IN_PARENT)
		{
			if (!joystickInput->X_HOLD)
			{
				joystickInput->X_HOLD = true;
			}
			else
			{
				joystickInput->X_HOLD = false;
			}
		}
		else
		{
			if (!joystickInput->X)
			{
				joystickInput->X = true;
			}
			else if (!joystickInput->X_HOLD)
			{
				joystickInput->X_HOLD = true;
			}
			else
			{
				joystickInput->X = false;
				joystickInput->X_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

bool FPTCustomization_JoystickInput::IsEnabled_X() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->X_HOLD_IN_PARENT;
	}

	return false;
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_A() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->A) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_A() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->A_HOLD) return HoldText;

		if (joystickInput->A) return PressedText;

		return FText::FromString("A");
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_A() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->A_HOLD_IN_PARENT)
		{
			if (!joystickInput->A_HOLD)
			{
				joystickInput->A_HOLD = true;
			}
			else
			{
				joystickInput->A_HOLD = false;
			}
		}
		else
		{
			if (!joystickInput->A)
			{
				joystickInput->A = true;
			}
			else if (!joystickInput->A_HOLD)
			{
				joystickInput->A_HOLD = true;
			}
			else
			{
				joystickInput->A = false;
				joystickInput->A_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

bool FPTCustomization_JoystickInput::IsEnabled_A() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->A_HOLD_IN_PARENT;
	}

	return false;
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_Y() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->Y) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_Y() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->Y_HOLD) return HoldText;

		if (joystickInput->Y) return PressedText;

		return FText::FromString("Y");
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_Y() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->Y_HOLD_IN_PARENT)
		{
			if (!joystickInput->Y_HOLD)
			{
				joystickInput->Y_HOLD = true;
			}
			else
			{
				joystickInput->Y_HOLD = false;
			}
		}
		else
		{
			if (!joystickInput->Y)
			{
				joystickInput->Y = true;
			}
			else if (!joystickInput->Y_HOLD)
			{
				joystickInput->Y_HOLD = true;
			}
			else
			{
				joystickInput->Y = false;
				joystickInput->Y_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

bool FPTCustomization_JoystickInput::IsEnabled_Y() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->Y_HOLD_IN_PARENT;
	}

	return false;
}

FSlateColor FPTCustomization_JoystickInput::ColorAndOpacity_B() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->B) return FSlateColor(FLinearColor(1, 1, 1, 1));
	}

	return FSlateColor(InactiveColor);
}

FText FPTCustomization_JoystickInput::Text_B() const {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->B_HOLD) return HoldText;

		if (joystickInput->B) return PressedText;

		return FText::FromString("B");
	}

	return UndeterminedText;
}

FReply FPTCustomization_JoystickInput::OnClicked_B() {
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		if (joystickInput->B_HOLD_IN_PARENT)
		{
			if (!joystickInput->B_HOLD)
			{
				joystickInput->B_HOLD = true;
			}
			else
			{
				joystickInput->B_HOLD = false;
			}
		}
		else
		{
			if (!joystickInput->B)
			{
				joystickInput->B = true;
			}
			else if (!joystickInput->B_HOLD)
			{
				joystickInput->B_HOLD = true;
			}
			else
			{
				joystickInput->B = false;
				joystickInput->B_HOLD = false;
			}
		}
	}

	return FReply::Handled();
}

bool FPTCustomization_JoystickInput::IsEnabled_B() const
{
	if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	{
		return !joystickInput->B_HOLD_IN_PARENT;
	}

	return false;
}

#undef LOCTEXT_NAMESPACE