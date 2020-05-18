#include "questui.hpp"

#include <thread>
#include <vector>

#include "../CustomSabers/AssetBundle.hpp"
#include "../beatsaber-hook/shared/utils/logging.hpp"
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"

namespace QuestUI {
    static bool loaderInstance = false;

    static QuestUIInfo* questUIInfo;
    static QuestUIModInfo* questUIModInfo;

    static Il2CppObject* menuTransformParent = nullptr;
    static Il2CppObject* buttonBinder = nullptr;
    static Array<Il2CppObject*>* menuButtons = nullptr;

    static Il2CppObject* modsButton = nullptr;
    static Il2CppObject* assetLoaderFinishedButton = nullptr;
    static Il2CppObject* customUIObject = nullptr;
    static AssetBundle* assetBundle = nullptr;

    static Il2CppObject* buttonModsListUp = nullptr;
    static Il2CppObject* buttonModsListDown = nullptr;

    static int listIndex = 0;
    static Il2CppObject* activeButtonMod = nullptr;

    void SetUIActive(bool active) {
        if (customUIObject) {
            CRASH_UNLESS(il2cpp_utils::RunMethod(customUIObject, "SetActive", active));
            if (menuButtons) {
                for (int i = 0; i < menuButtons->Length(); i++) {
                    auto* name = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<Il2CppString*>(menuButtons->values[i], "name"));
                    if (to_utf8(csstrtostr(name)) != "BeatmapEditorButton")
                        UnityHelper::SetGameObjectActive(menuButtons->values[i], !active);
                }
            }
        }
        if (!active && modsButton) {
            UnityHelper::SetButtonText(modsButton, "Mods");
        }
    }

    void RefreshList() {
        float buttonMod_TemplateTransformHeight = 27.5f;
        float modsListObjectTransformHeight = 220.0f;
        float listCount = modsListObjectTransformHeight / buttonMod_TemplateTransformHeight;
        Color textColorEnabled{0.0f, 184.0f / 255.0f, 1.0f, 1.0f};
        Color textColorDisabled{0.0f, 184.0f / 255.0f, 1.0f, 36.0f / 255.0f};
        bool buttonModsListUpEnabled = listIndex > 0;
        bool buttonModsListDownEnabled = questUIInfo->Mods.size() - listCount - listIndex > 0;
        UnityHelper::SetButtonEnabled(buttonModsListUp, buttonModsListUpEnabled);
        UnityHelper::SetButtonTextColor(buttonModsListUp, buttonModsListUpEnabled ? textColorEnabled : textColorDisabled);
        UnityHelper::SetButtonEnabled(buttonModsListDown, buttonModsListDownEnabled);
        UnityHelper::SetButtonTextColor(buttonModsListDown, buttonModsListDownEnabled ? textColorEnabled : textColorDisabled);

        auto* buttonMod_Template = UnityHelper::GetComponentInChildren(
            customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonMod_Template");
        auto* buttonMod_TemplateTransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(buttonMod_Template, "transform"));
        Vector3 buttonMod_TemplateTransformPosition = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<Vector3>(
            buttonMod_TemplateTransform, "localPosition"));
        Vector3 buttonPosition = buttonMod_TemplateTransformPosition;
        for (int i = 0; i < questUIInfo->Mods.size(); i++) {
            QuestUIModInfo* modInfo = questUIInfo->Mods[i];
            if (i >= listIndex && i < listIndex + listCount) {
                UnityHelper::SetGameObjectActive(modInfo->Button, true);
                auto* modInfoButtonTransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(modInfo->Button, "transform"));
                il2cpp_utils::SetPropertyValue(modInfoButtonTransform, "localPosition", buttonPosition);
                buttonPosition.y -= buttonMod_TemplateTransformHeight;
            } else {
                UnityHelper::SetGameObjectActive(modInfo->Button, false);
            }

            Color textColor;
            UnityHelper::ColorBlock colors = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<UnityHelper::ColorBlock>(
                modInfo->Button, "colors"));

            if (modInfo->Button == activeButtonMod) {
                UnityHelper::SetGameObjectActive(modInfo->Panel, true);
                textColor.r = textColor.g = textColor.b = 0.0f;
                textColor.a = 1.0f;
                colors.m_NormalColor.r = colors.m_NormalColor.g = colors.m_NormalColor.b = colors.m_NormalColor.a = 1.0f;
                colors.m_HighlightedColor = colors.m_NormalColor;
            } else {
                UnityHelper::SetGameObjectActive(modInfo->Panel, false);
                textColor.r = textColor.g = textColor.b = textColor.a = 1.0f;
                colors.m_NormalColor.a = 0.0f;
                colors.m_HighlightedColor.r = 0.0f;
                colors.m_HighlightedColor.g = 142.0f / 255.0f;
                colors.m_HighlightedColor.b = 205.0f / 255.0f;
                colors.m_HighlightedColor.a = 164.0f / 255.0f;
            }
            //CRASH_UNLESS(il2cpp_utils::RunMethod<UnityHelper::ColorBlock>(modInfo->Button, "set_colors", colors));
            UnityHelper::SetButtonTextColor(modInfo->Button, textColor);
        }
    }

    #pragma region ButtonHandlers
    void ButtonModsOnClick(Il2CppObject* button) {
        log(INFO, "QuestUI: ButtonModsOnClick!");
        SetUIActive(true);
    }

    void ButtonModsListUpOnClick(Il2CppObject* button) {
        listIndex--;
        RefreshList();
        log(INFO, "QuestUI: ButtonModsListUpOnClick!");
    }

    void ButtonModsListDownOnClick(Il2CppObject* button) {
        listIndex++;
        RefreshList();
        log(INFO, "QuestUI: ButtonModsListDownOnClick!");
    }

    void ButtonBackOnClick(Il2CppObject* button) {
        log(INFO, "QuestUI: ButtonBackOnClick!");
        SetUIActive(false);
    }

    void ButtonModOnClick(Il2CppObject* button) {
        log(INFO, "QuestUI: ButtonModOnClick!");
        activeButtonMod = button;
        RefreshList();
    }
    #pragma endregion

    void AssetLoaderFinishedButtonOnClick() {
        if (!customUIObject) {
            auto* tTMProUGUI = il2cpp_utils::GetSystemType("TMPro", "TextMeshProUGUI");
            auto* assetLoaderFinishedButtonTextObject = CRASH_UNLESS(il2cpp_utils::RunMethod(
                assetLoaderFinishedButton, "GetComponentInChildren", tTMProUGUI, true));

            auto* nameObject = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<Il2CppString*>(
                assetLoaderFinishedButtonTextObject, "text"));
            // TODO: figure out a way to singleton this across mods without insanity
            // restores the universal questUIInfo pointer from this button text
            sscanf(to_utf8(csstrtostr(nameObject)).c_str(), "%llx", (unsigned long long*)&questUIInfo);
        }
        questUIInfo->Mods.push_back(questUIModInfo);
        log(INFO, "QuestUI: Added mod %s to QuestUIInfo", questUIModInfo->Name.c_str());
    }

    void OnInitializedThread(QuestUIModInfo* modInfo) {
        if (!modInfo || !modInfo->OnInitialized) return;
        sleep(1);
        ((void (*)(void))modInfo->OnInitialized)();
    }

    void AddTestMods() {
        for (int i = 1; i <= 10; i++) {
            auto* mod = new QuestUIModInfo();
            mod->Name = "TestMod " + std::to_string(i);
            questUIInfo->Mods.push_back(mod);
        }
    }

    void OnLoadAssetComplete(Asset* asset) {
        log(INFO, "QuestUI: OnLoadAssetComplete Called!");
        customUIObject = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "Instantiate", asset));
        CRASH_UNLESS(il2cpp_utils::SetPropertyValue(customUIObject, "name", il2cpp_utils::createcsstr("CustomUIObject")));
        CRASH_UNLESS(il2cpp_utils::RunMethod(customUIObject, "SetActive", false));

        auto* customUITransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(customUIObject, "transform"));
        CRASH_UNLESS(il2cpp_utils::RunMethod(customUITransform, "SetParent", menuTransformParent, false));

        buttonModsListUp = UnityHelper::GetComponentInChildren(
            customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonModsListUp");
        buttonModsListDown = UnityHelper::GetComponentInChildren(
            customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonModsListDown");

        UnityHelper::AddButtonOnClick(
            buttonBinder, customUIObject, "ButtonBack", (UnityHelper::ButtonOnClickFunction*)ButtonBackOnClick);
        UnityHelper::AddButtonOnClick(buttonBinder, buttonModsListUp, (UnityHelper::ButtonOnClickFunction*)ButtonModsListUpOnClick);
        UnityHelper::AddButtonOnClick(
            buttonBinder, buttonModsListDown, (UnityHelper::ButtonOnClickFunction*)ButtonModsListDownOnClick);

        for (auto* modInfo : questUIInfo->Mods) {
            delete modInfo;
        }
        delete questUIInfo;
        questUIInfo = new QuestUIInfo();
        questUIInfo->CustomUIObject = customUIObject;
        questUIInfo->ButtonBinder = buttonBinder;

        char buffer[9];
        sprintf(buffer, "%llx", (unsigned long long)questUIInfo);  // stores the new universal questUIInfo pointer to this button text
        UnityHelper::SetButtonText(assetLoaderFinishedButton, buffer);
        auto* onClick = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(assetLoaderFinishedButton, "onClick"));
        il2cpp_utils::RunMethod(onClick, "Invoke");

        // AddTestMods();

        log(INFO, "QuestUI: %lu Mods Loaded!", questUIInfo->Mods.size());

        Il2CppObject* buttonMod_Template = UnityHelper::GetComponentInChildren(
            customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "ButtonMod_Template");
        UnityHelper::SetGameObjectActive(buttonMod_Template, false);
        Il2CppObject* panelMod_Template = UnityHelper::GetComponentInChildren(
            customUIObject, il2cpp_utils::GetClassFromName("UnityEngine", "Object"), "PanelMod_Template");
        UnityHelper::SetGameObjectActive(panelMod_Template, false);

        for (QuestUIModInfo* modInfo : questUIInfo->Mods) {
            modInfo->Button = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "Instantiate", buttonMod_Template));
            il2cpp_utils::SetPropertyValue(modInfo->Button, "name", il2cpp_utils::createcsstr("ButtonMod"));
            UnityHelper::SetSameParent(modInfo->Button, buttonMod_Template);

            UnityHelper::AddButtonOnClick(buttonBinder, modInfo->Button, (UnityHelper::ButtonOnClickFunction*)ButtonModOnClick);

            modInfo->Panel = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "Instantiate", panelMod_Template));
            CRASH_UNLESS(il2cpp_utils::SetPropertyValue(modInfo->Panel, "name", il2cpp_utils::createcsstr("PanelMod")));
            UnityHelper::SetSameParent(modInfo->Panel, panelMod_Template);

            UnityHelper::SetButtonText(modInfo->Button, modInfo->Name);

            std::thread onInitializedThread(OnInitializedThread, modInfo);
            onInitializedThread.detach();
        }
        if (questUIInfo->Mods.size() > 0) {
            activeButtonMod = questUIInfo->Mods[0]->Button;
        }
        RefreshList();
        log(INFO, "QuestUI: OnLoadAssetComplete Finished!");
    }

    void OnLoadAssetBundleComplete(AssetBundle* assetBundle) {
        assetBundle->LoadAssetAsync("_customasset", OnLoadAssetComplete);
    }

    MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, int scene) {
        auto* name = CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(
            "UnityEngine.SceneManagement", "Scene", "GetNameInternal", scene));
        log(INFO, "QuestUI: Loading Scene: %s", to_utf8(csstrtostr(name)).c_str());
        if (customUIObject) {
            il2cpp_utils::RunMethod("UnityEngine", "Object", "Destroy", UnityHelper::GetGameObject(customUIObject));
            customUIObject = nullptr;
            log(INFO, "QuestUI: Destroyed QuestUI!");
        }
        return SceneManager_SetActiveScene(scene);
    }

    MAKE_HOOK_OFFSETLESS(MainMenuViewController_DidActivate, void, Il2CppObject* self, bool firstActivation, int type) {
        if (customUIObject == nullptr) {
            log(INFO, "QuestUI: Loading QuestUI...");
            buttonBinder = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(self, "buttonBinder"));
            auto* settingsButton = CRASH_UNLESS(il2cpp_utils::GetFieldValue(self, "_settingsButton"));
            auto* settingsButtonTransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(settingsButton, "transform"));
            auto* settingsButtonTransformParent = CRASH_UNLESS(il2cpp_utils::RunMethod(settingsButtonTransform, "GetParent"));
            menuTransformParent = CRASH_UNLESS(il2cpp_utils::RunMethod(settingsButtonTransformParent, "GetParent"));
            menuTransformParent = CRASH_UNLESS(il2cpp_utils::RunMethod(menuTransformParent, "GetParent"));
            menuTransformParent = CRASH_UNLESS(il2cpp_utils::RunMethod(menuTransformParent, "GetParent"));

            modsButton = UnityHelper::GetComponentInChildren(
                settingsButtonTransformParent, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "MainMenuModsButton");
            assetLoaderFinishedButton = UnityHelper::GetComponentInChildren(settingsButtonTransformParent,
                il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), "AssetLoaderFinishedButton");
            if (!modsButton) {
                loaderInstance = true;
                modsButton = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "Instantiate", settingsButton));
                CRASH_UNLESS(il2cpp_utils::SetPropertyValue(modsButton, "name", il2cpp_utils::createcsstr("MainMenuModsButton")));
                auto* modsButtonTransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(modsButton, "transform"));
                CRASH_UNLESS(il2cpp_utils::RunMethod(modsButtonTransform, "SetParent", settingsButtonTransformParent, false));

                UnityHelper::AddButtonOnClick(buttonBinder, modsButton, (UnityHelper::ButtonOnClickFunction*)ButtonModsOnClick);

                assetLoaderFinishedButton = CRASH_UNLESS(il2cpp_utils::RunMethod(
                    "UnityEngine", "Object", "Instantiate", settingsButton));
                CRASH_UNLESS(il2cpp_utils::SetPropertyValue(assetLoaderFinishedButton, "name",
                    il2cpp_utils::createcsstr("AssetLoaderFinishedButton")));
                UnityHelper::SetSameParent(assetLoaderFinishedButton, settingsButton);
                UnityHelper::SetGameObjectActive(assetLoaderFinishedButton, false);
            }
            if (loaderInstance) {
                auto* tButton = il2cpp_utils::GetSystemType("UnityEngine.UI", "Button");
                menuButtons = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(
                    menuTransformParent, "GetComponentsInChildren", tButton, false));

                if (assetLoaderFinishedButton) {
                    auto* onClick = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(assetLoaderFinishedButton, "onClick"));
                    auto* m_Calls = CRASH_UNLESS(il2cpp_utils::GetFieldValue(onClick, "m_Calls"));
                    auto* m_RunTimeCalls = CRASH_UNLESS(il2cpp_utils::GetFieldValue(m_Calls, "m_RuntimeCalls"));
                    il2cpp_utils::RunMethod(m_RunTimeCalls, "Clear");
                    // il2cpp_utils::SetFieldValue(m_Calls, "m_NeedsUpdate", true); //RS
                }
                if (!assetBundle) {
                    log(INFO, "UnityAssetLoader: Loading from questUI.qui");
                    AssetBundle::LoadFromFileAsync("/sdcard/Android/data/com.beatgames.beatsaber/files/uis/questUI.qui",
                        OnLoadAssetBundleComplete);
                } else {
                    OnLoadAssetBundleComplete(assetBundle);
                }
            }
            if (assetLoaderFinishedButton) {
                auto* onClick = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(assetLoaderFinishedButton, "onClick"));
                auto* method = il2cpp_utils::FindMethodUnsafe(onClick, "AddListener", 1);
                auto* action = il2cpp_utils::MakeAction(method, 0, nullptr, AssetLoaderFinishedButtonOnClick);
                il2cpp_utils::RunMethod(onClick, method, action);
            }
            log(INFO, "QuestUI: Loaded QuestUI!");
        }
        if (loaderInstance) SetUIActive(false);
        MainMenuViewController_DidActivate(self, firstActivation, type);
    }

    void Initialize(std::string_view modName, VoidFunction* onInitialized) {
        questUIModInfo = new QuestUIModInfo();
        questUIModInfo->Name = modName;
        questUIModInfo->OnInitialized = onInitialized;
        il2cpp_functions::Init();
        INSTALL_HOOK_OFFSETLESS(
            MainMenuViewController_DidActivate, il2cpp_utils::FindMethodUnsafe("", "MainMenuViewController", "DidActivate", 2));
        INSTALL_HOOK_OFFSETLESS(SceneManager_SetActiveScene,
            il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "SetActiveScene", 1));
        log(INFO, "QuestUI: Installed Hook!");
    }

    QuestUIModInfo* GetQuestUIModInfo() { return questUIModInfo; }

    QuestUIInfo* GetQuestUIInfo() { return questUIInfo; }
}  // namespace QuestUI
