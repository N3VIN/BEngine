#include "Renderer/ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"


void bengine::ResourceManager::Init(const fs::path &dataPath) {
    m_dataPath = dataPath;

    if (!TTF_Init()) {
        throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
    }
}

std::shared_ptr<bengine::Texture2D> bengine::ResourceManager::LoadTexture(const std::string_view &file) {
    const auto fullPath = m_dataPath / file;
    const auto filename = fs::path(fullPath).filename().string();

    if (m_loadedTextures.find(filename) == m_loadedTextures.end()) {
        m_loadedTextures.insert(std::pair(filename, std::make_shared<Texture2D>(fullPath.string())));
    }

    return m_loadedTextures.at(filename);
}

std::shared_ptr<bengine::Font> bengine::ResourceManager::LoadFont(const std::string_view &file, uint8_t size) {
    const auto fullPath = m_dataPath / file;
    const auto filename = fs::path(fullPath).filename().string();
    const auto key = std::pair<std::string, uint8_t>(filename, size);

    if (m_loadedFonts.find(key) == m_loadedFonts.end()) {
        m_loadedFonts.insert(std::pair(key, std::make_shared<Font>(fullPath.string(), size)));
    }

    return m_loadedFonts.at(key);
}

void bengine::ResourceManager::UnloadUnusedResources() {
    for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();) {
        if (it->second.use_count() == 1) {
            it = m_loadedTextures.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();) {
        if (it->second.use_count() == 1) {
            it = m_loadedFonts.erase(it);
        }
        else {
            ++it;
        }
    }
}
