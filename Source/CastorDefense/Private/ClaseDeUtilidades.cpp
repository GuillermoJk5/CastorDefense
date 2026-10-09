#include "ClaseDeUtilidades.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Blueprint.h"
#include "Misc/PackageName.h"

TArray<TSubclassOf<AActor>> UClaseDeUtilidades::ObtenerTodasLasClasesHijas(
    TSubclassOf<AActor> ParentClass)
{
    TArray<TSubclassOf<AActor>> Result;

    if (!ParentClass)
    {
        return Result;
    }

    UClass* ParentUClass = ParentClass.Get();

    IAssetRegistry& AssetRegistry = FAssetRegistryModule::GetRegistry();

    FTopLevelAssetPath ParentClassPath = ParentUClass->GetClassPathName();

    TArray<FTopLevelAssetPath> ParentClasses;
    ParentClasses.Add(ParentClassPath);

    TSet<FTopLevelAssetPath> ExcludedClasses;
    TSet<FTopLevelAssetPath> DerivedClassPaths;

    AssetRegistry.GetDerivedClassNames(
        ParentClasses,
        ExcludedClasses,
        DerivedClassPaths
    );

    for (const FTopLevelAssetPath& DerivedClassPath : DerivedClassPaths)
    {
        const FName PackageName = DerivedClassPath.GetPackageName();
        const FString AssetName = FPackageName::GetShortName(PackageName.ToString());

        const FString BlueprintObjectPath =
            PackageName.ToString() + TEXT(".") + AssetName;

        FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(
            FName(*BlueprintObjectPath)
        );

        if (!AssetData.IsValid())
        {
            continue;
        }

        UObject* AssetObject = AssetData.GetAsset();

        UBlueprint* Blueprint = Cast<UBlueprint>(AssetObject);

        if (!Blueprint)
        {
            continue;
        }

        UClass* GeneratedClass = Blueprint->GeneratedClass;

        if (!GeneratedClass)
        {
            continue;
        }

        if (!GeneratedClass->IsChildOf(ParentUClass))
        {
            continue;
        }

        if (!GeneratedClass->IsChildOf(AActor::StaticClass()))
        {
            continue;
        }

        Result.Add(GeneratedClass);
    }

    if (Result.Num() == 0)
    {
        Result.Add(ParentClass);
    }

    return Result;
}

void UClaseDeUtilidades::ActivarDesactivarActor(AActor* Actor,bool Activar)
{
    if (Activar) {

        Actor->SetActorHiddenInGame(false);
        Actor->SetActorEnableCollision(true);
        Actor->SetActorTickEnabled(true);

    }
    else {

        Actor->SetActorTickEnabled(false);
        Actor->SetActorEnableCollision(false);
        Actor->SetActorHiddenInGame(true);
    }


}
