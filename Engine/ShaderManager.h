#include "Shader.h"
#include <map>
#include <string>

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    // Ajouter un nouveau shader et retourner une r�f�rence vers celui-ci
    Shader& AddShader(const wstring& shaderName, const wstring& shaderFile, const string& entryPoint, const string& target);

    // Obtenir un shader par son nom
    Shader* GetShader(const wstring& shaderName);

    // Supprimer un shader par son nom
    void RemoveShader(const wstring& shaderName);

private:
    map<wstring, unique_ptr<Shader>> mShaders;
};