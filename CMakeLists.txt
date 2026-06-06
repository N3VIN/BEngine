cmake_minimum_required(VERSION 3.26)
project(Bomberman)

if (EMSCRIPTEN)
    message("Using Emscripten!")
endif ()

if (EMSCRIPTEN)
    set(TARGET_NAME bomberman_web)
    set(TARGET_PARAM)
    set(GAME_TARGETS ${TARGET_NAME})
else ()
    set(TARGET_NAME demo)
    set(TARGET_PARAM WIN32)
    set(GAME_TARGETS demo bomberman)
endif ()

# core static library (engine only)
add_library(Core STATIC
        Core/Renderer/Font.cpp
        Core/Input/Gamepad.cpp
        Core/SceneGraph/GameObject.cpp
        Core/Input/InputManager.cpp
        Core/Engine/Engine.cpp
        Core/Renderer/Renderer.cpp
        Core/Renderer/ResourceManager.cpp
        Core/SceneGraph/Scene.cpp
        Core/SceneGraph/SceneManager.cpp
        Core/Renderer/Texture2D.cpp
        Core/SceneGraph/Transform.cpp
        Core/Components/RenderComponent.cpp
        Core/Components/TextComponent.cpp
        Core/Components/FPSComponent.cpp
        Core/Components/RotatorComponent.cpp
        Core/Components/CameraComponent.cpp
        Core/Audio/SDLAudioService.cpp
)

# demo executable (App)
add_executable(${TARGET_NAME} ${TARGET_PARAM}
        App/Main.cpp
        App/Components/CacheBenchmarkComponent.cpp
        App/Components/HealthComponent.cpp
        App/Components/LivesDisplayComponent.cpp
        App/Components/PickupComponent.cpp
        App/Components/ScoreDisplayComponent.cpp
        App/Components/SteamAchievementComponent.cpp
)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/App)

# native only
if (NOT EMSCRIPTEN)
    add_executable(bomberman ${TARGET_PARAM}
            Bomberman/Main.cpp
            Bomberman/Level/BuildLevelScene.cpp
            Bomberman/Level/Tileset.cpp
            Bomberman/Components/EnemyManagerComponent.cpp
            Bomberman/Components/EnemyAIComponent.cpp
            Bomberman/Components/EnemyControllerComponent.cpp
            Bomberman/States/EnemyStates.cpp
            Bomberman/Components/LevelGridComponent.cpp
            Bomberman/Components/GridMovementComponent.cpp
            Bomberman/Components/GridRenderComponent.cpp
            Bomberman/Components/SpriteRendererComponent.cpp
            Bomberman/Components/HealthComponent.cpp
            Bomberman/Components/PlayerControllerComponent.cpp
            Bomberman/Components/BombComponent.cpp
            Bomberman/Components/BombManagerComponent.cpp
            Bomberman/Components/HazardComponent.cpp
            Bomberman/Components/BrickComponent.cpp
            Bomberman/Components/ExplosionComponent.cpp
            Bomberman/States/PlayerStates.cpp
            Bomberman/SceneStates/MainMenuState.cpp
            Bomberman/SceneStates/GameSceneState.cpp
            Bomberman/SceneStates/GameEndState.cpp
    )
    target_include_directories(bomberman PRIVATE ${CMAKE_SOURCE_DIR}/Bomberman)
endif ()

# enable c++23 features
target_compile_features(Core PUBLIC cxx_std_23)

# precompiled header
target_precompile_headers(Core PUBLIC ${CMAKE_SOURCE_DIR}/Core/pch.h)

include(FetchContent)
# add glm
find_package(glm CONFIG QUIET)
if (NOT glm_FOUND)
    FetchContent_Declare(
            glm
            URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(glm)
endif ()

# Add SDL3
find_package(SDL3 3.4 CONFIG QUIET)
if (NOT SDL3_FOUND)
    if (WIN32)
        FetchContent_Declare(
                SDL3
                URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
                DOWNLOAD_NO_PROGRESS ON
                DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
        )
        FetchContent_MakeAvailable(SDL3)
        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
        find_package(SDL3 CONFIG REQUIRED)
    else ()
        FetchContent_Declare(
                SDL3
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG release-3.4.0
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(SDL3)
    endif ()
endif ()

# Add SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if (NOT SDL3_ttf_FOUND)
    if (WIN32)
        FetchContent_Declare(
                SDL3_ttf
                URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
                DOWNLOAD_NO_PROGRESS ON
                DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
        )
        FetchContent_MakeAvailable(SDL3_ttf)
        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
        find_package(SDL3_ttf CONFIG REQUIRED)
    else ()
        FetchContent_Declare(
                SDL3_ttf
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
                GIT_TAG release-3.2.2
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        set(SDL3TTF_INSTALL OFF)
        if (EMSCRIPTEN)
            list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten")
            set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
            set(SDL3TTF_VENDORED OFF)
            set(SDLTTF_PLUTOSVG OFF)
            add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
            add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
        endif ()
        FetchContent_MakeAvailable(SDL3_ttf)
    endif ()
endif ()

# Add SDL3_mixer
find_package(SDL3_mixer 3.2 CONFIG QUIET)
if (NOT SDL3_mixer_FOUND)
    if (WIN32)
        FetchContent_Declare(
                SDL3_mixer
                URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-3.2.0/SDL3_mixer-devel-3.2.0-VC.zip
                DOWNLOAD_NO_PROGRESS ON
                DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
        )
        FetchContent_MakeAvailable(SDL3_mixer)
        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_mixer_SOURCE_DIR}")
        find_package(SDL3_mixer CONFIG REQUIRED)
    else ()
        FetchContent_Declare(
                SDL3_mixer
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
                GIT_TAG release-3.2.0
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        set(SDLMIXER_INSTALL OFF)
        set(SDLMIXER_VENDORED ON)
        if (EMSCRIPTEN)
            set(SDLMIXER_MP3_MPG123 OFF)
            set(SDLMIXER_FLAC_LIBFLAC OFF)
            set(SDLMIXER_MOD OFF)
            set(SDLMIXER_MIDI OFF)
            set(SDLMIXER_WAVPACK OFF)
            set(SDLMIXER_GME OFF)
        endif ()
        FetchContent_MakeAvailable(SDL3_mixer)
    endif ()
endif ()

# Add nlohmann
FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_MakeAvailable(nlohmann_json)

# Add Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.92.5-docking
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${sdl3_SOURCE_DIR}/include)
target_link_libraries(imgui PUBLIC SDL3::SDL3)
target_compile_features(imgui PUBLIC cxx_std_23)

# Add imgui-plot
set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR})
FetchContent_Declare(
        imgui_plot
        GIT_REPOSITORY https://github.com/soulthreads/imgui-plot.git
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui_plot)

if (WIN32)
    # Add VLD (Visual Leak Detector) for memory leak detection on Windows
    find_package(VLD CONFIG)
endif ()

# Add steam if available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")
find_package(Steamworks QUIET)

if (Steamworks_FOUND)
    target_link_libraries(Core PUBLIC Steamworks::Steamworks)
    target_compile_definitions(Core PUBLIC USE_STEAMWORKS)
endif ()

# Core library configuration
target_include_directories(Core PUBLIC
        ${CMAKE_SOURCE_DIR}/Core/
        ${imgui_SOURCE_DIR}
        ${VLD_INCLUDE_DIR}
)

target_link_libraries(Core PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        glm::glm
        imgui
        imgui_plot
        nlohmann_json::nlohmann_json
        ${VLD_LIBRARIES}
)

if (WIN32 AND NOT EMSCRIPTEN)
    target_link_libraries(Core PRIVATE xinput)
endif ()

# Executable configuration (applied to every game target)
if (MSVC)
    target_compile_options(Core PRIVATE /W4 /WX)
else ()
    target_compile_options(Core PRIVATE -Wall -Wextra -Werror)
endif ()

foreach (game_target IN LISTS GAME_TARGETS)
    target_link_libraries(${game_target} PRIVATE Core)
    if (MSVC)
        target_compile_options(${game_target} PRIVATE /W4 /WX)
    else ()
        target_compile_options(${game_target} PRIVATE -Wall -Wextra -Werror)
    endif ()
endforeach ()

if (EMSCRIPTEN)
    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\"")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)

    # Additional settings for Emscripten build
    set_target_properties(${TARGET_NAME} PROPERTIES
            COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
            LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif (WIN32)
    # Set the demo as the default startup project (first in GAME_TARGETS)
    list(GET GAME_TARGETS 0 _startup_target)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${_startup_target})

    # Copy SDL dlls to output folder
    set(SDL_RUNTIME_DLLS
            SDL3::SDL3
            SDL3_ttf::SDL3_ttf
            SDL3_mixer::SDL3_mixer
    )

    foreach (game_target IN LISTS GAME_TARGETS)
        set_target_properties(${game_target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${game_target}>")

        foreach (dllTarget IN LISTS SDL_RUNTIME_DLLS)
            add_custom_command(
                    TARGET ${game_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:${dllTarget}>"
                    "$<TARGET_FILE_DIR:${game_target}>"
                    VERBATIM
            )
        endforeach ()

        if (Steamworks_FOUND)
            target_link_libraries(${game_target} PUBLIC Steamworks::Steamworks)
            target_compile_definitions(${game_target} PRIVATE USE_STEAMWORKS)
            add_custom_command(TARGET ${game_target} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${game_target}>")
            add_custom_command(TARGET ${game_target} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${game_target}>/steam_appid.txt"
                    COMMENT "Generating steam_appid.txt"
            )
        endif ()

        add_custom_command(TARGET ${game_target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${game_target}>/Data"
        )
    endforeach ()
elseif (APPLE)
    foreach (game_target IN LISTS GAME_TARGETS)
        set_target_properties(${game_target} PROPERTIES
                MACOSX_BUNDLE TRUE
                INSTALL_RPATH "@executable_path/../Frameworks"
                BUILD_WITH_INSTALL_RPATH TRUE
        )
        add_custom_command(TARGET ${game_target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_BUNDLE_DIR:${game_target}>/Contents/Frameworks"
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_BUNDLE_DIR:${game_target}>/Contents/Frameworks/"
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared>" "$<TARGET_BUNDLE_DIR:${game_target}>/Contents/Frameworks/"
                COMMAND install_name_tool -change "@rpath/libSDL3_ttf.0.dylib" "@executable_path/../Frameworks/libSDL3_ttf.0.2.2.dylib" "$<TARGET_FILE:${game_target}>"
                COMMAND install_name_tool -change "@rpath/libSDL3.0.dylib" "@executable_path/../Frameworks/libSDL3.0.dylib" "$<TARGET_FILE:${game_target}>"
        )

        if (Steamworks_FOUND)
            target_link_libraries(${game_target} PUBLIC Steamworks::Steamworks)
            target_compile_definitions(${game_target} PRIVATE USE_STEAMWORKS)
            add_custom_command(TARGET ${game_target} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_BUNDLE_DIR:${game_target}>/Contents/Frameworks/"
            )
            add_custom_command(TARGET ${game_target} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${game_target}>/steam_appid.txt"
                    COMMENT "Generating steam_appid.txt"
            )
        endif ()

        add_custom_command(TARGET ${game_target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${game_target}>/Data"
        )
    endforeach ()
else ()
    foreach (game_target IN LISTS GAME_TARGETS)
        if (Steamworks_FOUND)
            target_link_libraries(${game_target} PUBLIC Steamworks::Steamworks)
            target_compile_definitions(${game_target} PRIVATE USE_STEAMWORKS)
            add_custom_command(TARGET ${game_target} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_FILE_DIR:${game_target}>"
            )
            add_custom_command(TARGET ${game_target} POST_BUILD
                    COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${game_target}>/steam_appid.txt"
                    COMMENT "Generating steam_appid.txt"
            )
        endif ()

        add_custom_command(TARGET ${game_target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${game_target}>/Data"
        )
    endforeach ()
endif ()
