#include "unity-helper.hpp"
#include <vector>

namespace UnityHelper {

    bool boolTrue = true;
    bool boolFalse = false;

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
        Array<Il2CppObject*>* objects;
        helper->RunMethod(&objects, parentObject, "GetComponentsInParent", helper->type_get_object(helper->class_get_type(klass)), &boolTrue);
        for(int i = 0;i<objects->Length();i++){
            Il2CppObject* object = objects->values[i];
            if(object != nullptr){
                Il2CppString* nameObject;
                helper->RunMethod(&nameObject, object, "get_name");
                if (strcmp(to_utf8(csstrtostr(nameObject)).c_str(), name) == 0)
                {
                    return object;
                }
            }
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
        Array<Il2CppObject*>* objects;
        helper->RunMethod(&objects, parentObject, "GetComponentsInChildren", helper->type_get_object(helper->class_get_type(klass)), &boolTrue);
        for(int i = 0;i<objects->Length();i++){
            Il2CppObject* object = objects->values[i];
            if(object != nullptr){
                Il2CppString* nameObject;
                helper->RunMethod(&nameObject, object, "get_name");
                if (strcmp(to_utf8(csstrtostr(nameObject)).c_str(), name) == 0)
                {
                    return object;
                }
            }
        }
        return nullptr;
    }

    void SetActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active){
        helper->RunMethod(object, "SetActive", &active);
    }

    void SetGameObjectActive(IL2CPP_Helper* helper, Il2CppObject* object, bool active){
        Il2CppObject* gameObject;
        helper->RunMethod(&gameObject, object, "get_gameObject");
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
    
    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* customUIObject, char* name, void* handler){
        std::vector<Il2CppObject*> customUIButtons = GetComponentsInChildren(helper, customUIObject, helper->GetClassFromName("UnityEngine.UI", "Button"), name);
        for(Il2CppObject* customUIButton : customUIButtons){
            auto action = helper->MakeAction(customUIButton, handler, helper->class_get_type(helper->GetClassFromName("System", "Action")));
            helper->RunMethod(buttonBinder, "AddBinding", customUIButton, action);
        }
    }

    void AddButtonOnClick(IL2CPP_Helper* helper, Il2CppObject* buttonBinder, Il2CppObject* button, void* handler){
        auto action = helper->MakeAction(button, handler, helper->class_get_type(helper->GetClassFromName("System", "Action")));
        helper->RunMethod(buttonBinder, "AddBinding", button, action);
    }
        
    void SetButtonText(IL2CPP_Helper* helper, Il2CppObject* button, char* text){
        Il2CppObject* buttonTextObject;
        helper->RunMethod(&buttonTextObject, button, "GetComponentInChildren", helper->type_get_object(helper->class_get_type(helper->GetClassFromName("TMPro", "TextMeshProUGUI"))), &boolTrue);
        helper->RunMethod(buttonTextObject, "set_text", helper->createcsstr(text));
    }

    void SetButtonEnabled(IL2CPP_Helper* helper, Il2CppObject* button, bool enabled){
        helper->RunMethod(button, "set_interactable", &enabled);
    }
}