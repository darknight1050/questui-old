#include "questui.hpp"
#include <vector>
#include <thread>

namespace QuestUI {

    static bool boolTrue = true;
    static bool boolFalse = false;

    static bool loaderInstance = false;
   
    static QuestUIInfo* questUIInfo = nullptr;
    static QuestUIModInfo questUIModInfo;

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
    static Il2CppObject* activeButtonMod = nullptr;
 
    void SetUIActive(bool active){
        if(customUIObject != nullptr){
            il2cpp_utils::RunMethod(customUIObject, "SetActive", active);
            if(menuButtons != nullptr){
                for(int i = 0;i<menuButtons->Length();i++){
                    Il2CppString* nameObject;
                     nameObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(menuButtons->values[i], "get_name"));
                    if(strcmp(to_utf8(csstrtostr(nameObject)).c_str(), "BeatmapEditorButton") != 0)
                        UnityHelper::SetGameObjectActive(menuButtons->values[i], !active);
                }
            }
        }
        if(!active && modsButton != nullptr){
            UnityHelper::SetButtonText(modsButton, "Mods");
        }
    }
    

    void RefreshList(){
        float buttonMod_TemplateTransformHeight = 27.5f;
        float modsListObjectTransformHeight = 220.0f;
        float listCount = modsListObjectTransformHeight/buttonMod_TemplateTransformHeight;
        Color textColorEnabled;
        textColorEnabled.r = 0.0f;
        textColorEnabled.g = 184.0f/255.0f;
        textColorEnabled.b = 1.0f;
        textColorEnabled.a = 1.0f;
        Color textColorDisabled;
        textColorDisabled.r = 0.0f;
        textColorDisabled.g = 184.0f/255.0f;
        textColorDisabled.b = 1.0f;
        textColorDisabled.a = 36.0f/255.0f;
        bool buttonModsListUpEnabled = listIndex > 0;
        bool buttonModsListDownEnabled = questUIInfo->Mods->size()-listCount-listIndex > 0;
        UnityHelper::SetButtonEnabled(buttonModsListUp, buttonModsListUpEnabled);
        UnityHelper::SetButtonTextColor(buttonModsListUp, buttonModsListUpEnabled ? textColorEnabled : textColorDisabled);
        UnityHelper::SetButtonEnabled(buttonModsListDown, buttonModsListDownEnabled);
        UnityHelper::SetButtonTextColor(buttonModsListDown, buttonModsListDownEnabled ? textColorEnabled : textColorDisabled);
             
        Il2CppObject* buttonMod_Template = UnityHelper::GetComponentInChildren(customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonMod_Template");
        Il2CppObject* buttonMod_TemplateTransform;
        buttonMod_TemplateTransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(buttonMod_Template, "get_transform"));
        Vector3 buttonMod_TemplateTransformPosition;
        buttonMod_TemplateTransformPosition = *CRASH_UNLESS(il2cpp_utils::RunMethod<Vector3>(buttonMod_TemplateTransform, "get_localPosition"));
        Vector3 buttonPosition = buttonMod_TemplateTransformPosition;
        for(int i = 0;i<questUIInfo->Mods->size();i++){
            QuestUIModInfo* modInfo = (*questUIInfo->Mods)[i];
            if(i >= listIndex && i < listIndex + listCount){
                UnityHelper::SetGameObjectActive(modInfo->Button, true);
                Il2CppObject* modInfoButtonTransform;
                modInfoButtonTransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(modInfo->Button, "get_transform"));
                il2cpp_utils::RunMethod(modInfoButtonTransform, "set_localPosition", buttonPosition);
                buttonPosition.y -= buttonMod_TemplateTransformHeight;
            }else{
                UnityHelper::SetGameObjectActive(modInfo->Button, false);
            }
            Color textColor;
            UnityHelper::ColorBlock colors;
            colors = *CRASH_UNLESS(il2cpp_utils::RunMethod<UnityHelper::ColorBlock>(modInfo->Button, "get_colors"));
            if(modInfo->Button == activeButtonMod){
                UnityHelper::SetGameObjectActive(modInfo->Panel, true);
                textColor.r = textColor.g = textColor.b = 0.0f;
                textColor.a = 1.0f;
                colors.m_NormalColor.r = colors.m_NormalColor.g = colors.m_NormalColor.b = colors.m_NormalColor.a = 1.0f;
                colors.m_HighlightedColor = colors.m_NormalColor;
            }else{
                UnityHelper::SetGameObjectActive(modInfo->Panel, false);
                textColor.r = textColor.g = textColor.b = textColor.a = 1.0f;
                colors.m_NormalColor.a = 0.0f;
                colors.m_HighlightedColor.r = 0.0f;
                colors.m_HighlightedColor.g = 142.0f/255.0f;
                colors.m_HighlightedColor.b = 205.0f/255.0f;
                colors.m_HighlightedColor.a = 164.0f/255.0f;
            }
            //*CRASH_UNLESS(il2cpp_utils::RunMethod<UnityHelper::ColorBlock>(modInfo->Button, "set_colors", colors));
            UnityHelper::SetButtonTextColor(modInfo->Button, textColor);
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
        activeButtonMod = button;
        RefreshList();
    }
    #pragma endregion
    
    void AssetLoaderFinishedButtonOnClick(){
        if(customUIObject == nullptr){
            Il2CppObject* assetLoaderFinishedButtonTextObject;
            assetLoaderFinishedButtonTextObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(assetLoaderFinishedButton, "GetComponentInChildren", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("TMPro", "TextMeshProUGUI"))), boolTrue));
            Il2CppString* nameObject;
            nameObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(assetLoaderFinishedButtonTextObject, "get_text"));
            sscanf(to_utf8(csstrtostr(nameObject)).c_str(), "%llx", &questUIInfo);
        }
        questUIInfo->Mods->push_back(&questUIModInfo);
        log(INFO, "QuestUI: Added mod %s to QuestUIInfo: %p", questUIModInfo.Name, questUIInfo);
    }

    void OnInitializedThread(QuestUIModInfo* modInfo){
        if(modInfo == nullptr || modInfo->OnInitialized == nullptr)
            return;
        sleep(1);
        ((void (*)(void)) modInfo->OnInitialized)();
    }

    void AddTestMods(){
        QuestUIModInfo* info = new QuestUIModInfo();
        info->Name = "TestMod 1";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 2";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 3";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 4";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 5";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 6";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 7";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 8";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 9";
        questUIInfo->Mods->push_back(info);
        info = new QuestUIModInfo();
        info->Name = "TestMod 10";
        questUIInfo->Mods->push_back(info);
    }

    void OnLoadAssetComplete(Il2CppObject* assetArg){
        log(INFO, "QuestUI: OnLoadAssetComplete Called!");
        il2cpp_utils::RunMethod(&customUIObject, il2cpp_utils::FindMethodUnsafe("UnityEngine", "Object", "Instantiate", 1), assetArg);
        il2cpp_utils::RunMethod(customUIObject, "set_name", il2cpp_utils::createcsstr("CustomUIObject"));
        il2cpp_utils::RunMethod(customUIObject, "SetActive", boolFalse);

        Il2CppObject* customUITransform;
        customUITransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(customUIObject, "get_transform"));
        il2cpp_utils::RunMethod(customUITransform, "SetParent", menuTransformParent, boolFalse);
       
        buttonModsListUp = UnityHelper::GetComponentInChildren(customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonModsListUp");
        buttonModsListDown = UnityHelper::GetComponentInChildren(customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonModsListDown");
       
        UnityHelper::AddButtonOnClick(buttonBinder, customUIObject, "ButtonBack", (UnityHelper::ButtonOnClickFunction*)ButtonBackOnClick);
        UnityHelper::AddButtonOnClick(buttonBinder, buttonModsListUp, (UnityHelper::ButtonOnClickFunction*)ButtonModsListUpOnClick);
        UnityHelper::AddButtonOnClick(buttonBinder, buttonModsListDown, (UnityHelper::ButtonOnClickFunction*)ButtonModsListDownOnClick);

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
        UnityHelper::SetButtonText(assetLoaderFinishedButton, buffer);
        Il2CppObject* onClick;
        onClick = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(assetLoaderFinishedButton, "get_onClick"));
        il2cpp_utils::RunMethod(onClick, "Invoke");
        
        //AddTestMods();

        log(INFO, "QuestUI: %lu Mods Loaded!", questUIInfo->Mods->size());

        Il2CppObject* buttonMod_Template = UnityHelper::GetComponentInChildren(customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonMod_Template");
        UnityHelper::SetGameObjectActive(buttonMod_Template, false);
        Il2CppObject* panelMod_Template = UnityHelper::GetComponentInChildren(customUIObject, il2cpp_utils::GetClassFromName("UnityEngine", "Object"), "PanelMod_Template");
        UnityHelper::SetGameObjectActive(panelMod_Template, false);

        for(QuestUIModInfo* modInfo : *questUIInfo->Mods){
            il2cpp_utils::RunMethod(&modInfo->Button, il2cpp_utils::FindMethodUnsafe("UnityEngine", "Object", "Instantiate", 1), buttonMod_Template);
            il2cpp_utils::RunMethod(modInfo->Button, "set_name", il2cpp_utils::createcsstr("ButtonMod"));
            UnityHelper::SetSameParent(modInfo->Button, buttonMod_Template);

            UnityHelper::AddButtonOnClick(buttonBinder, modInfo->Button, (UnityHelper::ButtonOnClickFunction*)ButtonModOnClick);

            il2cpp_utils::RunMethod(&modInfo->Panel, il2cpp_utils::FindMethodUnsafe("UnityEngine", "Object", "Instantiate", 1), panelMod_Template);
            il2cpp_utils::RunMethod(modInfo->Panel, "set_name", il2cpp_utils::createcsstr("PanelMod"));
            UnityHelper::SetSameParent(modInfo->Panel, panelMod_Template);

            UnityHelper::SetButtonText(modInfo->Button, modInfo->Name);
            
            std::thread onInitializedThread(OnInitializedThread, modInfo);
            onInitializedThread.detach();
        }
        if(questUIInfo->Mods->size() > 0){
           activeButtonMod = (*questUIInfo->Mods)[0]->Button;
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
        nameObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(il2cpp_utils::GetClassFromName("UnityEngine.SceneManagement", "Scene"), "GetNameInternal", scene));
        log(ANDROID_LOG_INFO, "QuestUI: Loading Scene: %s", to_utf8(csstrtostr(nameObject)).c_str());
        if(customUIObject != nullptr){
            il2cpp_utils::RunMethod(il2cpp_utils::GetClassFromName("UnityEngine", "Object"), "Destroy", UnityHelper::GetGameObject(customUIObject));
            customUIObject = nullptr;
            log(INFO, "QuestUI: Destroyed QuestUI!");
        }
        return SceneManager_SetActiveScene(scene);
    }
    static AssetImporter* importerQUEST;
    MAKE_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, void, Il2CppObject* self, bool firstActivation, int type){
        if(customUIObject == nullptr){
            log(INFO, "QuestUI: Loading QuestUI...");
            buttonBinder = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(self, "get__buttonBinder"));
            Il2CppObject* settingsButton = *CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppObject*>(self, "_settingsButton"));
            Il2CppObject* settingsButtonTransform;
            settingsButtonTransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(settingsButton, "get_transform"));
            Il2CppObject* settingsButtonTransformParent;
            settingsButtonTransformParent = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(settingsButtonTransform, "GetParent"));
            menuTransformParent = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(settingsButtonTransformParent, "GetParent"));
            menuTransformParent = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(menuTransformParent, "GetParent"));
            menuTransformParent = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(menuTransformParent, "GetParent"));
           
            modsButton = UnityHelper::GetComponentInChildren(settingsButtonTransformParent, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "MainMenuModsButton");
            assetLoaderFinishedButton = UnityHelper::GetComponentInChildren(settingsButtonTransformParent, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "AssetLoaderFinishedButton");
            if(modsButton == nullptr){
                loaderInstance = true;
                il2cpp_utils::RunMethod(&modsButton, il2cpp_utils::FindMethodUnsafe("UnityEngine", "Object", "Instantiate", 1), settingsButton);
                il2cpp_utils::RunMethod(modsButton, "set_name", il2cpp_utils::createcsstr("MainMenuModsButton"));
                Il2CppObject* modsButtonTransform;
                modsButtonTransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(modsButton, "get_transform"));
                il2cpp_utils::RunMethod(modsButtonTransform, "SetParent", settingsButtonTransformParent, boolFalse);
                        
                UnityHelper::AddButtonOnClick(buttonBinder, modsButton, (UnityHelper::ButtonOnClickFunction*)ButtonModsOnClick);

                il2cpp_utils::RunMethod(&assetLoaderFinishedButton, il2cpp_utils::FindMethodUnsafe("UnityEngine", "Object", "Instantiate", 1), settingsButton);
                il2cpp_utils::RunMethod(assetLoaderFinishedButton, "set_name", il2cpp_utils::createcsstr("AssetLoaderFinishedButton"));
                UnityHelper::SetSameParent(assetLoaderFinishedButton, settingsButton);
                UnityHelper::SetGameObjectActive(assetLoaderFinishedButton, false);
            }
            if(loaderInstance){
                menuButtons = *CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(menuTransformParent, "GetComponentsInChildren", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"))), boolFalse));
                if(assetLoaderFinishedButton != nullptr){
                    Il2CppObject* onClick;
                    onClick = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(assetLoaderFinishedButton, "get_onClick"));
                    Il2CppObject* m_Calls = *CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppObject*>(onClick, "m_Calls"));
                    Il2CppObject* m_RunTimeCalls = *CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppObject*>(m_Calls, "m_RuntimeCalls"));
                    il2cpp_utils::RunMethod(m_RunTimeCalls, "Clear");
                    //il2cpp_utils::SetFieldValue(m_Calls, "m_NeedsUpdate", boolTrue); //RS
                }
                if(assetBundle == nullptr){
                    log(INFO, "UnityAssetLoader: Loading from questUI.qui");
                    
                    if (!importerQUEST) {
                        log(DEBUG, "Making importer");
                        importerQUEST = new AssetImporter("/sdcard/Android/data/com.beatgames.beatsaber/files/uis/questUI.qui", "_customasset");
                        importerQUEST->LoadAssetBundle((UnityAssetLoader_OnLoadAssetCompleteFunction*)OnLoadAssetComplete);
                    }
                   // UnityAssetLoader::LoadAssetBundleFromFileAsync("/sdcard/Android/data/com.beatgames.beatsaber/files/uis/questUI.qui", (UnityAssetLoader_OnLoadAssetBundleCompleteFunction*)OnLoadAssetBundleComplete);
                }else{
                    importerQUEST->LoadAsset((UnityAssetLoader_OnLoadAssetCompleteFunction*)OnLoadAssetComplete,"_customasset");
                    //UnityAssetLoader::LoadAssetFromAssetBundleAsync(assetBundle, (UnityAssetLoader_OnLoadAssetCompleteFunction*)OnLoadAssetComplete);
                }
            }
            if(assetLoaderFinishedButton != nullptr){
                Il2CppObject* onClick;
                onClick = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(assetLoaderFinishedButton, "get_onClick"));
                auto action = il2cpp_utils::MakeAction(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("UnityEngine.Events", "UnityAction")),nullptr, AssetLoaderFinishedButtonOnClick);
                il2cpp_utils::RunMethod(onClick, "AddListener", action);
            }
            log(INFO, "QuestUI: Loaded QuestUI!");
        }
        if(loaderInstance)
            SetUIActive(false);
        MainMenuViewController_DidActivate(self, firstActivation, type);   
    }

    void Initialize(char* modName, VoidFunction* onInitialized){
        questUIModInfo.Name = modName;
        questUIModInfo.OnInitialized = onInitialized;
        il2cpp_functions::Init();
        INSTALL_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, il2cpp_functions::class_get_method_from_name(il2cpp_utils::GetClassFromName("", "MainMenuViewController"), "DidActivate", 2));
        INSTALL_HOOK_OFFSETLESS(SceneManager_SetActiveScene, il2cpp_functions::class_get_method_from_name(il2cpp_utils::GetClassFromName("UnityEngine.SceneManagement", "SceneManager"), "SetActiveScene", 1));
        log(INFO, "QuestUI: Installed Hook!");
    }

    QuestUIModInfo GetQuestUIModInfo(){
        return questUIModInfo;
    }

    QuestUIInfo* GetQuestUIInfo(){
        return questUIInfo;
    }

}