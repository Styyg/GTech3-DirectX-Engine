#include "ShaderManager.h"

ShaderManager::ShaderManager() {}

ShaderManager::~ShaderManager() {
    mShaders.clear();
}

Shader& ShaderManager::AddShader(const wstring& shaderName, const wstring& shaderFile, const string& entryPoint, const string& target) {
    auto shader = make_unique<Shader>(shaderFile, entryPoint, target);
    auto& ref = *shader;
    mShaders[shaderName] = move(shader);
    return ref;
}

Shader* ShaderManager::GetShader(const wstring& shaderName) {
    auto it = mShaders.find(shaderName);
    if (it != mShaders.end()) {
        return it->second.get();
    }
    return nullptr;
}

void ShaderManager::RemoveShader(const wstring& shaderName) {
    mShaders.erase(shaderName);
}
