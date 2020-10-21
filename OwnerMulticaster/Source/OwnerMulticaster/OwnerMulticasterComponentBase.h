// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utils/LogManager.h"
#include <typeinfo>
#include "OwnerMulticasterComponentBase.generated.h"

class UActorComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOwnerMulticasterComponentBase : public UInterface {
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE(FOnComponentEndPlayEvent);

class OWNERMULTICASTER_API IOwnerMulticasterComponentBase {

	GENERATED_BODY()

protected:
			
	FCriticalSection Locker;
		
	template<typename T>
	TMap<IOwnerMulticasterComponentBase*, T*>& GetMapping_TOwner() {
		static TMap<IOwnerMulticasterComponentBase*, T*> ownerMapping;
		return ownerMapping;
	}
		
	template<typename T>
	TMap<IOwnerMulticasterComponentBase*, FDelegateHandle>& GetMapping_Callback() {
		static TMap<IOwnerMulticasterComponentBase*, FDelegateHandle> callbackMapping;
		return callbackMapping;
	}
		
	template<typename T>
	void OnComponentEndPlayCallback() {
		
		FScopeLock Lock(&Locker);
		
		auto& ownerMapping = GetMapping_TOwner<T>();
		ownerMapping.Remove(this);
		LogManager::LogWarning(TEXT("OWNER[%d]: [%d] remove %s"), &ownerMapping, this, *FString(typeid(T).name()));

		auto& callbackMapping = GetMapping_Callback<T>();
		if (callbackMapping.Contains(this)) {
			OnComponentEndPlay.Remove(callbackMapping[this]);
			callbackMapping.Remove(this);
			LogManager::LogWarning(TEXT("CALLBACK[%d]: [%d] remove %s"), &callbackMapping, this, *FString(typeid(T).name()));
		}
	}

public:

	FOnComponentEndPlayEvent OnComponentEndPlay;

	virtual UActorComponent* GetSelfAsActorComponent() { return nullptr; };

	template<typename T>
	T* GetOwnerAs() {
		
		FScopeLock Lock(&Locker);

		auto& ownerMapping = GetMapping_TOwner<T>();
		if (!ownerMapping.Contains(this)) {
			
			T* castedOwner = nullptr;
			if (auto component = GetSelfAsActorComponent()) {
				auto owner = component->GetOwner();
				castedOwner = Cast<T>(owner);
			}
			ownerMapping.Add(this, castedOwner);
			LogManager::LogWarning(TEXT("OWNER[%d]: [%d] add %s"), &ownerMapping, this, *FString(typeid(T).name()));

			auto& callbackMapping = GetMapping_Callback<T>();
			callbackMapping.Add(this, OnComponentEndPlay.AddRaw(this, &IOwnerMulticasterComponentBase::OnComponentEndPlayCallback<T>));
			LogManager::LogWarning(TEXT("CALLBACK[%d]: [%d] add %s"), &callbackMapping, this, *FString(typeid(T).name()));
		}

		return ownerMapping[this];
	}

	template<typename T>
	void Log() {
		auto ownerMapping = GetMapping_TOwner<T>();

		FString result = "[";

		for (TMap<IOwnerMulticasterComponentBase*, T*>::TIterator It(ownerMapping); It; ++It) {
			auto key = It->Key;
			auto value = It->Value;
			auto owner = key->GetSelfAsActorComponent()->GetOwner();
			auto component = GetSelfAsActorComponent();

			//result += FString::Printf(TEXT("Owner %s(%d) class %s(%d) diff (%d)"), *owner->GetName(), owner, *T::StaticClass()->GetName(), value, (uint64)value - (uint64)owner);
			result += FString::Printf(TEXT("Comp(%d) interface(%d) diff(%d)"), component, this, (uint64)component - (uint64)this);
			result += " , ";
		}

		result += "]";
		LogManager::LogWarning(TEXT("%s"), *result);
	}
};
