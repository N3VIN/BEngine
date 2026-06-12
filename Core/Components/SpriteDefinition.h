#pragma once

namespace bengine {
    struct SpriteDefinition {
        int col{};
        int row{};
        int numCols{1};
        int numRows{1};
        int frameCount{1};
        bool autoPlay{false};
        int frameColumns{0};
        int frameStrideCols{0};
        int frameStrideRows{0};
    };
}
