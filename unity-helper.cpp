#include "unity-helper.hpp"
#include <vector>

namespace UnityHelper {

    bool boolTrue = true;
    bool boolFalse = false;

    std::vector<Il2CppObject*> FindGameObjectsWithTag(char* name){
        std::vector<Il2CppObject*> componentsFound;
        Array<Il2CppObject*>* objects;
        objects = *CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(il2cpp_utils::GetClassFromName("UnityEngine", "GameObject"), "FindGameObjectsWithTag", il2cpp_utils::createcsstr(name)));
        for(int i = 0;i<objects->Length();i++){
            componentsFound.push_back(objects->values[i]);
        }
        return componentsFound;
    }

    Il2CppObject* FindGameObjectWithTag(char* name){
        std::vector<Il2CppObject*> componentsFound = FindGameObjectsWithTag(name);
        if(componentsFound.size() > 0){
            return componentsFound[0];
        }
        return nullptr;
    } 

    Il2CppObject* FindGameObject(char* name){
        Il2CppObject* object;
        il2cpp_utils::RunMethod(&object, il2cpp_utils::FindMethodUnsafe("UnityEngine", "GameObject", "Find", 1), il2cpp_utils::createcsstr(name));
        return object;
    }
  
    std::vector<Il2CppObject*> GetComponentsInParent(Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound;
        Array<Il2CppObject*>* objects;
        objects = *CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(parentObject, "GetComponentsInParent", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(klass)), boolTrue));
        for(int i = 0;i<objects->Length();i++){
            Il2CppObject* object = objects->values[i];
            if(object != nullptr){
                Il2CppString* nameObject;
                nameObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(object, "get_name"));
                if (strcmp(to_utf8(csstrtostr(nameObject)).c_str(), name) == 0)
                {
                    componentsFound.push_back(object);
                }
            }
        }
        return componentsFound;
    }

    Il2CppObject* GetComponentInParent(Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound = GetComponentsInParent(parentObject, klass, name);
        if(componentsFound.size() > 0){
            return componentsFound[0];
        }
        return nullptr;
    }

    std::vector<Il2CppObject*> GetComponentsInChildren(Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound;
        Array<Il2CppObject*>* objects;
        objects = *CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Il2CppObject*>*>(parentObject, "GetComponentsInChildren", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(klass)), boolTrue));
        for(int i = 0;i<objects->Length();i++){
            Il2CppObject* object = objects->values[i];
            if(object != nullptr){
                Il2CppString* nameObject;
                nameObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppString*>(object, "get_name"));
                if (strcmp(to_utf8(csstrtostr(nameObject)).c_str(), name) == 0)
                {
                    componentsFound.push_back(object);
                }
            }
        }
        return componentsFound;
    }

    Il2CppObject* GetComponentInChildren(Il2CppObject* parentObject, Il2CppClass* klass, char* name){
        std::vector<Il2CppObject*> componentsFound = GetComponentsInChildren(parentObject, klass, name);
        if(componentsFound.size() > 0){
            return componentsFound[0];
        }
        return nullptr;
    }

    Il2CppObject* GetGameObject(Il2CppObject* object){
        Il2CppObject* gameObject = nullptr;
        if(object != nullptr){
            gameObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(object, "get_gameObject"));
        }
        return gameObject;
    }

    void DontDestroyOnLoad(Il2CppObject* object){
        il2cpp_utils::RunMethod(il2cpp_utils::GetClassFromName("UnityEngine", "Object"), "DontDestroyOnLoad", object);
    }
   
    void SetActive(Il2CppObject* object, bool active){
        il2cpp_utils::RunMethod(object, "SetActive", active);
    }

    void SetGameObjectActive(Il2CppObject* object, bool active){
        Il2CppObject* gameObject = GetGameObject(object);
        if(gameObject != nullptr)
           SetActive(gameObject, active);
    }

    Il2CppObject* GetParent(Il2CppObject* object){
        Il2CppObject* parent = nullptr;
        if(object != nullptr)
            parent = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(object, "get_transform"));
        if(parent != nullptr)
            parent = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(parent, "GetParent"));
        return parent;
    }

    void SetParent(Il2CppObject* object, Il2CppObject* parent){
        Il2CppObject* parentTransform;
        parentTransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(parent, "get_transform"));
        Il2CppObject* objectTransform;
        objectTransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(object, "get_transform"));
        il2cpp_utils::RunMethod(objectTransform, "SetParent", parentTransform, boolFalse);
    }

    void SetSameParent(Il2CppObject* object, Il2CppObject* objectTwo){
        Il2CppObject* parent = GetParent(objectTwo);
        Il2CppObject* objectTransform;
        objectTransform = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(object, "get_transform"));
        il2cpp_utils::RunMethod(objectTransform, "SetParent", parent, boolFalse);
    }
    
    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* customUIObject, char* name, ButtonOnClickFunction* handler){
        std::vector<Il2CppObject*> customUIButtons = GetComponentsInChildren(customUIObject, il2cpp_utils::GetClassFromName("UnityEngine.UI", "Button"), name);
        for(Il2CppObject* customUIButton : customUIButtons){
            auto action = il2cpp_utils::MakeAction(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("System", "Action")), customUIButton, handler); //RS
            il2cpp_utils::RunMethod(buttonBinder, "AddBinding", customUIButton, action); 
        }
    }

    void AddButtonOnClick(Il2CppObject* buttonBinder, Il2CppObject* button, ButtonOnClickFunction* handler){
        auto action = il2cpp_utils::MakeAction(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("System", "Action")),button, handler); //RS
        il2cpp_utils::RunMethod(buttonBinder, "AddBinding", button, action); 
    }
        
    void SetButtonText(Il2CppObject* button, std::string text){
        Il2CppObject* buttonTextObject;
        buttonTextObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(button, "GetComponentInChildren", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("TMPro", "TextMeshProUGUI"))), boolTrue));
        il2cpp_utils::RunMethod(buttonTextObject, "set_text", il2cpp_utils::createcsstr(text));
    }

    void SetButtonTextColor(Il2CppObject* button, Color color){
        Il2CppObject* buttonTextObject;
        buttonTextObject = *CRASH_UNLESS(il2cpp_utils::RunMethod<Il2CppObject*>(button, "GetComponentInChildren", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("TMPro", "TextMeshProUGUI"))), boolTrue));
        il2cpp_utils::RunMethod(buttonTextObject, "set_color", color);
    } 

    void SetButtonEnabled(Il2CppObject* button, bool enabled){
        il2cpp_utils::RunMethod(button, "set_interactable", enabled);
    }

    bool GetToggleIsOn(Il2CppObject* toggle)
    {
        if(toggle == nullptr)
            return false;
        bool isOn;
        isOn = *CRASH_UNLESS(il2cpp_utils::RunMethod<bool>(toggle, "get_isOn"));
        return isOn;
    }

    void SetToggleIsOn(Il2CppObject* toggle, bool isOn){
        if(toggle == nullptr)
            return;
        il2cpp_utils::RunMethod(toggle, "set_isOn", isOn);
    }

}