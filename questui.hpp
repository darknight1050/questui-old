#include "../beatsaber-hook/shared/utils/typedefs.h"
#include "unity-helper.hpp"

namespace QuestUI {

    typedef void VoidFunction(); 

    struct QuestUIModInfo {
        std::string Name;
        VoidFunction* OnInitialized;
        Il2CppObject* Button;
        Il2CppObject* Panel;
    };

    struct QuestUIInfo {
        Il2CppObject* CustomUIObject;
        Il2CppObject* ButtonBinder;
        std::vector<QuestUIModInfo*> Mods;
    };

    void Initialize(std::string_view modNameArg, VoidFunction* onInitializedArg);

    QuestUIModInfo* GetQuestUIModInfo();
    QuestUIInfo* GetQuestUIInfo();
}
