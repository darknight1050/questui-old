#include "../beatsaber-hook/shared/utils/utils.h"

namespace UnityHelper {

    std::vector<Il2CppObject*> GetComponentsInParent(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    Il2CppObject* GetComponentInParent(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    
    std::vector<Il2CppObject*> GetComponentsInChildren(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);
    Il2CppObject* GetComponentInChildren(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name);

    void SetActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active);
    void SetGameObjectActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active);
    void SetParent(IL2CPP_Helper* helper, Il2CppObject* object, Il2CppObject* parent);
    void SetSameParent(IL2CPP_Helper* helper, Il2CppObject* object, Il2CppObject* parent);
    
    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* customUIObject, char* name, void* handler);
    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* button, void* handler);
    void SetButtonText(IL2CPP_Helper* helper, Il2CppObject* button, char* text);
    void SetButtonEnabled(IL2CPP_Helper* helper, Il2CppObject* button, bool enabled);

}