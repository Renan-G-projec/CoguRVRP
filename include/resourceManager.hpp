#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "raylib.h"
#include <map>
#include <string>


class ResourceManager {
private:
    std::map<std::string, Texture2D> textures;

public:
    Texture2D getTexture(const std::string& path);
    void unloadAll();
};

#endif