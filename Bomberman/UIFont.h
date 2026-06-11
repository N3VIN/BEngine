#pragma once
#include <memory>
#include <vector>

#include "Renderer/SpriteFont.h"

namespace bomberman {
    inline const std::shared_ptr<bengine::SpriteFont> &GetUIFont() {
        constexpr int glyphSize{8};
        static const auto font = std::make_shared<bengine::SpriteFont>(
            "Sprites/Title_Text.png", glyphSize, glyphSize,
            std::vector<bengine::SpriteFont::GlyphRow>{
                {"0123456789?", 0, 248},
                {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 256},
            });
        return font;
    }
}
