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

    std::vector<Il2CppObject*> FindGameObjectsWithTag(IL2CPP_Helper* helper, char* name);
    Il2CppObject* FindGameObjectWithTag(IL2CPP_Helper* helper, char* name);
    Il2CppObject* FindGameObject(IL2CPP_Helper* helper, char* name);
  
    std::vector<Il2CppObject*> GetComponentsInParent(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    Il2CppObject* GetComponentInParent(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    
    std::vector<Il2CppObject*> GetComponentsInChildren(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    Il2CppObject* GetComponentInChildren(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);

    void DontDestroyOnLoad(IL2CPP_Helper* helper, Il2CppObject* object);
    void SetActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active);
    void SetGameObjectActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active);
    void SetParent(IL2CPP_Helper* helper, Il2CppObject* object, Il2CppObject* parent);
    void SetSameParent(IL2CPP_Helper* helper, Il2CppObject* object, Il2CppObject* parent);
    
    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* customUIObject, char* name, ButtonOnClickFunction* handler);
    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* button, ButtonOnClickFunction* handler);
    void SetButtonText(IL2CPP_Helper* helper, Il2CppObject* button, char* text);
    void SetButtonTextColor(IL2CPP_Helper* helper, Il2CppObject* button, Color color);
    void SetButtonEnabled(IL2CPP_Helper* helper, Il2CppObject* button, bool enabled);

}