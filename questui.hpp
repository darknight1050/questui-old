#include "../beatsaber-hook/shared/utils/utils.h"
#include "../CustomSabers/AssetImporter.hpp"

#include "unityasset-loader.hpp"
#include "unity-helper.hpp"

namespace QuestUI {

    typedef void VoidFunction(); 

    struct QuestUIModInfo {
        char* Name;
        VoidFunction* OnInitialized;
        Il2CppObject* Button;
        Il2CppObject* Panel;
    };

    struct QuestUIInfo {
        Il2CppObject* CustomUIObject;
        Il2CppObject* ButtonBinder;
        std::vector<QuestUIModInfo*>* Mods;
    };

    void Initialize(char* modNameArg, VoidFunction* onInitializedArg);

    QuestUIModInfo GetQuestUIModInfo();
    QuestUIInfo* GetQuestUIInfo();
}