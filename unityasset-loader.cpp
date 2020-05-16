#include "unityasset-loader.hpp"
#include "unity-helper.hpp"
#include <fstream>

namespace UnityAssetLoader {

    static bool boolTrue = true;
    static bool boolFalse = false;

    static UnityAssetLoader_OnLoadAssetBundleCompleteFunction* onLoadAssetBundleComplete = nullptr;
    static UnityAssetLoader_OnLoadAssetCompleteFunction* onLoadAssetComplete = nullptr;
    static Il2CppObject* assetBundleCreateRequest = nullptr;
    static Il2CppObject* assetBundleRequest = nullptr;
    
    void AssetBundleRequestComplete(){
        log(INFO, "UnityAssetLoader: AssetBundleRequestComplete Called!");
        Il2CppObject* customAsset;
        customAsset = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(assetBundleRequest, "get_asset"));
        if(customAsset == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get Asset");
            onLoadAssetComplete = nullptr;
            return;
        }
        onLoadAssetComplete(customAsset);
        onLoadAssetComplete = nullptr;
        il2cpp_utils::RunMethod(assetBundleRequest, "Finalize");
        log(INFO, "UnityAssetLoader: AssetBundleRequestComplete Finished!");
    }

    void AssetBundleCreateRequestComplete(){
        log(INFO, "UnityAssetLoader: AssetBundleCreateRequestComplete Called!");
        Il2CppObject* customAssetBundle;
        customAssetBundle = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(assetBundleCreateRequest, "get_assetBundle"));
        if(customAssetBundle == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get AssetBundle");
            onLoadAssetBundleComplete = nullptr;
            return;
        }
        onLoadAssetBundleComplete(customAssetBundle);
        onLoadAssetBundleComplete = nullptr;
        il2cpp_utils::RunMethod(assetBundleCreateRequest, "Finalize");
        log(INFO, "UnityAssetLoader: AssetBundleCreateRequestComplete Finished!");
    }

    bool LoadAssetFromAssetBundleAsync(Il2CppObject* assetBundle, UnityAssetLoader_OnLoadAssetCompleteFunction* onLoadAssetCompleteCall){
        if(onLoadAssetComplete != nullptr){
            log(ERROR, "UnityAssetLoader: Already Loading an Asset");
            return false;
        }
        assetBundleRequest = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(assetBundle, "LoadAssetAsync", il2cpp_utils::createcsstr("_customasset"), il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("UnityEngine", "GameObject")))));
        if(assetBundleRequest == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't run method LoadAssetAsync");
            return false;
        }
        il2cpp_utils::RunMethod(assetBundleRequest, "set_allowSceneActivation", boolTrue);
        auto action = il2cpp_utils::MakeAction(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("System", "Action")),nullptr,AssetBundleRequestComplete);
        if(action == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't make AssetBundleComplete Action");
            return false;
        }
        if(!il2cpp_utils::SetFieldValue(assetBundleRequest, "m_completeCallback", action)){
            log(ERROR, "UnityAssetLoader: Couldn't set AssetBundleComplete Action");
            return false;
        }
        onLoadAssetComplete = onLoadAssetCompleteCall;
        return true;
    }
        
     bool LoadAssetBundleFromFileAsync(char* filePath, UnityAssetLoader_OnLoadAssetBundleCompleteFunction* onLoadAssetBundleCompleteCall){
        if(onLoadAssetBundleComplete != nullptr){
            log(ERROR, "UnityAssetLoader: Already Loading an AssetBundle");
            return false;
        }
        Il2CppClass* assetBundleClass = il2cpp_utils::GetClassFromName("UnityEngine", "AssetBundle");
        if(assetBundleClass == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get class AssetBundle");
            return false;
        }
        const MethodInfo* assetBundle_LoadFromFileAsyncMethod = il2cpp_functions::class_get_method_from_name(assetBundleClass, "LoadFromFileAsync", 1);
        if(assetBundle_LoadFromFileAsyncMethod == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get method LoadFromFileAsync");
            return false;
        }
        il2cpp_utils::RunMethod(&assetBundleCreateRequest, assetBundle_LoadFromFileAsyncMethod, il2cpp_utils::createcsstr(filePath));
        if(assetBundleCreateRequest == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't run method LoadFromFileAsync");
            return false;
        }
        il2cpp_utils::RunMethod(assetBundleCreateRequest, "set_allowSceneActivation", boolTrue);
        auto action = il2cpp_utils::MakeAction(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("System", "Action")),nullptr,AssetBundleCreateRequestComplete);
        if(action == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't make AssetBundleCreateRequestComplete Action");
            return false;
        }
        if(!il2cpp_utils::SetFieldValue(assetBundleCreateRequest, "m_completeCallback", action)){
            log(ERROR, "UnityAssetLoader: Couldn't set AssetBundleCreateRequestComplete Action");
            return false;
        }
        onLoadAssetBundleComplete = onLoadAssetBundleCompleteCall;
        log(INFO, "UnityAssetLoader: Loading asset %s", filePath);
        return true;
    }
}