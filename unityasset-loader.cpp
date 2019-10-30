#include "unityasset-loader.hpp"

namespace UnityAssetLoader {

    static IL2CPP_Helper* helper = nullptr;

    static UnityAssetLoader_OnFinishFunction* onFinish = nullptr;
    static Il2CppObject* assetBundleCreateRequest = nullptr;
    static Il2CppObject* assetBundleRequest = nullptr;

    void AssetBundleComplete(){
        log(INFO, "UnityAssetLoader: AssetBundleComplete Called!");
        Il2CppObject* customAsset;
        helper->RunMethod(&customAsset, assetBundleRequest, "get_asset");
        if(customAsset == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get Asset");
            onFinish = nullptr;
            return;
        }
        onFinish(customAsset);
        onFinish = nullptr;
        log(INFO, "UnityAssetLoader: AssetBundleComplete Finished!");
    }

    void AssetBundleCreateRequestComplete(){
        log(INFO, "UnityAssetLoader: AssetBundleCreateRequestComplete Called!");
        Il2CppObject* customAssetBundle;
        helper->RunMethod(&customAssetBundle, assetBundleCreateRequest, "get_assetBundle");
        if(customAssetBundle == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't get AssetBundle");
            onFinish = nullptr;
            return;
        }
        helper->RunMethod(&assetBundleRequest, customAssetBundle, "LoadAssetAsync", helper->createcsstr("_customasset"), helper->type_get_object(helper->class_get_type(helper->GetClassFromName("UnityEngine", "GameObject"))));
        if(assetBundleRequest == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't run method LoadAssetAsync");
            onFinish = nullptr;
            return;
        }
        auto action = helper->MakeAction(nullptr, (void*)AssetBundleComplete, helper->class_get_type(helper->GetClassFromName("System", "Action")));
        if(action == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't make AssetBundleComplete Action");
            onFinish = nullptr;
            return;
        }
        if(!helper->SetFieldValue(assetBundleRequest, "m_completeCallback", action)){
            log(ERROR, "UnityAssetLoader: Couldn't set AssetBundleComplete Action");
            onFinish = nullptr;
            return;
        }
        log(INFO, "UnityAssetLoader: AssetBundleCreateRequestComplete Finished!");
    }

    bool LoadFromFileAsync(char* filePath, UnityAssetLoader_OnFinishFunction* onFinishCall){
        if(helper == nullptr){
            helper = new IL2CPP_Helper();
            helper->Initialize();
        }
        if(onFinish != nullptr){
            log(ERROR, "UnityAssetLoader: Already Loading an Asset");
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
        auto action = helper->MakeAction(nullptr, AssetBundleCreateRequestComplete, helper->class_get_type(helper->GetClassFromName("System", "Action")));
        if(action == nullptr){
            log(ERROR, "UnityAssetLoader: Couldn't make AssetBundleCreateRequestComplete Action");
            return false;
        }
        if(!helper->SetFieldValue(assetBundleCreateRequest, "m_completeCallback", action)){
            log(ERROR, "UnityAssetLoader: Couldn't set AssetBundleCreateRequestComplete Action");
            return false;
        }
        onFinish = onFinishCall;
        log(INFO, "UnityAssetLoader: Loading asset %s", filePath);
        return true;
    }
}