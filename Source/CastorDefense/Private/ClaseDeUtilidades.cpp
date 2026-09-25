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

    // Obtenemos el Asset Registry
    IAssetRegistry& AssetRegistry = FAssetRegistryModule::GetRegistry();

    // Path de la clase padre
    FTopLevelAssetPath ParentClassPath = ParentUClass->GetClassPathName();

    TArray<FTopLevelAssetPath> ParentClasses;
    ParentClasses.Add(ParentClassPath);

    TSet<FTopLevelAssetPath> ExcludedClasses;
    TSet<FTopLevelAssetPath> DerivedClassPaths;

    // Obtiene TODOS los descendientes, incluidos nietos, bisnietos, etc.
    AssetRegistry.GetDerivedClassNames(
        ParentClasses,
        ExcludedClasses,
        DerivedClassPaths
    );

    for (const FTopLevelAssetPath& DerivedClassPath : DerivedClassPaths)
    {
        /*
         * DerivedClassPath para un Blueprint será algo parecido a:
         *
         * /Game/Enemies/BP_Enemigo.BP_Enemigo_C
         *
         * El Blueprint asset que queremos cargar es:
         *
         * /Game/Enemies/BP_Enemigo.BP_Enemigo
         */

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

        // Cargamos el Blueprint asset
        UObject* AssetObject = AssetData.GetAsset();

        UBlueprint* Blueprint = Cast<UBlueprint>(AssetObject);

        if (!Blueprint)
        {
            continue;
        }

        // Esta es la clase que realmente necesitamos para Spawn Actor From Class
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

    // Si no existen descendientes, devolvemos el propio padre
    if (Result.Num() == 0)
    {
        Result.Add(ParentClass);
    }

    return Result;
}