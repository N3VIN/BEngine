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

bomberman::LevelGridComponent *bomberman::CreateLevelBackground(bengine::Scene &scene, const Tileset &tileset, LevelGridComponent *gridComponent) {
    const auto levelOrigin = gridComponent->GetOrigin();
    const int tileSize = tileset.tileSize;

    auto levelGameObject = std::make_unique<bengine::GameObject>();
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

bengine::CameraComponent *bomberman::CreateCamera(bengine::Scene &scene, LevelGridComponent *gridComponent) {
    auto cameraGo = std::make_unique<bengine::GameObject>();
    auto *cameraComp = cameraGo->AddComponent<bengine::CameraComponent>();
    cameraComp->SetZoom(2.0f);
    cameraComp->SetLevelBounds(
        gridComponent->GetOrigin(),
        glm::vec2(gridComponent->GetColumns(), gridComponent->GetRows()) * gridComponent->GetCellSize()
    );

    scene.Add(std::move(cameraGo));

    bengine::Renderer::GetInstance().SetActiveCamera(cameraComp);
    return cameraComp;
}

std::array<glm::ivec2, 2> bomberman::SpawnLevelTiles(bengine::Scene &scene, LevelGridComponent *gridComponent) {
    std::array<glm::ivec2, 2> playerSpawnPositions{};

    const auto spawnTile = [&scene, gridComponent](glm::ivec2 cell, SpriteType spriteType) {
        auto tile = std::make_unique<bengine::GameObject>();
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

void bomberman::CreateFPSDisplay(bengine::Scene &scene) {
    auto fpsGo = std::make_unique<bengine::GameObject>();
    auto *fpsText = fpsGo->AddComponent<bengine::TextComponent>();
    fpsGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
    fpsText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
    fpsText->SetColor({0, 255, 0, 255});
    fpsGo->AddComponent<bengine::FPSComponent>();
    fpsGo->SetLocalPosition(bengine::ScreenFraction(0.01f, 0.01f));
    scene.Add(std::move(fpsGo));
}

void bomberman::SetupPlayerInput(bengine::InputManager &input, bengine::GameObject *p1, bengine::GameObject *p2, BombManagerComponent *bombManager) {
    input.BindCommand(SDL_SCANCODE_W, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{0, -1}));
    input.BindCommand(SDL_SCANCODE_S, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{0, 1}));
    input.BindCommand(SDL_SCANCODE_A, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{-1, 0}));
    input.BindCommand(SDL_SCANCODE_D, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{1, 0}));

    input.BindCommand(0, bengine::Gamepad::Button::DpadUp, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{0, -1}));
    input.BindCommand(0, bengine::Gamepad::Button::DpadDown, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{0, 1}));
    input.BindCommand(0, bengine::Gamepad::Button::DpadLeft, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{-1, 0}));
    input.BindCommand(0, bengine::Gamepad::Button::DpadRight, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{1, 0}));

    input.BindCommand(SDL_SCANCODE_SPACE, bengine::KeyState::Down, std::make_unique<BombPlaceCommand>(p1, bombManager));
    input.BindCommand(0, bengine::Gamepad::Button::A, bengine::KeyState::Down, std::make_unique<BombPlaceCommand>(p2, bombManager));
}

bengine::Scene &bomberman::BuildLevelScene(std::string_view jsonRelativePath) {
    const auto &tileset = GetTileset();

    auto &scene = bengine::SceneManager::GetInstance().CreateScene();
    auto levelGameObject = std::make_unique<bengine::GameObject>();
    auto *levelGridComponent = levelGameObject->AddComponent<LevelGridComponent>(bengine::ResourceManager::GetInstance().GetDataPath() / jsonRelativePath);
    scene.Add(std::move(levelGameObject));

    CreateLevelBackground(scene, tileset, levelGridComponent);

    auto *cameraComponent = CreateCamera(scene, levelGridComponent);

    auto playerSpawnPositions = SpawnLevelTiles(scene, levelGridComponent);

    auto bombManagerGO = std::make_unique<bengine::GameObject>();
    auto *bombManager = bombManagerGO->AddComponent<BombManagerComponent>(&scene, levelGridComponent);
    scene.Add(std::move(bombManagerGO));

    auto *p1 = CreatePlayer(scene, {levelGridComponent, playerSpawnPositions[0], "bomberman.png", 4.0f, tileset.spriteScale});
    cameraComponent->SetTarget(p1);

    bengine::GameObject *p2 = CreatePlayer(scene, {levelGridComponent, playerSpawnPositions[1], "bomberman.png", 4.0f, tileset.spriteScale});

    bombManager->RegisterPlayer(p1);
    bombManager->RegisterPlayer(p2);

    CreateFPSDisplay(scene);

    SetupPlayerInput(bengine::InputManager::GetInstance(), p1, p2, bombManager);

    return scene;
}
