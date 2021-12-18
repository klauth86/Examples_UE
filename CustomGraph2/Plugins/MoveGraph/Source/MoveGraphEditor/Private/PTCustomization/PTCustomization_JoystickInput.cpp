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
FLinearColor ActiveColor(1, 1, 1, 1);

class SJoytsickInputElement : public SGridPanel
{
public:

	FJoystickInputElement* InputElement;

	void Construct(FArguments args, FJoystickInputElement* inputElement)
	{
		InputElement = inputElement;

		SetColumnFill(0, 0);
		SetColumnFill(1, 1);
		SetColumnFill(2, 0);
		SetColumnFill(3, 1);
		SetColumnFill(4, 0);
		SetColumnFill(5, 1);
		SetColumnFill(6, 0);
		SetColumnFill(7, 1);
		SetColumnFill(8, 0);

		AddSlot(0, 0).VAlign(VAlign_Center)[
			SNew(SGridPanel)

				+ SGridPanel::Slot(0, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftUpperTrigger)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftUpperTrigger)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftUpperTrigger)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftUpperTrigger)]

				+ SGridPanel::Slot(0, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftTrigger)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftTrigger)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftTrigger)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftTrigger)]
		];

		AddSlot(2, 0).VAlign(VAlign_Center)[
			SNew(SGridPanel)

				+ SGridPanel::Slot(0, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_315)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_315)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_315)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]
				+ SGridPanel::Slot(1, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_0)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_0)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_0)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]
				+ SGridPanel::Slot(2, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_45)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_45)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_45)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]

				+ SGridPanel::Slot(0, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_270)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_270)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_270)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]
				+ SGridPanel::Slot(2, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_90)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_90)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_90)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]

				+ SGridPanel::Slot(0, 2)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_225)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_225)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_225)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]
				+ SGridPanel::Slot(1, 2)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_180)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_180)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_180)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]
				+ SGridPanel::Slot(2, 2)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_LeftStick_135)
				.Text_Raw(this, &SJoytsickInputElement::Text_LeftStick_135)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_LeftStick_135)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_LeftStick)]
		];

		AddSlot(4, 0).VAlign(VAlign_Center)[
			SNew(SGridPanel)

				+ SGridPanel::Slot(0, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_315)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_315)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_315)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]
				+ SGridPanel::Slot(1, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_0)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_0)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_0)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]
				+ SGridPanel::Slot(2, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_45)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_45)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_45)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]

				+ SGridPanel::Slot(0, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_270)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_270)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_270)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]
				+ SGridPanel::Slot(2, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_90)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_90)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_90)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]

				+ SGridPanel::Slot(0, 2)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_225)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_225)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_225)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]
				+ SGridPanel::Slot(1, 2)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_180)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_180)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_180)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]
				+ SGridPanel::Slot(2, 2)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightStick_135)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightStick_135)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightStick_135)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightStick)]
		];

		AddSlot(6, 0).VAlign(VAlign_Center)[
			SNew(SGridPanel)

				+ SGridPanel::Slot(0, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightUpperTrigger)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightUpperTrigger)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightUpperTrigger)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightUpperTrigger)]

				+ SGridPanel::Slot(0, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_RightTrigger)
				.Text_Raw(this, &SJoytsickInputElement::Text_RightTrigger)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_RightTrigger)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_RightTrigger)]
		];

		AddSlot(8, 0).VAlign(VAlign_Center)[
			SNew(SGridPanel)

				+ SGridPanel::Slot(0, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_X)
				.Text_Raw(this, &SJoytsickInputElement::Text_X)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_X)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_X)]
				+ SGridPanel::Slot(0, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_A)
				.Text_Raw(this, &SJoytsickInputElement::Text_A)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_A)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_A)]
				+ SGridPanel::Slot(1, 0)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_Y)
				.Text_Raw(this, &SJoytsickInputElement::Text_Y)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_Y)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_Y)]
				+ SGridPanel::Slot(1, 1)[SNew(SButton)
				.ButtonColorAndOpacity_Raw(this, &SJoytsickInputElement::ColorAndOpacity_B)
				.Text_Raw(this, &SJoytsickInputElement::Text_B)
				.OnClicked_Raw(this, &SJoytsickInputElement::OnClicked_B)
				.IsEnabled_Raw(this, &SJoytsickInputElement::IsEnabled_B)]
		];
	}

	FText GetStickButtonText(FString label, bool isHold) const
	{
		return FText::FromString(label + (isHold ? "(H)" : ""));
	}

	FSlateColor ColorAndOpacity_LeftUpperTrigger() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftUpperTrigger) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftUpperTrigger() const
	{
		if (InputElement)
		{
			return GetStickButtonText("LB", InputElement->Flags.LeftUpperTrigger_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftUpperTrigger()
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftUpperTrigger_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.LeftUpperTrigger_HOLD)
				{
					InputElement->Flags.LeftUpperTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.LeftUpperTrigger_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.LeftUpperTrigger)
				{
					InputElement->Flags.LeftUpperTrigger = true;
				}
				else if (!InputElement->Flags.LeftUpperTrigger_HOLD)
				{
					InputElement->Flags.LeftUpperTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.LeftUpperTrigger = false;
					InputElement->Flags.LeftUpperTrigger_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_LeftUpperTrigger() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.LeftUpperTrigger_HOLD_IN_PARENT;
		}

		return false;
	}

	FSlateColor ColorAndOpacity_LeftTrigger() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftTrigger) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftTrigger() const
	{
		if (InputElement)
		{
			return GetStickButtonText("LT", InputElement->Flags.LeftTrigger_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftTrigger()
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftTrigger_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.LeftTrigger_HOLD)
				{
					InputElement->Flags.LeftTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.LeftTrigger_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.LeftTrigger)
				{
					InputElement->Flags.LeftTrigger = true;
				}
				else if (!InputElement->Flags.LeftTrigger_HOLD)
				{
					InputElement->Flags.LeftTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.LeftTrigger = false;
					InputElement->Flags.LeftTrigger_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_LeftTrigger() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.LeftTrigger_HOLD_IN_PARENT;
		}

		return false;
	}

	bool IsEnabled_LeftStick() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.LeftStick_HOLD_IN_PARENT;
		}

		return false;
	}

	void SwitchHold_LeftStick()
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_HOLD)
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_HOLD = false;
			}
			else
			{
				InputElement->Flags.LeftStick_HOLD = true;
			}
		}
	}

	FSlateColor ColorAndOpacity_LeftStick_315() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_315) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_315() const
	{
		if (InputElement)
		{
			return GetStickButtonText("315", InputElement->Flags.LeftStick_315 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_315()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_315;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_315 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_LeftStick_0() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_0) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_0() const
	{
		if (InputElement)
		{
			return GetStickButtonText("0", InputElement->Flags.LeftStick_0 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_0()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_0;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_0 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_LeftStick_45() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_45) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_45() const
	{
		if (InputElement)
		{
			return GetStickButtonText("45", InputElement->Flags.LeftStick_45 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_45()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_45;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_45 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_LeftStick_270() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_270) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_270() const
	{
		if (InputElement)
		{
			return GetStickButtonText("270", InputElement->Flags.LeftStick_270 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_270()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_270;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_270 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_LeftStick_90() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_90) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_90() const
	{
		if (InputElement)
		{
			return GetStickButtonText("90", InputElement->Flags.LeftStick_90 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_90()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_90;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_90 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_LeftStick_225() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_225) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_225() const
	{
		if (InputElement)
		{
			return GetStickButtonText("225", InputElement->Flags.LeftStick_225 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_225()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_225;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_225 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_LeftStick_180() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_180) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_180() const
	{
		if (InputElement)
		{
			return GetStickButtonText("180", InputElement->Flags.LeftStick_180 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_180()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_180;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_180 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_LeftStick_135() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.LeftStick_135) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_LeftStick_135() const
	{
		if (InputElement)
		{
			return GetStickButtonText("135", InputElement->Flags.LeftStick_135 && InputElement->Flags.LeftStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_LeftStick_135()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.LeftStick_135;
			if (prevState)
			{
				SwitchHold_LeftStick();
			}
			else
			{
				InputElement->Flags.ResetLeftStick();
				InputElement->Flags.LeftStick_135 = true;
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_RightStick() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.RightStick_HOLD_IN_PARENT;
		}

		return false;
	}

	void SwitchHold_RightStick()
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_HOLD)
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_HOLD = false;
			}
			else
			{
				InputElement->Flags.RightStick_HOLD = true;
			}
		}
	}

	FSlateColor ColorAndOpacity_RightStick_315() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_315) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_315() const
	{
		if (InputElement)
		{
			return GetStickButtonText("315", InputElement->Flags.RightStick_315 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_315()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_315;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_315 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightStick_0() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_0) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_0() const
	{
		if (InputElement)
		{
			return GetStickButtonText("0", InputElement->Flags.RightStick_0 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_0()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_0;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_0 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightStick_45() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_45) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_45() const
	{
		if (InputElement)
		{
			return GetStickButtonText("45", InputElement->Flags.RightStick_45 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_45()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_45;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_45 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightStick_270() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_270) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_270() const
	{
		if (InputElement)
		{
			return GetStickButtonText("270", InputElement->Flags.RightStick_270 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_270()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_270;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_270 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightStick_90() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_90) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_90() const
	{
		if (InputElement)
		{
			return GetStickButtonText("90", InputElement->Flags.RightStick_90 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_90()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_90;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_90 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightStick_225() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_225) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_225() const
	{
		if (InputElement)
		{
			return GetStickButtonText("225", InputElement->Flags.RightStick_225 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_225()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_225;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_225 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightStick_180() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_180) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_180() const
	{
		if (InputElement)
		{
			return GetStickButtonText("180", InputElement->Flags.RightStick_180 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_180()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_180;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_180 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightStick_135() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightStick_135) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightStick_135() const
	{
		if (InputElement)
		{
			return GetStickButtonText("135", InputElement->Flags.RightStick_135 && InputElement->Flags.RightStick_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightStick_135()
	{
		if (InputElement)
		{
			const bool prevState = InputElement->Flags.RightStick_135;
			if (prevState)
			{
				SwitchHold_RightStick();
			}
			else
			{
				InputElement->Flags.ResetRightStick();
				InputElement->Flags.RightStick_135 = true;
			}
		}

		return FReply::Handled();
	}

	FSlateColor ColorAndOpacity_RightUpperTrigger() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightUpperTrigger) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightUpperTrigger() const
	{
		if (InputElement)
		{
			return GetStickButtonText("LB", InputElement->Flags.RightUpperTrigger_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightUpperTrigger()
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightUpperTrigger_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.RightUpperTrigger_HOLD)
				{
					InputElement->Flags.RightUpperTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.RightUpperTrigger_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.RightUpperTrigger)
				{
					InputElement->Flags.RightUpperTrigger = true;
				}
				else if (!InputElement->Flags.RightUpperTrigger_HOLD)
				{
					InputElement->Flags.RightUpperTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.RightUpperTrigger = false;
					InputElement->Flags.RightUpperTrigger_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_RightUpperTrigger() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.RightUpperTrigger_HOLD_IN_PARENT;
		}

		return false;
	}

	FSlateColor ColorAndOpacity_RightTrigger() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightTrigger) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_RightTrigger() const
	{
		if (InputElement)
		{
			return GetStickButtonText("LT", InputElement->Flags.RightTrigger_HOLD);
		}

		return UndeterminedText;
	}

	FReply OnClicked_RightTrigger()
	{
		if (InputElement)
		{
			if (InputElement->Flags.RightTrigger_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.RightTrigger_HOLD)
				{
					InputElement->Flags.RightTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.RightTrigger_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.RightTrigger)
				{
					InputElement->Flags.RightTrigger = true;
				}
				else if (!InputElement->Flags.RightTrigger_HOLD)
				{
					InputElement->Flags.RightTrigger_HOLD = true;
				}
				else
				{
					InputElement->Flags.RightTrigger = false;
					InputElement->Flags.RightTrigger_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_RightTrigger() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.RightTrigger_HOLD_IN_PARENT;
		}

		return false;
	}

	FSlateColor ColorAndOpacity_X() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.X) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_X() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.X_HOLD) return HoldText;

			if (InputElement->Flags.X) return PressedText;

			return FText::FromString("X");
		}

		return UndeterminedText;
	}

	FReply OnClicked_X()
	{
		if (InputElement)
		{
			if (InputElement->Flags.X_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.X_HOLD)
				{
					InputElement->Flags.X_HOLD = true;
				}
				else
				{
					InputElement->Flags.X_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.X)
				{
					InputElement->Flags.X = true;
				}
				else if (!InputElement->Flags.X_HOLD)
				{
					InputElement->Flags.X_HOLD = true;
				}
				else
				{
					InputElement->Flags.X = false;
					InputElement->Flags.X_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_X() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.X_HOLD_IN_PARENT;
		}

		return false;
	}

	FSlateColor ColorAndOpacity_A() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.A) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_A() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.A_HOLD) return HoldText;

			if (InputElement->Flags.A) return PressedText;

			return FText::FromString("A");
		}

		return UndeterminedText;
	}

	FReply OnClicked_A()
	{
		if (InputElement)
		{
			if (InputElement->Flags.A_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.A_HOLD)
				{
					InputElement->Flags.A_HOLD = true;
				}
				else
				{
					InputElement->Flags.A_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.A)
				{
					InputElement->Flags.A = true;
				}
				else if (!InputElement->Flags.A_HOLD)
				{
					InputElement->Flags.A_HOLD = true;
				}
				else
				{
					InputElement->Flags.A = false;
					InputElement->Flags.A_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_A() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.A_HOLD_IN_PARENT;
		}

		return false;
	}

	FSlateColor ColorAndOpacity_Y() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.Y) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_Y() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.Y_HOLD) return HoldText;

			if (InputElement->Flags.Y) return PressedText;

			return FText::FromString("Y");
		}

		return UndeterminedText;
	}

	FReply OnClicked_Y()
	{
		if (InputElement)
		{
			if (InputElement->Flags.Y_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.Y_HOLD)
				{
					InputElement->Flags.Y_HOLD = true;
				}
				else
				{
					InputElement->Flags.Y_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.Y)
				{
					InputElement->Flags.Y = true;
				}
				else if (!InputElement->Flags.Y_HOLD)
				{
					InputElement->Flags.Y_HOLD = true;
				}
				else
				{
					InputElement->Flags.Y = false;
					InputElement->Flags.Y_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_Y() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.Y_HOLD_IN_PARENT;
		}

		return false;
	}

	FSlateColor ColorAndOpacity_B() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.B) return FSlateColor(ActiveColor);
		}

		return FSlateColor(InactiveColor);
	}

	FText Text_B() const
	{
		if (InputElement)
		{
			if (InputElement->Flags.B_HOLD) return HoldText;

			if (InputElement->Flags.B) return PressedText;

			return FText::FromString("B");
		}

		return UndeterminedText;
	}

	FReply OnClicked_B()
	{
		if (InputElement)
		{
			if (InputElement->Flags.B_HOLD_IN_PARENT)
			{
				if (!InputElement->Flags.B_HOLD)
				{
					InputElement->Flags.B_HOLD = true;
				}
				else
				{
					InputElement->Flags.B_HOLD = false;
				}
			}
			else
			{
				if (!InputElement->Flags.B)
				{
					InputElement->Flags.B = true;
				}
				else if (!InputElement->Flags.B_HOLD)
				{
					InputElement->Flags.B_HOLD = true;
				}
				else
				{
					InputElement->Flags.B = false;
					InputElement->Flags.B_HOLD = false;
				}
			}
		}

		return FReply::Handled();
	}

	bool IsEnabled_B() const
	{
		if (InputElement)
		{
			return !InputElement->Flags.B_HOLD_IN_PARENT;
		}

		return false;
	}
};

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

	ChildBuilder.AddCustomRow(LOCTEXT("SearchString", "Joystick Input"))[SNew(SGridPanel)];
}

#undef LOCTEXT_NAMESPACE