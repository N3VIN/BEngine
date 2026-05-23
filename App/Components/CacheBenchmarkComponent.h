#pragma once
#include "Components/Component.h"
#include <vector>

namespace app {
    struct TransformAlt {
        float matrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    };

    struct GameObject3D {
        TransformAlt transform;
        int id;
    };

    struct GameObject3DAlt {
        TransformAlt *transform;
        int id;
    };

    class CacheBenchmarkComponent final : public bengine::Component {
    public:
        explicit CacheBenchmarkComponent(bengine::GameObject *owner);

        void Update(float deltaTime) override;
        void Render() const override;

    private:
        struct BenchmarkResult {
            int stepSize;
            float avgTimeMs;
        };

        template<typename T, typename ModifyFunc>
        std::vector<BenchmarkResult> RunBenchmark(int samples, ModifyFunc modify);

        struct BenchmarkState {
            std::vector<BenchmarkResult> results;
            std::vector<BenchmarkResult> resultsAlt; // only for GameObject3DAlt
            std::vector<float> timings;
            std::vector<float> timingsAlt;
            std::vector<float> stepLabels;
            int samples = 10;
            bool done = false;
            bool requested = false;
        };

        mutable BenchmarkState m_gameObjectBenchmark;
        mutable BenchmarkState m_intBenchmark;
    };
} // app
