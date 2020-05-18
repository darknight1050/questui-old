#pragma once
#include "../beatsaber-hook/shared/utils/typedefs.h"
#include "../beatsaber-hook/shared/utils/utils.h"
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include <string_view>

// class Asset : public Il2CppObject {
//     Asset() = delete;
// };
typedef Il2CppObject Asset;

class AssetBundle : public Il2CppObject {
    typedef function_ptr_t<void, Asset*> AssetCallback;
    typedef function_ptr_t<void, AssetBundle*> AssetBundleCallback;

  public:
    AssetBundle() = delete;
    static bool LoadFromFileAsync(std::string_view filePath, AssetBundleCallback callback);
    static AssetBundle* LoadFromFile(std::string_view filePath);

    // Loads the asset with the given name and type from this bundle, then calls the given callback.
    // If not supplied, assetType will default to UnityEngine.GameObject.
    bool LoadAssetAsync(std::string_view assetName, AssetCallback callback, Il2CppReflectionType* assetType = nullptr);
    // Loads the asset with the given name and type from this bundle, blocking until finished.
    // If not supplied, assetType will default to UnityEngine.GameObject.
    Asset* LoadAsset(std::string_view assetName, Il2CppReflectionType* assetType = nullptr);
  private:
    static void AssetBundleComplete(AssetBundleCallback callback, Il2CppObject* assetBundleCreateRequest);
    static void AssetComplete(AssetCallback callback, Il2CppObject* assetBundleRequest);
};
DEFINE_IL2CPP_ARG_TYPE(AssetBundle*, "UnityEngine", "AssetBundle");
