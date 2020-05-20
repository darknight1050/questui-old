#include "unity-helper.hpp"

#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../beatsaber-hook/shared/utils/utils.h"

namespace UnityHelper {
    std::vector<Il2CppObject*> FindGameObjectsWithTag(std::string_view name) {
        auto* objects = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(
            "UnityEngine", "GameObject", "FindGameObjectsWithTag", il2cpp_utils::createcsstr(name)));
        std::vector<Il2CppObject*> componentsFound;
        for (int i = 0; i < objects->Length(); i++) {
            componentsFound.push_back(objects->values[i]);
        }
        return componentsFound;
    }

    Il2CppObject* FindGameObjectWithTag(std::string_view name) {
        std::vector<Il2CppObject*> componentsFound = FindGameObjectsWithTag(name);
        if (componentsFound.size() > 0) {
            return componentsFound[0];
        }
        return nullptr;
    }

    Il2CppObject* FindGameObject(std::string_view name) {
        return CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "GameObject", "Find", il2cpp_utils::createcsstr(name)));
    }

    std::vector<Il2CppObject*> GetComponentsInParent(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name) {
        auto* objects = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(
            parentObject, "GetComponentsInParent", tComp, true));
        std::vector<Il2CppObject*> componentsFound;
        for (int i = 0; i < objects->Length(); i++) {
            Il2CppObject* object = objects->values[i];
            if (object) {
                auto* nameObject = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<Il2CppString*>(object, "name"));
                if (to_utf8(csstrtostr(nameObject)) == name) {
                    componentsFound.push_back(object);
                }
            }
        }
        return componentsFound;
    }

    Il2CppObject* GetComponentInParent(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name) {
        std::vector<Il2CppObject*> componentsFound = GetComponentsInParent(parentObject, tComp, name);
        if (componentsFound.size() > 0) {
            return componentsFound[0];
        }
        return nullptr;
    }

    std::vector<Il2CppObject*> GetComponentsInChildren(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name) {
        auto* objects = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(
            parentObject, "GetComponentsInChildren", tComp, true));
        std::vector<Il2CppObject*> componentsFound;
        for (int i = 0; i < objects->Length(); i++) {
            Il2CppObject* object = objects->values[i];
            if (object) {
                auto* nameObject = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<Il2CppString*>(object, "name"));
                if (name == to_utf8(csstrtostr(nameObject))) {
                    componentsFound.push_back(object);
                }
            }
        }
        return componentsFound;
    }

    Il2CppObject* GetComponentInChildren(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name) {
        std::vector<Il2CppObject*> componentsFound = GetComponentsInChildren(parentObject, tComp, name);
        if (componentsFound.size() > 0) {
            return componentsFound[0];
        }
        return nullptr;
    }

    Il2CppObject* GetGameObject(Il2CppObject* object) {
        CRASH_UNLESS(object);
        return CRASH_UNLESS(il2cpp_utils::GetPropertyValue(object, "gameObject"));
    }

    void DontDestroyOnLoad(Il2CppObject* object) {
        CRASH_UNLESS(object);
        CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "DontDestroyOnLoad", object));
    }

    void SetActive(Il2CppObject* object, bool active) {
        CRASH_UNLESS(object);
        CRASH_UNLESS(il2cpp_utils::RunMethod(object, "SetActive", active));
    }

    void SetGameObjectActive(Il2CppObject* object, bool active) {
        Il2CppObject* gameObject = CRASH_UNLESS(GetGameObject(object));
        SetActive(gameObject, active);
    }

    Il2CppObject* GetParentTransform(Il2CppObject* object) {
        CRASH_UNLESS(object);
        auto* objT = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(object, "transform"));
        return CRASH_UNLESS(il2cpp_utils::RunMethod(objT, "GetParent"));
    }

    void SetParent(Il2CppObject* object, Il2CppObject* parent) {
        auto* parentTransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(parent, "transform"));
        auto* objectTransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(object, "transform"));
        il2cpp_utils::RunMethod(objectTransform, "SetParent", parentTransform, false);
    }

    void SetSameParent(Il2CppObject* object, Il2CppObject* objectTwo) {
        Il2CppObject* parent = GetParentTransform(objectTwo);
        auto* objectTransform = CRASH_UNLESS(il2cpp_utils::GetPropertyValue(object, "transform"));
        CRASH_UNLESS(il2cpp_utils::RunMethod(objectTransform, "SetParent", parent, false));
    }

    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* customUIObject, std::string_view name, ButtonOnClickFunction* handler) {
        auto* tButton = CRASH_UNLESS(il2cpp_utils::GetSystemType("UnityEngine.UI", "Button"));
        std::vector<Il2CppObject*> customUIButtons = GetComponentsInChildren(customUIObject, tButton, name);
        for (Il2CppObject* customUIButton : customUIButtons) {
            AddButtonOnClick(buttonBinder, customUIButton, handler);
        }
    }

    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* button, ButtonOnClickFunction* handler) {
        static auto* method = CRASH_UNLESS(il2cpp_utils::FindMethodUnsafe(buttonBinder, "AddBinding", 2));
        auto* action = CRASH_UNLESS(il2cpp_utils::MakeAction(method, 1, button, handler));
        CRASH_UNLESS(il2cpp_utils::RunMethod(buttonBinder, method, button, action));
    }

    Il2CppObject* _getButtonTextObject(Il2CppObject* button) {
        CRASH_UNLESS(button);
        auto* tTMProUGUI = CRASH_UNLESS(il2cpp_utils::GetSystemType("TMPro", "TextMeshProUGUI"));
        return CRASH_UNLESS(il2cpp_utils::RunMethod(button, "GetComponentInChildren", tTMProUGUI, true));
    }

    void SetButtonText(Il2CppObject* button, std::string text) {
        auto* buttonTextObject = CRASH_UNLESS(_getButtonTextObject(button));
        CRASH_UNLESS(il2cpp_utils::SetPropertyValue(buttonTextObject, "text", il2cpp_utils::createcsstr(text)));
    }

    void SetButtonTextColor(Il2CppObject* button, Color color) {
        auto* buttonTextObject = _getButtonTextObject(button);
        CRASH_UNLESS(il2cpp_utils::SetPropertyValue(buttonTextObject, "color", color));
    }

    void SetButtonEnabled(Il2CppObject* button, bool enabled) {
        CRASH_UNLESS(il2cpp_utils::SetPropertyValue(button, "interactable", enabled));
    }

    bool GetToggleIsOn(Il2CppObject* toggle) {
        CRASH_UNLESS(toggle);
        return CRASH_UNLESS(il2cpp_utils::GetPropertyValue<bool>(toggle, "isOn"));
    }

    void SetToggleIsOn(Il2CppObject* toggle, bool isOn) {
        CRASH_UNLESS(toggle);
        CRASH_UNLESS(il2cpp_utils::SetPropertyValue(toggle, "isOn", isOn));
    }
}  // namespace UnityHelper
