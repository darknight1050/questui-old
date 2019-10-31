#include "questui.hpp"
#include <vector>
#include <thread>

namespace QuestUI {

    static bool boolTrue = true;
    static bool boolFalse = false;

    static IL2CPP_Helper* helper = nullptr;
    static bool loaderInstance = false;
   
    static QuestUIInfo* questUIInfo;
    static QuestUIModInfo questUIModInfo;

    static bool loadUI = false;

    static Il2CppObject* menuTransformParent = nullptr;
    static Il2CppObject* buttonBinder = nullptr;
    static Array<Il2CppObject*>* menuButtons = nullptr;
    
    static Il2CppObject* modsButton = nullptr;
    static Il2CppObject* assetLoaderFinishedButton = nullptr;
    static Il2CppObject* customUIObject = nullptr;
    static Il2CppObject* assetBundle = nullptr;

    static Il2CppObject* buttonModsListUp = nullptr;
    static Il2CppObject* buttonModsListDown = nullptr;
   
    static int listIndex = 0;

    void SetUIActive(bool active){
        if(customUIObject != nullptr){
            helper->RunMethod(customUIObject, "SetActive", &active);
            for(int i = 0;i<menuButtons->Length();i++){
                Il2CppString* nameObject;
                helper->RunMethod(&nameObject, menuButtons->values[i], "get_name");
                if(strcmp(to_utf8(csstrtostr(nameObject)).c_str(), "BeatmapEditorButton") != 0)
                    UnityHelper::SetGameObjectActive(helper, menuButtons->values[i], !active);
            }
        }
        if(!active && modsButton != nullptr){
            UnityHelper::SetButtonText(helper, modsButton, "Mods");
        }
    }

    void RefreshList(){
        float buttonMod_TemplateTransformHeight = 27.5f;
        float ModsListObjectTransformHeight = 220.0f;
        UnityHelper::SetButtonEnabled(helper, buttonModsListUp, listIndex > 0);
        UnityHelper::SetButtonEnabled(helper, buttonModsListDown, questUIInfo->Mods->size()-ModsListObjectTransformHeight/buttonMod_TemplateTransformHeight-listIndex > 0);
            
        Il2CppObject* buttonMod_Template = UnityHelper::GetComponentInChildren(helper, customUIObject, helper->GetClassFromName("UnityEngine.UI", "Button"), "ButtonMod_Template");
        Il2CppObject* buttonMod_TemplateTransform;
        helper->RunMethod(&buttonMod_TemplateTransform, buttonMod_Template, "get_transform");
        Vector3 buttonMod_TemplateTransformPosition;
        helper->RunMethod(&buttonMod_TemplateTransformPosition, buttonMod_TemplateTransform, "get_localPosition");
        Vector3 buttonPos = buttonMod_TemplateTransformPosition;
        for(int i = listIndex;i<questUIInfo->Mods->size();i++){
            QuestUIModInfo* modInfo = (*questUIInfo->Mods)[i];
            Il2CppObject* modInfoButtonTransform;
            UnityHelper::SetGameObjectActive(helper, modInfo->Button, true);
            helper->RunMethod(&modInfoButtonTransform, modInfo->Button, "get_transform");
            helper->RunMethod(modInfoButtonTransform, "set_localPosition", &buttonPos);
            buttonPos.y -= buttonMod_TemplateTransformHeight;
        }
    }


    #pragma region ButtonHandlers
    void ButtonModsOnClick(Il2CppObject* button){
        log(INFO, "QuestUI: ButtonModsOnClick!");
        SetUIActive(true);
    }
 
    void ButtonModsListUpOnClick(Il2CppObject* button){
        listIndex--;
        RefreshList();
        log(INFO, "QuestUI: ButtonModsListUpOnClick!");
    }

    void ButtonModsListDownOnClick(Il2CppObject* button){
        listIndex++;
        RefreshList();
        log(INFO, "QuestUI: ButtonModsListDownOnClick!");
    }

    void ButtonBackOnClick(Il2CppObject* button){
        log(INFO, "QuestUI: ButtonBackOnClick!");
        SetUIActive(false);
    }

    void ButtonModOnClick(Il2CppObject* button){
        log(INFO, "QuestUI: ButtonModOnClick!");
        for(QuestUIModInfo* modInfo : *questUIInfo->Mods){
            UnityHelper::SetGameObjectActive(helper, modInfo->Panel, modInfo->Button == button);
        }
    }
    #pragma endregion
    
    void AssetLoaderFinishedButtonOnClick(){
        if(customUIObject == nullptr){
            Il2CppObject* assetLoaderFinishedButtonTextObject;
            helper->RunMethod(&assetLoaderFinishedButtonTextObject, assetLoaderFinishedButton, "GetComponentInChildren", helper->type_get_object(helper->class_get_type(helper->GetClassFromName("TMPro", "TextMeshProUGUI"))), &boolTrue);
            Il2CppString* nameObject;
            helper->RunMethod(&nameObject, assetLoaderFinishedButtonTextObject, "get_text");
            sscanf(to_utf8(csstrtostr(nameObject)).c_str(), "%llx", &questUIInfo);
            customUIObject = questUIInfo->CustomUIObject;
        }
        questUIInfo->Mods->push_back(&questUIModInfo);
        log(INFO, "QuestUI: Added mod %s to QuestUIInfo: %p", questUIModInfo.Name, questUIInfo);
    }

    void OnInitializedThread(QuestUIModInfo* modInfo){
        sleep(1);
        ((void (*)(void)) modInfo->OnInitialized)();
    }

    void OnLoadAssetComplete(Il2CppObject* assetArg){
        log(INFO, "QuestUI: OnLoadAssetComplete Called!");
        helper->RunMethod(&customUIObject, nullptr, helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine", "Object"), "Instantiate", 1), assetArg);
        helper->RunMethod(customUIObject, "set_name", helper->createcsstr("CustomUIObject"));
        helper->RunMethod(customUIObject, "SetActive", &boolFalse);

        Il2CppObject* customUITransform;
        helper->RunMethod(&customUITransform, customUIObject, "get_transform");
        helper->RunMethod(customUITransform, "SetParent", menuTransformParent, &boolFalse);

        buttonModsListUp = UnityHelper::GetComponentInChildren(helper, customUIObject, helper->GetClassFromName("UnityEngine.UI", "Button"), "ButtonModsListUp");
        buttonModsListDown = UnityHelper::GetComponentInChildren(helper, customUIObject, helper->GetClassFromName("UnityEngine.UI", "Button"), "ButtonModsListDown");
       
        UnityHelper::AddButtonOnClick(helper, buttonBinder, customUIObject, "ButtonBack", (UnityHelper::ButtonOnClickFunction*)ButtonBackOnClick);
        UnityHelper::AddButtonOnClick(helper, buttonBinder, buttonModsListUp, (UnityHelper::ButtonOnClickFunction*)ButtonModsListUpOnClick);
        UnityHelper::AddButtonOnClick(helper, buttonBinder, buttonModsListDown, (UnityHelper::ButtonOnClickFunction*)ButtonModsListDownOnClick);

        if(questUIInfo != nullptr){
            delete questUIInfo->Mods;
            delete questUIInfo;
        }
        questUIInfo = new QuestUIInfo();
        questUIInfo->CustomUIObject = customUIObject;
        questUIInfo->ButtonBinder = buttonBinder;
        questUIInfo->Mods = new std::vector<QuestUIModInfo*>();

        char buffer[9];
        sprintf(buffer, "%llx", questUIInfo);
        UnityHelper::SetButtonText(helper, assetLoaderFinishedButton, buffer);
        Il2CppObject* onClick;
        helper->RunMethod(&onClick, assetLoaderFinishedButton, "get_onClick");
        helper->RunMethod(onClick, "Invoke");

        log(INFO, "QuestUI: %lu Mods Loaded!", questUIInfo->Mods->size());

        Il2CppObject* buttonMod_Template = UnityHelper::GetComponentInChildren(helper, customUIObject, helper->GetClassFromName("UnityEngine.UI", "Button"), "ButtonMod_Template");
        UnityHelper::SetGameObjectActive(helper, buttonMod_Template, false);
        Il2CppObject* panelMod_Template = UnityHelper::GetComponentInChildren(helper, customUIObject, helper->GetClassFromName("UnityEngine", "Object"), "PanelMod_Template");
        UnityHelper::SetGameObjectActive(helper, panelMod_Template, false);

        for(QuestUIModInfo* modInfo : *questUIInfo->Mods){
            helper->RunMethod(&modInfo->Button, nullptr, helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine", "Object"), "Instantiate", 1), buttonMod_Template);
            helper->RunMethod(modInfo->Button, "set_name", helper->createcsstr("ButtonMod"));
            UnityHelper::SetSameParent(helper, modInfo->Button, buttonMod_Template);

            UnityHelper::AddButtonOnClick(helper, buttonBinder, modInfo->Button, (UnityHelper::ButtonOnClickFunction*)ButtonModOnClick);

            helper->RunMethod(&modInfo->Panel, nullptr, helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine", "Object"), "Instantiate", 1), panelMod_Template);
            helper->RunMethod(modInfo->Panel, "set_name", helper->createcsstr("PanelMod"));
            UnityHelper::SetSameParent(helper, modInfo->Panel, panelMod_Template);

            UnityHelper::SetButtonText(helper, modInfo->Button, modInfo->Name);
            
            std::thread onInitializedThread(OnInitializedThread, modInfo);
            onInitializedThread.detach();
        }
        if(questUIInfo->Mods->size() > 0){
           UnityHelper::SetGameObjectActive(helper, (*questUIInfo->Mods)[0]->Panel, true);
        }
        RefreshList();
        log(INFO, "QuestUI: OnLoadAssetComplete Finished!");
    }

    void OnLoadAssetBundleComplete(Il2CppObject* assetBundleArg){
        assetBundle = assetBundleArg;
        UnityAssetLoader::LoadAssetFromAssetBundleAsync(assetBundle, (UnityAssetLoader_OnLoadAssetBundleCompleteFunction*)OnLoadAssetComplete);
    }

    MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, int scene)
    {
        Il2CppString* nameObject;
        helper->RunStaticMethod(&nameObject, helper->GetClassFromName("UnityEngine.SceneManagement", "Scene"), "GetNameInternal", &scene);
        auto name = to_utf8(csstrtostr(nameObject)).c_str();
        if(strcmp(name, "MenuCore") == 0){
            loadUI = true;
        }
        return SceneManager_SetActiveScene(scene);
    }

    MAKE_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, void, Il2CppObject* self, bool firstActivation, int type){
        if(loadUI){
            loadUI = false;
            customUIObject = nullptr;
            Il2CppObject* settingsButton = helper->GetFieldObjectValue(self, "_settingsButton");
            Il2CppObject* settingsButtonTransform;
            helper->RunMethod(&settingsButtonTransform, settingsButton, "get_transform");
            Il2CppObject* settingsButtonTransformParent;
            helper->RunMethod(&settingsButtonTransformParent, settingsButtonTransform, "GetParent");
            helper->RunMethod(&buttonBinder, self, "get__buttonBinder");
            helper->RunMethod(&menuTransformParent, settingsButtonTransformParent, "GetParent");
            helper->RunMethod(&menuTransformParent, menuTransformParent, "GetParent");
            helper->RunMethod(&menuTransformParent, menuTransformParent, "GetParent");
           
            modsButton = UnityHelper::GetComponentInChildren(helper, settingsButtonTransformParent, helper->GetClassFromName("UnityEngine.UI", "Button"), "MainMenuModsButton");
            assetLoaderFinishedButton = UnityHelper::GetComponentInChildren(helper, settingsButtonTransformParent, helper->GetClassFromName("UnityEngine.UI", "Button"), "AssetLoaderFinishedButton");
            bool assetLoaderFinishedButtonExist = assetLoaderFinishedButton != nullptr;
            if(modsButton == nullptr){
                loaderInstance = true;
                helper->RunMethod(&modsButton, nullptr, helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine", "Object"), "Instantiate", 1), settingsButton);
                helper->RunMethod(modsButton, "set_name", helper->createcsstr("MainMenuModsButton"));
                Il2CppObject* modsButtonTransform;
                helper->RunMethod(&modsButtonTransform, modsButton, "get_transform");
                helper->RunMethod(modsButtonTransform, "SetParent", settingsButtonTransformParent, &boolFalse);
                        
                UnityHelper::AddButtonOnClick(helper, buttonBinder, modsButton, (UnityHelper::ButtonOnClickFunction*)ButtonModsOnClick);

                helper->RunMethod(&assetLoaderFinishedButton, nullptr, helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine", "Object"), "Instantiate", 1), modsButton);
                helper->RunMethod(assetLoaderFinishedButton, "set_name", helper->createcsstr("AssetLoaderFinishedButton"));
                UnityHelper::SetParent(helper, assetLoaderFinishedButton, modsButton);
                UnityHelper::SetGameObjectActive(helper, assetLoaderFinishedButton, false);
            }
            if(loaderInstance){
                helper->RunMethod(&menuButtons, menuTransformParent, "GetComponentsInChildren", helper->type_get_object(helper->class_get_type(helper->GetClassFromName("UnityEngine.UI", "Button"))), &boolFalse); 
                if(assetBundle == nullptr){
                    UnityAssetLoader::LoadAssetBundleFromFileAsync("/sdcard/Android/data/com.beatgames.beatsaber/files/uis/questUI.qui", (UnityAssetLoader_OnLoadAssetBundleCompleteFunction*)OnLoadAssetBundleComplete);
                }else{
                    UnityAssetLoader::LoadAssetFromAssetBundleAsync(assetBundle, (UnityAssetLoader_OnLoadAssetCompleteFunction*)OnLoadAssetComplete);
                }
            }
            if(assetLoaderFinishedButton != nullptr && !assetLoaderFinishedButtonExist){
                Il2CppObject* onClick;
                helper->RunMethod(&onClick, assetLoaderFinishedButton, "get_onClick");
                auto action = helper->MakeAction(nullptr, AssetLoaderFinishedButtonOnClick, helper->class_get_type(helper->GetClassFromName("UnityEngine.Events", "UnityAction")));
                helper->RunMethod(onClick, "AddListener", action);
            }
        }
        if(loaderInstance)
            SetUIActive(false);
        MainMenuViewController_DidActivate(self, firstActivation, type);   
    }

    void Initialize(char* modName, VoidFunction* onInitialized){
        questUIModInfo.Name = modName;
        questUIModInfo.OnInitialized = onInitialized;
        if(helper == nullptr){
            helper = new IL2CPP_Helper();
            helper->Initialize();
        }
        INSTALL_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, helper->class_get_method_from_name(helper->GetClassFromName("", "MainMenuViewController"), "DidActivate", 2));
        INSTALL_HOOK_OFFSETLESS(SceneManager_SetActiveScene, helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine.SceneManagement", "SceneManager"), "SetActiveScene", 1));
        log(INFO, "QuestUI: Installed Hook!");
    }

    QuestUIModInfo GetQuestUIModInfo(){
        return questUIModInfo;
    }

    QuestUIInfo* GetQuestUIInfo(){
        return questUIInfo;
    }

}