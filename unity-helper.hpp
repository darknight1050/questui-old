#pragma once
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include <string_view>
#include <vector>

namespace UnityHelper {

    typedef struct ColorBlock {
        Color m_NormalColor;
        Color m_HighlightedColor;
        Color m_PressedColor;
        Color m_SelectedColor;
        Color m_DisabledColor;
        float m_ColorMultiplier;
        float m_FadeDuration;
    } ColorBlock;

    typedef void ButtonOnClickFunction(Il2CppObject* button);

    std::vector<Il2CppObject*> FindGameObjectsWithTag(std::string_view name);
    Il2CppObject* FindGameObjectWithTag(std::string_view name);
    Il2CppObject* FindGameObject(std::string_view name);
  
    // TODO: default tComp to UnityEngine::Object if nullptr is passed?
    std::vector<Il2CppObject*> GetComponentsInParent(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name);
    Il2CppObject* GetComponentInParent(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name);
    
    std::vector<Il2CppObject*> GetComponentsInChildren(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name);
    Il2CppObject* GetComponentInChildren(Il2CppObject* parentObject, Il2CppReflectionType* tComp, std::string_view name);

    Il2CppObject* GetGameObject(Il2CppObject* object);

    void DontDestroyOnLoad(Il2CppObject* object);
    void SetActive(Il2CppObject* object, bool active);
    void SetGameObjectActive(Il2CppObject* object, bool active);

    Il2CppObject* GetParentTransform(Il2CppObject* object);
    void SetParent(Il2CppObject* object, Il2CppObject* parent);
    void SetSameParent(Il2CppObject* object, Il2CppObject* parent);
    
    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* customUIObject, std::string_view name, ButtonOnClickFunction* handler);
    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* button, ButtonOnClickFunction* handler);
    void SetButtonText(Il2CppObject* button, std::string text);
    void SetButtonTextColor(Il2CppObject* button, Color color);
    void SetButtonEnabled(Il2CppObject* button, bool enabled);

    bool GetToggleIsOn(Il2CppObject* toggle);
    void SetToggleIsOn(Il2CppObject* toggle, bool isOn);

}
DEFINE_IL2CPP_ARG_TYPE(UnityHelper::ColorBlock, "UnityEngine.UI", "ColorBlock");
