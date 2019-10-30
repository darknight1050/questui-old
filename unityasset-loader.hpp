#include "../beatsaber-hook/shared/utils/utils.h"

typedef void UnityAssetLoader_OnFinishFunction(Il2CppObject* asset);

namespace UnityAssetLoader {

    bool LoadFromFileAsync(char* filePath, UnityAssetLoader_OnFinishFunction* onFinish);

};