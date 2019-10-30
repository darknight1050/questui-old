#include "../beatsaber-hook/shared/utils/utils.h"

#include "unityasset-loader.hpp"
#include "unity-helper.hpp"

namespace QuestUI {

    struct QuestUIModInfo {
        char* Name;
        void* OnInitialized;
        Il2CppObject* Button;
        Il2CppObject* Panel;
    };

    struct QuestUIInfo {
        Il2CppObject* CustomUIObject;
        Il2CppObject* ButtonBinder;
        std::vector<QuestUIModInfo*>* Mods;
    };

    void Initialize(char* modNameArg, void* onInitializedArg);

    QuestUIModInfo GetQuestUIModInfo();
    QuestUIInfo* GetQuestUIInfo();
}