// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UnrealEd/Classes/ActorFactories/ActorFactoryTriggerSphere.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeActorFactoryTriggerSphere() {}
// Cross Module References
	UNREALED_API UClass* Z_Construct_UClass_UActorFactoryTriggerSphere_NoRegister();
	UNREALED_API UClass* Z_Construct_UClass_UActorFactoryTriggerSphere();
	UNREALED_API UClass* Z_Construct_UClass_UActorFactory();
	UPackage* Z_Construct_UPackage__Script_UnrealEd();
// End Cross Module References
	void UActorFactoryTriggerSphere::StaticRegisterNativesUActorFactoryTriggerSphere()
	{
	}
	UClass* Z_Construct_UClass_UActorFactoryTriggerSphere_NoRegister()
	{
		return UActorFactoryTriggerSphere::StaticClass();
	}
	struct Z_Construct_UClass_UActorFactoryTriggerSphere_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UActorFactoryTriggerSphere_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_UnrealEd,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UActorFactoryTriggerSphere_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Object Object" },
		{ "IncludePath", "ActorFactories/ActorFactoryTriggerSphere.h" },
		{ "ModuleRelativePath", "Classes/ActorFactories/ActorFactoryTriggerSphere.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UActorFactoryTriggerSphere_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UActorFactoryTriggerSphere>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UActorFactoryTriggerSphere_Statics::ClassParams = {
		&UActorFactoryTriggerSphere::StaticClass,
		"Editor",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000830ACu,
		METADATA_PARAMS(Z_Construct_UClass_UActorFactoryTriggerSphere_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UActorFactoryTriggerSphere_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UActorFactoryTriggerSphere()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UActorFactoryTriggerSphere_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UActorFactoryTriggerSphere, 2818110957);
	template<> UNREALED_API UClass* StaticClass<UActorFactoryTriggerSphere>()
	{
		return UActorFactoryTriggerSphere::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UActorFactoryTriggerSphere(Z_Construct_UClass_UActorFactoryTriggerSphere, &UActorFactoryTriggerSphere::StaticClass, TEXT("/Script/UnrealEd"), TEXT("UActorFactoryTriggerSphere"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UActorFactoryTriggerSphere);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
