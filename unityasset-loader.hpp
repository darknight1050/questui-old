#include "../beatsaber-hook/shared/utils/utils.h"

typedef void UnityAssetLoader_OnLoadAssetBundleCompleteFunction(Il2CppObject* assetBundle);
typedef void UnityAssetLoader_OnLoadAssetCompleteFunction(Il2CppObject* asset);

namespace UnityAssetLoader {

    bool LoadAssetBundleFromFileAsync(char* filePath, UnityAssetLoader_OnLoadAssetBundleCompleteFunction* onLoadAssetBundleCompleteCall);
    bool LoadAssetFromAssetBundleAsync(Il2CppObject* assetBundle, UnityAssetLoader_OnLoadAssetCompleteFunction* onLoadAssetCompleteCall);
    
};