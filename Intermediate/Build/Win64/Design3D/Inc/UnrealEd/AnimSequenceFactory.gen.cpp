// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UnrealEd/Classes/Factories/AnimSequenceFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAnimSequenceFactory() {}
// Cross Module References
	UNREALED_API UClass* Z_Construct_UClass_UAnimSequenceFactory_NoRegister();
	UNREALED_API UClass* Z_Construct_UClass_UAnimSequenceFactory();
	UNREALED_API UClass* Z_Construct_UClass_UFactory();
	UPackage* Z_Construct_UPackage__Script_UnrealEd();
	ENGINE_API UClass* Z_Construct_UClass_USkeletalMesh_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USkeleton_NoRegister();
// End Cross Module References
	void UAnimSequenceFactory::StaticRegisterNativesUAnimSequenceFactory()
	{
	}
	UClass* Z_Construct_UClass_UAnimSequenceFactory_NoRegister()
	{
		return UAnimSequenceFactory::StaticClass();
	}
	struct Z_Construct_UClass_UAnimSequenceFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PreviewSkeletalMesh_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_PreviewSkeletalMesh;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TargetSkeleton_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TargetSkeleton;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAnimSequenceFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_UnrealEd,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAnimSequenceFactory_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Object" },
		{ "IncludePath", "Factories/AnimSequenceFactory.h" },
		{ "ModuleRelativePath", "Classes/Factories/AnimSequenceFactory.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_PreviewSkeletalMesh_MetaData[] = {
		{ "ModuleRelativePath", "Classes/Factories/AnimSequenceFactory.h" },
		{ "ToolTip", "The preview mesh to use with this animation" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_PreviewSkeletalMesh = { "PreviewSkeletalMesh", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UAnimSequenceFactory, PreviewSkeletalMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_PreviewSkeletalMesh_MetaData, ARRAY_COUNT(Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_PreviewSkeletalMesh_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_TargetSkeleton_MetaData[] = {
		{ "ModuleRelativePath", "Classes/Factories/AnimSequenceFactory.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_TargetSkeleton = { "TargetSkeleton", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UAnimSequenceFactory, TargetSkeleton), Z_Construct_UClass_USkeleton_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_TargetSkeleton_MetaData, ARRAY_COUNT(Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_TargetSkeleton_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UAnimSequenceFactory_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_PreviewSkeletalMesh,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UAnimSequenceFactory_Statics::NewProp_TargetSkeleton,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAnimSequenceFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAnimSequenceFactory>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UAnimSequenceFactory_Statics::ClassParams = {
		&UAnimSequenceFactory::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UAnimSequenceFactory_Statics::PropPointers,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		0,
		ARRAY_COUNT(Z_Construct_UClass_UAnimSequenceFactory_Statics::PropPointers),
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UAnimSequenceFactory_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UAnimSequenceFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UAnimSequenceFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UAnimSequenceFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UAnimSequenceFactory, 1610747653);
	template<> UNREALED_API UClass* StaticClass<UAnimSequenceFactory>()
	{
		return UAnimSequenceFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UAnimSequenceFactory(Z_Construct_UClass_UAnimSequenceFactory, &UAnimSequenceFactory::StaticClass, TEXT("/Script/UnrealEd"), TEXT("UAnimSequenceFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAnimSequenceFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
