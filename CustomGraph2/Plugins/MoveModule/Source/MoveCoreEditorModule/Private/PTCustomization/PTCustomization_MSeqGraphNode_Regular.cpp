#include "PTCustomization_MSeqGraphNode_Regular.h"

TSharedRef<IPropertyTypeCustomization> FPTCustomization_MSeqGraphNode_Regular::MakeInstance()
{
	return MakeShareable(new FPTCustomization_MSeqGraphNode_Regular);
}

void FPTCustomization_MSeqGraphNode_Regular::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//FPTCustomization_Base::CustomizeHeader(PropertyHandle, HeaderRow, CustomizationUtils);
}

void FPTCustomization_MSeqGraphNode_Regular::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	InternalPropertyHandle = PropertyHandle;

	//ChildBuilder.AddCustomRow(LOCTEXT("SearchString", "Joystick Input"))[
	//	SNew(SVerticalBox)
	//		+ SVerticalBox::Slot().FillHeight(1)[SAssignNew(grid, SGridPanel).FillColumn(0, 1).FillColumn(1, 0)]
	//		+ SVerticalBox::Slot().AutoHeight().Padding(8)[SNew(SButton).OnClicked(this, &FPTCustomization_JoystickInput::Add).HAlign(HAlign_Center)[
	//			SNew(STextBlock).Text(LOCTEXT("Add", "ADD"))
	//		]]
	//];

	//if (FJoystickInput* joystickInput = GetPropertyAs<FJoystickInput>())
	//{
	//	for (int32 i = 0; i < joystickInput->InputElements.Num(); i++)
	//	{
	//		AddWidget(joystickInput, i);
	//	}
	//}
}
