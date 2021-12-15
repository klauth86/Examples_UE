// Fill out your copyright notice in the Description page of Project Settings.

#include "PTCustomization_JoystickInput.h"

#define LOCTEXT_NAMESPACE "PropertyTypeCustomization_JoystickInput.h"

TSharedRef<IPropertyTypeCustomization> FPTCustomization_JoystickInput::MakeInstance()
{
	return MakeShareable(new FPTCustomization_JoystickInput);
}

void FPTCustomization_JoystickInput::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructPropertyHandle = PropertyHandle;
	TSharedPtr<IPropertyUtilities> PropertyUtils = CustomizationUtils.GetPropertyUtilities();

	////// Get list of enum indexes
	////TArray< TSharedPtr<int32> > ComboItems;
	////GenerateComboBoxIndexes(ComboItems);

	////ComboBox = SNew(SPinComboBox)
	////	.ComboItemList(ComboItems)
	////	.VisibleText(this, &FPropertyTypeCustomization_Replica::OnGetText)
	////	.OnSelectionChanged(this, &FPropertyTypeCustomization_Replica::ComboBoxSelectionChanged)
	////	.OnGetDisplayName(this, &FPropertyTypeCustomization_Replica::OnGetFriendlyName)
	////	.OnGetTooltip(this, &FPropertyTypeCustomization_Replica::OnGetTooltip);

	////HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
	////	.ValueContent()[ComboBox.ToSharedRef()].IsEnabled(MakeAttributeLambda([=] { return !PropertyHandle->IsEditConst() && PropertyUtils->IsPropertyEditingEnabled(); }));
}

#undef LOCTEXT_NAMESPACE