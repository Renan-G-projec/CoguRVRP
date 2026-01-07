#include "resourceManager.hpp"

Texture2D ResourceManager::getTexture(const std::string& path) {
    // Searches on RAM if the texture is loaded
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

    // If isn't, loads to RAM
    Texture2D texture = LoadTexture(path.c_str());

    // Registring to this instance
    textures[path] = texture;

    return texture;

}

void ResourceManager::unloadAll() {
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
}