#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Patterns/Singleton.h"


namespace fs = std::filesystem;

namespace dae {
    class Texture2D;
    class Font;

    class ResourceManager final : public Singleton<ResourceManager> {
    public:
        void Init(const fs::path &data);

        [[nodiscard]] const fs::path &GetDataPath() const {
            return m_dataPath;
        }

        std::shared_ptr<Texture2D> LoadTexture(const std::string_view &file);
        std::shared_ptr<Font> LoadFont(const std::string_view &file, uint8_t size);

    private:
        friend class Singleton<ResourceManager>;
        ResourceManager() = default;
        fs::path m_dataPath;

        void UnloadUnusedResources();

        std::map<std::string, std::shared_ptr<Texture2D> > m_loadedTextures;
        std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font> > m_loadedFonts;
    };
}
