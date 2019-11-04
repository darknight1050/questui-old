#include "unity-helper.hpp"
#include <vector>

namespace UnityHelper {

    bool boolTrue = true;
    bool boolFalse = false;

    std::vector<Il2CppObject*> FindGameObjectsWithTag(IL2CPP_Helper* helper, char* name){
        std::vector<Il2CppObject*> componentsFound;
        Array<Il2CppObject*>* objects;
        const MethodInfo* gameObject_FindGameObjectsWithTagMethod = helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine", "GameObject"), "FindGameObjectsWithTag", 1);
        helper->RunMethod(&objects, nullptr, gameObject_FindGameObjectsWithTagMethod, helper->createcsstr(name));
        for(int i = 0;i<objects->Length();i++){
            componentsFound.push_back(objects->values[i]);
        }
        return componentsFound;
    }

    Il2CppObject* FindGameObjectWithTag(IL2CPP_Helper* helper, char* name){
        std::vector<Il2CppObject*> componentsFound = FindGameObjectsWithTag(helper, name);
        if(componentsFound.size() > 0){
            return componentsFound[0];
        }
        return nullptr;
    } 

    Il2CppObject* FindGameObject(IL2CPP_Helper* helper, char* name){
        Il2CppObject* object;
        const MethodInfo* gameObject_FindMethod = helper->class_get_method_from_name(helper->GetClassFromName("UnityEngine", "GameObject"), "Find", 1);
        helper->RunMethod(&object, nullptr, gameObject_FindMethod, helper->createcsstr(name));
        return object;
    }
  
    std::vector<Il2CppObject*> GetComponentsInParent(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound;
        Array<Il2CppObject*>* objects;
        helper->RunMethod(&objects, parentObject, "GetComponentsInParent", helper->type_get_object(helper->class_get_type(klass)), &boolTrue);
        for(int i = 0;i<objects->Length();i++){
            Il2CppObject* object = objects->values[i];
            if(object != nullptr){
                Il2CppString* nameObject;
                helper->RunMethod(&nameObject, object, "get_name");
                if (strcmp(to_utf8(csstrtostr(nameObject)).c_str(), name) == 0)
                {
                    componentsFound.push_back(object);
                }
            }
        }
        return componentsFound;
    }

    Il2CppObject* GetComponentInParent(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound = GetComponentsInParent(helper, parentObject, klass, name);
        if(componentsFound.size() > 0){
            return componentsFound[0];
        }
        return nullptr;
    }

    std::vector<Il2CppObject*> GetComponentsInChildren(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound;
        Array<Il2CppObject*>* objects;
        helper->RunMethod(&objects, parentObject, "GetComponentsInChildren", helper->type_get_object(helper->class_get_type(klass)), &boolTrue);
        for(int i = 0;i<objects->Length();i++){
            Il2CppObject* object = objects->values[i];
            if(object != nullptr){
                Il2CppString* nameObject;
                helper->RunMethod(&nameObject, object, "get_name");
                if (strcmp(to_utf8(csstrtostr(nameObject)).c_str(), name) == 0)
                {
                    componentsFound.push_back(object);
                }
            }
        }
        return componentsFound;
    }

    Il2CppObject* GetComponentInChildren(IL2CPP_Helper* helper, Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound = GetComponentsInChildren(helper, parentObject, klass, name);
        if(componentsFound.size() > 0){
            return componentsFound[0];
        }
        return nullptr;
    }

    Il2CppObject* GetGameObject(IL2CPP_Helper* helper, Il2CppObject* object){
        Il2CppObject* gameObject = nullptr;
        if(object != nullptr){
            helper->RunMethod(&gameObject, object, "get_gameObject");
        }
        return gameObject;
    }

    void DontDestroyOnLoad(IL2CPP_Helper* helper, Il2CppObject* object){
        helper->RunStaticMethod(object, "DontDestroyOnLoad", object);
    }
   
    void SetActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active){
        helper->RunMethod(object, "SetActive", &active);
    }

    void SetGameObjectActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active){
        Il2CppObject* gameObject = GetGameObject(helper, object);
        if(gameObject != nullptr)
           SetActive(helper, gameObject, active);
    }

    void SetParent(IL2CPP_Helper* helper, Il2CppObject* object, Il2CppObject* parent){
        Il2CppObject* parentTransform;
        helper->RunMethod(&parentTransform, parent, "get_transform");
        Il2CppObject* objectTransform;
        helper->RunMethod(&objectTransform, object, "get_transform");
        helper->RunMethod(objectTransform, "SetParent", parentTransform, &boolFalse);
    }
    void SetSameParent(IL2CPP_Helper* helper, Il2CppObject* object, Il2CppObject* parent){
        Il2CppObject* parentTransform;
        helper->RunMethod(&parentTransform, parent, "get_transform");
        helper->RunMethod(&parentTransform, parentTransform, "GetParent");
        Il2CppObject* objectTransform;
        helper->RunMethod(&objectTransform, object, "get_transform");
        helper->RunMethod(objectTransform, "SetParent", parentTransform, &boolFalse);
    }
    
    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* customUIObject, char* name, ButtonOnClickFunction* handler){
        std::vector<Il2CppObject*> customUIButtons = GetComponentsInChildren(helper, customUIObject, helper->GetClassFromName("UnityEngine.UI", "Button"), name);
        for(Il2CppObject* customUIButton : customUIButtons){
            auto action = helper->MakeAction(customUIButton, handler, helper->class_get_type(helper->GetClassFromName("System", "Action")));
            helper->RunMethod(buttonBinder, "AddBinding", customUIButton, action);
        }
    }

    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* button, ButtonOnClickFunction* handler){
        auto action = helper->MakeAction(button, handler, helper->class_get_type(helper->GetClassFromName("System", "Action")));
        helper->RunMethod(buttonBinder, "AddBinding", button, action);
    }
        
    void SetButtonText(IL2CPP_Helper* helper, Il2CppObject* button, char* text){
        Il2CppObject* buttonTextObject;
        helper->RunMethod(&buttonTextObject, button, "GetComponentInChildren", helper->type_get_object(helper->class_get_type(helper->GetClassFromName("TMPro", "TextMeshProUGUI"))), &boolTrue);
        helper->RunMethod(buttonTextObject, "set_text", helper->createcsstr(text));
    }

    void SetButtonTextColor(IL2CPP_Helper* helper, Il2CppObject* button, Color color){
        Il2CppObject* buttonTextObject;
        helper->RunMethod(&buttonTextObject, button, "GetComponentInChildren", helper->type_get_object(helper->class_get_type(helper->GetClassFromName("TMPro", "TextMeshProUGUI"))), &boolTrue);
        helper->RunMethod(buttonTextObject, "set_color", &color);
    } 

    void SetButtonEnabled(IL2CPP_Helper* helper, Il2CppObject* button, bool enabled){
        helper->RunMethod(button, "set_interactable", &enabled);
    }

    bool GetToggleIsOn(IL2CPP_Helper* helper, Il2CppObject* toggle)
    {
        if(toggle == nullptr)
            return false;
        bool isOn;
        helper->RunMethod(&isOn, toggle, "get_isOn");
        return isOn;
    }

    void SetToggleIsOn(IL2CPP_Helper* helper, Il2CppObject* toggle, bool isOn){
        if(toggle == nullptr)
            return;
        helper->RunMethod(toggle, "set_isOn", &isOn);
    }

}