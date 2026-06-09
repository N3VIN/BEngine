#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "PickupType.h"

namespace bengine {
    class GameObject;
}

namespace bomberman {
    struct SpriteDefinition;

    // wanted to use OOP for a change
    class Pickup {
    public:
        explicit Pickup(glm::ivec2 cell);
        virtual ~Pickup() = default;

        [[nodiscard]] glm::ivec2 GetCell() const;
        [[nodiscard]] bool IsRevealed() const;
        [[nodiscard]] bengine::GameObject *GetObject() const;
        void MarkRevealed(bengine::GameObject *object);

        [[nodiscard]] virtual const SpriteDefinition &GetSprite() const = 0;
        [[nodiscard]] virtual PickupType GetType() const = 0;

    private:
        glm::ivec2 m_cell;
        bool m_revealed{false};
        bengine::GameObject *m_object{nullptr};
    };

    class BombUpPickup final : public Pickup {
    public:
        using Pickup::Pickup;
        [[nodiscard]] const SpriteDefinition &GetSprite() const override;
        [[nodiscard]] PickupType GetType() const override;
    };

    class FlameUpPickup final : public Pickup {
    public:
        using Pickup::Pickup;
        [[nodiscard]] const SpriteDefinition &GetSprite() const override;
        [[nodiscard]] PickupType GetType() const override;
    };

    class DetonatorPickup final : public Pickup {
    public:
        using Pickup::Pickup;
        [[nodiscard]] const SpriteDefinition &GetSprite() const override;
        [[nodiscard]] PickupType GetType() const override;
    };

    [[nodiscard]] std::unique_ptr<Pickup> MakePickup(PickupType type, glm::ivec2 cell);
}
