// Fill out your copyright notice in the Description page of Project Settings.

#include "PTCustomization_JoystickInput.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "PropertyTypeCustomization_JoystickInput.h"

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
	TSharedPtr<IPropertyUtilities> PropertyUtils = CustomizationUtils.GetPropertyUtilities();

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
			.FillRow(0, 2)
			.FillRow(0, 3)

			// LEFT TRIGGERS

			+SGridPanel::Slot(0, 0)[SNew(STextBlock).Text(FText::FromString("LB"))]
			+ SGridPanel::Slot(0, 1)[SNew(STextBlock).Text(FText::FromString("LT"))]

			// LEFT STICK

			+SGridPanel::Slot(1, 0)[SNew(STextBlock).Text(FText::FromString("315"))]
			+ SGridPanel::Slot(2, 0)[SNew(STextBlock).Text(FText::FromString("0"))]
			+ SGridPanel::Slot(3, 0)[SNew(STextBlock).Text(FText::FromString("45"))]

			+ SGridPanel::Slot(1, 1)[SNew(STextBlock).Text(FText::FromString("270"))]

			+ SGridPanel::Slot(3, 1)[SNew(STextBlock).Text(FText::FromString("90"))]

			+ SGridPanel::Slot(1, 2)[SNew(STextBlock).Text(FText::FromString("225"))]
			+ SGridPanel::Slot(2, 2)[SNew(STextBlock).Text(FText::FromString("180"))]
			+ SGridPanel::Slot(3, 2)[SNew(STextBlock).Text(FText::FromString("135"))]

			// RIGHT STICK

			+SGridPanel::Slot(4, 0)[SNew(STextBlock).Text(FText::FromString("315"))]
			+ SGridPanel::Slot(5, 0)[SNew(STextBlock).Text(FText::FromString("0"))]
			+ SGridPanel::Slot(6, 0)[SNew(STextBlock).Text(FText::FromString("45"))]

			+ SGridPanel::Slot(4, 1)[SNew(STextBlock).Text(FText::FromString("270"))]

			+ SGridPanel::Slot(6, 1)[SNew(STextBlock).Text(FText::FromString("90"))]

			+ SGridPanel::Slot(4, 2)[SNew(STextBlock).Text(FText::FromString("225"))]
			+ SGridPanel::Slot(5, 2)[SNew(STextBlock).Text(FText::FromString("180"))]
			+ SGridPanel::Slot(6, 2)[SNew(STextBlock).Text(FText::FromString("135"))]

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

#undef LOCTEXT_NAMESPACE