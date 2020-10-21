// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject_A.h"

FArchive& operator<<(FArchive& Ar, UMyObject_A& context) {

	Ar << context.StringProp;
	return Ar;

}