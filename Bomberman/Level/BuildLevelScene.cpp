#include "BuildLevelScene.h"
#include "Tileset.h"
#include "UIHelpers.h"
#include "Components/LevelGridComponent.h"
#include "Components/GridRenderComponent.h"
#include "Components/BombManagerComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/BrickComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/CameraComponent.h"
#include "Commands/GridMoveCommand.h"
#include "Commands/BombPlaceCommand.h"
#include "PlayerFactory.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Renderer/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"

namespace dae {
    LevelGridComponent *CreateLevelBackground(Scene &scene, const Tileset &tileset, LevelGridComponent *gridComponent) {
        const auto levelOrigin = gridComponent->GetOrigin();
        const int tileSize = tileset.tileSize;

        auto levelGameObject = std::make_unique<GameObject>();
        levelGameObject->SetLocalPosition(levelOrigin);

        const SDL_Rect backgroundSourceRect{
            tileset.backgroundCoord.x, tileset.backgroundCoord.y,
            gridComponent->GetColumns() * tileSize,
            gridComponent->GetRows() * tileSize
        };

        levelGameObject->AddComponent<GridRenderComponent>(tileset.texturePath, backgroundSourceRect, tileset.spriteScale);
        scene.Add(std::move(levelGameObject));

        return gridComponent;
    }

    CameraComponent *CreateCamera(Scene &scene, LevelGridComponent *gridComponent) {
        auto cameraGo = std::make_unique<GameObject>();
        auto *cameraComp = cameraGo->AddComponent<CameraComponent>();
        cameraComp->SetZoom(2.0f);
        cameraComp->SetLevelBounds(
            gridComponent->GetOrigin(),
            glm::vec2(gridComponent->GetColumns(), gridComponent->GetRows()) * gridComponent->GetCellSize()
        );

        scene.Add(std::move(cameraGo));

        Renderer::GetInstance().SetActiveCamera(cameraComp);
        return cameraComp;
    }

    std::array<glm::ivec2, 2> SpawnLevelTiles(Scene &scene, LevelGridComponent *gridComponent) {
        std::array<glm::ivec2, 2> playerSpawnPositions{};

        const auto spawnTile = [&scene, gridComponent](glm::ivec2 cell, SpriteType spriteType) {
            auto tile = std::make_unique<GameObject>();
            tile->SetLocalPosition(gridComponent->CellToWorld(cell));
            tile->AddComponent<SpriteRendererComponent>(spriteType);
            auto *pTile = tile.get();
            scene.Add(std::move(tile));

            return pTile;
        };

        const glm::ivec2 dims = gridComponent->GetDimensions();
        for (int column = 0; column < dims.x; ++column) {
            for (int row = 0; row < dims.y; ++row) {
                const glm::ivec2 cell{column, row};
                switch (gridComponent->GetTile(column, row)) {
                    case TileType::Wall:
                        gridComponent->SetWall(cell, true);
                        break;
                    case TileType::Brick: {
                        auto *tile = spawnTile(cell, SpriteType::Brick);
                        tile->AddComponent<BrickComponent>(&scene, 0.875f);
                        gridComponent->SetWall(cell, true);
                        gridComponent->SetTile(tile, cell);
                        break;
                    }
                    case TileType::Exit: {
                        auto *tile = spawnTile(cell, SpriteType::Exit);
                        gridComponent->SetTile(tile, cell);
                        break;
                    }
                    case TileType::Player1Spawn:
                        playerSpawnPositions[0] = cell;
                        break;
                    case TileType::Player2Spawn:
                        playerSpawnPositions[1] = cell;
                        break;
                    case TileType::Empty:
                        break;
                }
            }
        }

        return playerSpawnPositions;
    }

    void CreateFPSDisplay(Scene &scene) {
        auto fpsGo = std::make_unique<GameObject>();
        auto *fpsText = fpsGo->AddComponent<TextComponent>();
        fpsGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);
        fpsText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
        fpsText->SetColor({0, 255, 0, 255});
        fpsGo->AddComponent<FPSComponent>();
        fpsGo->SetLocalPosition(ScreenFraction(0.01f, 0.01f));
        scene.Add(std::move(fpsGo));
    }

    void SetupPlayerInput(InputManager &input, GameObject *p1, GameObject *p2, BombManagerComponent *bombManager) {
        input.BindCommand(SDL_SCANCODE_W, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{0, -1}));
        input.BindCommand(SDL_SCANCODE_S, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{0, 1}));
        input.BindCommand(SDL_SCANCODE_A, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{-1, 0}));
        input.BindCommand(SDL_SCANCODE_D, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{1, 0}));

        input.BindCommand(0, Gamepad::Button::DpadUp, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{0, -1}));
        input.BindCommand(0, Gamepad::Button::DpadDown, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{0, 1}));
        input.BindCommand(0, Gamepad::Button::DpadLeft, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{-1, 0}));
        input.BindCommand(0, Gamepad::Button::DpadRight, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{1, 0}));

        input.BindCommand(SDL_SCANCODE_SPACE, KeyState::Down, std::make_unique<BombPlaceCommand>(p1, bombManager));
        input.BindCommand(0, Gamepad::Button::A, KeyState::Down, std::make_unique<BombPlaceCommand>(p2, bombManager));
    }

    Scene &BuildLevelScene(std::string_view jsonRelativePath) {
        const auto &tileset = GetTileset();

        auto &scene = SceneManager::GetInstance().CreateScene();
        auto levelGameObject = std::make_unique<GameObject>();
        auto *levelGridComponent = levelGameObject->AddComponent<LevelGridComponent>(ResourceManager::GetInstance().GetDataPath() / jsonRelativePath);
        scene.Add(std::move(levelGameObject));

        CreateLevelBackground(scene, tileset, levelGridComponent);

        auto *cameraComponent = CreateCamera(scene, levelGridComponent);

        auto playerSpawnPositions = SpawnLevelTiles(scene, levelGridComponent);

        auto bombManagerGO = std::make_unique<GameObject>();
        auto *bombManager = bombManagerGO->AddComponent<BombManagerComponent>(&scene, levelGridComponent);
        scene.Add(std::move(bombManagerGO));

        auto *p1 = CreatePlayer(scene, {levelGridComponent, playerSpawnPositions[0], "bomberman.png", 4.0f, tileset.spriteScale});
        cameraComponent->SetTarget(p1);

        GameObject *p2 = CreatePlayer(scene, {levelGridComponent, playerSpawnPositions[1], "bomberman.png", 4.0f, tileset.spriteScale});

        bombManager->RegisterPlayer(p1);
        bombManager->RegisterPlayer(p2);

        CreateFPSDisplay(scene);

        SetupPlayerInput(InputManager::GetInstance(), p1, p2, bombManager);

        return scene;
    }
}
