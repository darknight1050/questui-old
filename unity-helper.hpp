#ifndef UNITY_HELPER_H
#define UNITY_HELPER_H

#include "../beatsaber-hook/shared/utils/utils.h"

namespace UnityHelper {

    typedef struct ColorBlock {
        Color m_NormalColor;
        Color m_HighlightedColor;
        Color m_PressedColor;
        Color m_DisabledColor;
        float m_ColorMultiplier;
	    float m_FadeDuration;
    } ColorBlock;

    typedef void ButtonOnClickFunction(Il2CppObject* button);

    std::vector<Il2CppObject*> FindGameObjectsWithTag(char* name);
    Il2CppObject* FindGameObjectWithTag(char* name);
    Il2CppObject* FindGameObject(char* name);
  
    std::vector<Il2CppObject*> GetComponentsInParent(Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    Il2CppObject* GetComponentInParent(Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    
    std::vector<Il2CppObject*> GetComponentsInChildren(Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    Il2CppObject* GetComponentInChildren(Il2CppObject* parentObject, Il2CppClass* klass, char* name);

    Il2CppObject* GetGameObject(Il2CppObject* object);

    void DontDestroyOnLoad(Il2CppObject* object);
    void SetActive(Il2CppObject* object, bool active);
    void SetGameObjectActive(Il2CppObject* object, bool active);

    Il2CppObject* GetParent(Il2CppObject* object);
    void SetParent(Il2CppObject* object, Il2CppObject* parent);
    void SetSameParent(Il2CppObject* object, Il2CppObject* parent);
    
    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* customUIObject, char* name, ButtonOnClickFunction* handler);
    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* button, ButtonOnClickFunction* handler);
    void SetButtonText(Il2CppObject* button, std::string text);
    void SetButtonTextColor(Il2CppObject* button, Color color);
    void SetButtonEnabled(Il2CppObject* button, bool enabled);

    bool GetToggleIsOn(Il2CppObject* toggle);
    void SetToggleIsOn(Il2CppObject* toggle, bool isOn);

}
#endif