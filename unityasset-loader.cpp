#include "unityasset-loader.hpp"
#include "unity-helper.hpp"
#include <fstream>

namespace UnityAssetLoader {

    static bool boolTrue = true;
    static bool boolFalse = false;

    static IL2CPP_Helper* helper = nullptr;

    static UnityAssetLoader_OnLoadAssetBundleCompleteFunction* onLoadAssetBundleComplete = nullptr;
    static UnityAssetLoader_OnLoadAssetCompleteFunction* onLoadAssetComplete = nullptr;
    static Il2CppObject* assetBundleCreateRequest = nullptr;
    static Il2CppObject* assetBundleRequest = nullptr;
    
    void AssetBundleRequestComplete(){
        log(INFO, "UnityAssetLoader: AssetBundleRequestComplete Called!");
        Il2CppObject* customAsset;
        helper->RunMethod(&customAsset, assetBundleRequest, "get_asset");
        if(customAsset == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get Asset");
            return;
        }
        onLoadAssetComplete(customAsset);
        onLoadAssetComplete = nullptr;
        helper->RunMethod(assetBundleRequest, "Finalize");
        log(INFO, "UnityAssetLoader: AssetBundleRequestComplete Finished!");
    }

    void AssetBundleCreateRequestComplete(){
        log(INFO, "UnityAssetLoader: AssetBundleCreateRequestComplete Called!");
        Il2CppObject* customAssetBundle;
        helper->RunMethod(&customAssetBundle, assetBundleCreateRequest, "get_assetBundle");
        if(customAssetBundle == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get AssetBundle");
            onLoadAssetBundleComplete = nullptr;
            return;
        }
        onLoadAssetBundleComplete(customAssetBundle);
        onLoadAssetBundleComplete = nullptr;
        helper->RunMethod(assetBundleCreateRequest, "Finalize");
        log(INFO, "UnityAssetLoader: AssetBundleCreateRequestComplete Finished!");
    }

    bool LoadAssetFromAssetBundleAsync(Il2CppObject* assetBundle, UnityAssetLoader_OnLoadAssetCompleteFunction* onLoadAssetCompleteCall){
        if(helper == nullptr){
            helper = new IL2CPP_Helper();
            helper->Initialize();
        }
        if(onLoadAssetComplete != nullptr){
            log(ERROR, "UnityAssetLoader: Already Loading an Asset");
            return false;
        }
        helper->RunMethod(&assetBundleRequest, assetBundle, "LoadAssetAsync", helper->createcsstr("_customasset"), helper->type_get_object(helper->class_get_type(helper->GetClassFromName("UnityEngine", "GameObject"))));
        if(assetBundleRequest == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't run method LoadAssetAsync");
            return false;
        }
        helper->RunMethod(assetBundleRequest, "set_allowSceneActivation", &boolTrue);
        auto action = helper->MakeAction(nullptr, (void*)AssetBundleRequestComplete, helper->class_get_type(helper->GetClassFromName("System", "Action")));
        if(action == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't make AssetBundleComplete Action");
            return false;
        }
        if(!helper->SetFieldValue(assetBundleRequest, "m_completeCallback", action)){
            log(ERROR, "UnityAssetLoader: Couldn't set AssetBundleComplete Action");
            return false;
        }
        onLoadAssetComplete = onLoadAssetCompleteCall;
        return true;
    }
        
     bool LoadAssetBundleFromFileAsync(char* filePath, UnityAssetLoader_OnLoadAssetBundleCompleteFunction* onLoadAssetBundleCompleteCall){
        if(helper == nullptr){
            helper = new IL2CPP_Helper();
            helper->Initialize();
        }
        if(onLoadAssetBundleComplete != nullptr){
            log(ERROR, "UnityAssetLoader: Already Loading an AssetBundle");
            return false;
        }
        Il2CppClass* assetBundleClass = helper->GetClassFromName("UnityEngine", "AssetBundle");
        if(assetBundleClass == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get class AssetBundle");
            return false;
        }
        const MethodInfo* assetBundle_LoadFromFileAsyncMethod = helper->class_get_method_from_name(assetBundleClass, "LoadFromFileAsync", 1);
        if(assetBundle_LoadFromFileAsyncMethod == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get method LoadFromFileAsync");
            return false;
        }
        helper->RunMethod(&assetBundleCreateRequest, nullptr, assetBundle_LoadFromFileAsyncMethod, helper->createcsstr(filePath));
        if(assetBundleCreateRequest == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't run method LoadFromFileAsync");
            return false;
        }
        helper->RunMethod(assetBundleCreateRequest, "set_allowSceneActivation", &boolTrue);
        auto action = helper->MakeAction(nullptr, AssetBundleCreateRequestComplete, helper->class_get_type(helper->GetClassFromName("System", "Action")));
        if(action == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't make AssetBundleCreateRequestComplete Action");
            return false;
        }
        if(!helper->SetFieldValue(assetBundleCreateRequest, "m_completeCallback", action)){
            log(ERROR, "UnityAssetLoader: Couldn't set AssetBundleCreateRequestComplete Action");
            return false;
        }
        onLoadAssetBundleComplete = onLoadAssetBundleCompleteCall;
        log(INFO, "UnityAssetLoader: Loading asset %s", filePath);
        return true;
    }
}