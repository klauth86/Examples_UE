// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject_B.h"

FArchive& operator<<(FArchive& Ar, UMyObject_B& context) {

	Ar << context.StringProp;
	Ar << context.FloatProp;
	return Ar;

}