#include "init.h"

// components
#include "Components/TestComponent.h"
#include "Components/MoveComponent.h"

// system
#include "Systems/TestSystem.h"
#include "Systems/CameraControllerSystem.h"
#include "Systems/MoveSystem.h"

#include <iostream>

using namespace minty;
using namespace minty::rendering;
using namespace game;

InputMap input;

// called when the engine is initialized
int init(Runtime &runtime)
{
    try
    {
        // components
        EntityRegistry::register_component<game::TestComponent>("Test");
        EntityRegistry::register_component<game::MoveComponent>("Move");

        // systems
        SystemRegistry::register_system<game::TestSystem>("Test");
        SystemRegistry::register_system<game::CameraControllerSystem>("CameraController");
        SystemRegistry::register_system<game::MoveSystem>("Move");

        Engine &engine = runtime.get_engine();
        Window &window = engine.get_window();
        RenderEngine &renderer = engine.get_render_engine();
        AudioEngine &audio = engine.get_audio_engine();
        SceneManager &sceneManager = engine.get_scene_manager();

        // create renderer
        Info info("TestProject", 0, 0, 0);

        // use defaults for most things
        RenderEngineBuilder rb(&info);

        // basic::create_basic_renderer_builder(rb);
        renderer.init(rb);

        // create textures
        // renderer.load_texture("Textures/oak_planks.png");
        // renderer.load_texture("Textures/pattern.png");
        // renderer.load_texture("Textures/texture.jpg");
        // renderer.load_texture("Textures/brian.png");
        // renderer.load_texture("Textures/x.png");
        // ID deathId = renderer.load_texture("Textures/death.png");
        // renderer.load_texture("Resources/Models/pumpkin_tex.jpg");

        // // create shaders
        // renderer.load_shader("Shaders/shader.minty");
        // renderer.load_shader("Shaders/spriteShader.minty");

        // // create shader passes
        // renderer.load_shader_pass("Shaders/shaderPass.minty");
        // renderer.load_shader_pass("Shaders/spriteShaderPass.minty");

        // // create material templates
        // renderer.load_material_template("Materials/materialTemplate.minty");
        // renderer.load_material_template("Materials/spriteMaterialTemplate.minty");

        // // create materials
        // renderer.load_material("Materials/material1.minty");
        // // renderer.load_material("Materials/material2.minty");
        // // renderer.load_material("Materials/material3.minty");
        // // renderer.load_material("Materials/pumpkinMat.minty");
        // renderer.load_material("Materials/spriteMaterial.minty");
        // renderer.load_material("Materials/xSpriteMaterial.minty");

        // // // create models
        // // renderer.load_mesh("Resources/Models/pumpkin.obj");

        // // creat sprites
        // renderer.load_sprite("Sprites/brian.minty");
        // ID xId = renderer.load_sprite("Sprites/x.minty");

        // TextureAtlasBuilder atlasBuilder{
        //     .textureId = deathId,
        //     .materialId = ERROR_ID,
        //     .coordinateMode = CoordinateMode::Pixel,
        //     .slice = Vector2(90.0f, 90.0f),
        //     .pivot = Vector2(45.0f, 45.0f)};

        // TextureAtlas atlas(atlasBuilder, renderer);

        // // create audio
        // audio.load_clip("Audio/blinking-forest.wav");
        // audio.load_clip("Audio/cow-bells.wav");
        // audio.load_clip("Audio/ding.wav");
        // audio.load_clip("Audio/dong.wav");

        // load scene from disk
        ID sceneId = sceneManager.create_scene("Scenes/test.minty");
        Scene &scene = sceneManager.get_scene(sceneId);
        EntityRegistry& er = scene.get_entity_registry();
        SystemRegistry& sr = scene.get_system_registry();

        // load the scene we created, with the camera
        sceneManager.load_scene(sceneId);

        // debug print scene:
        {
            Node node;
            SerializationData data{
                .scene = &scene,
                .entity = NULL_ENTITY};
            Writer writer(node, &data);
            scene.serialize(writer);
            minty::console::print(node);
        }

        //          audio

        // play audio
        // audio.play_background("blinking-forest", 0.2f);

        AudioSystem *audioPtr = sr.find<AudioSystem>();
        Window *windowPtr = &window;
        EntityRegistry* erPtr = &er;

        //          input
        input.emplace_key_down(Key::E, [audioPtr, erPtr](KeyPressEventArgs const &args)
                               { audioPtr->play_spatial("ding", erPtr->find("Model")); });
        input.emplace_key_up(Key::E, [audioPtr, erPtr](KeyPressEventArgs const &args)
                             { audioPtr->play_spatial("dong", erPtr->find("Model")); });

        // quit on key close
        input.emplace_key_down(Key::Escape, [windowPtr](KeyPressEventArgs const &args)
                               { windowPtr->close(); });

        MoveSystem *moveSystem = sr.find<MoveSystem>();
        input.emplace_key_down(Key::W, [moveSystem](KeyPressEventArgs const &args)
                               { moveSystem->forward += 1.0f; });
        input.emplace_key_up(Key::W, [moveSystem](KeyPressEventArgs const &args)
                             { moveSystem->forward -= 1.0f; });
        input.emplace_key_down(Key::S, [moveSystem](KeyPressEventArgs const &args)
                               { moveSystem->forward += -1.0f; });
        input.emplace_key_up(Key::S, [moveSystem](KeyPressEventArgs const &args)
                             { moveSystem->forward -= -1.0f; });
        input.emplace_key_down(Key::D, [moveSystem](KeyPressEventArgs const &args)
                               { moveSystem->right += 1.0f; });
        input.emplace_key_up(Key::D, [moveSystem](KeyPressEventArgs const &args)
                             { moveSystem->right -= 1.0f; });
        input.emplace_key_down(Key::A, [moveSystem](KeyPressEventArgs const &args)
                               { moveSystem->right += -1.0f; });
        input.emplace_key_up(Key::A, [moveSystem](KeyPressEventArgs const &args)
                             { moveSystem->right -= -1.0f; });
        input.emplace_key_down(Key::Space, [moveSystem](KeyPressEventArgs const &args)
                               { moveSystem->up += -1.0f; });
        input.emplace_key_up(Key::Space, [moveSystem](KeyPressEventArgs const &args)
                             { moveSystem->up -= -1.0f; });
        input.emplace_key_down(Key::LeftShift, [moveSystem](KeyPressEventArgs const &args)
                               { moveSystem->up += 1.0f; });
        input.emplace_key_up(Key::LeftShift, [moveSystem](KeyPressEventArgs const &args)
                             { moveSystem->up -= 1.0f; });

        input.emplace_key_down(Key::LeftControl, [moveSystem](KeyPressEventArgs const &args)
                               { moveSystem->faster = true; });
        input.emplace_key_up(Key::LeftControl, [moveSystem](KeyPressEventArgs const &args)
                             { moveSystem->faster = false; });

        TestSystem *testSystem = sr.find<TestSystem>();
        input.emplace_key_down(Key::Pause, [testSystem](KeyPressEventArgs const &args)
                               { testSystem->toggle_pause(); });
        input.emplace_key_down(Key::Insert, [testSystem](KeyPressEventArgs const &args)
                               { testSystem->reset(); });

        CameraControllerSystem *cameraControllerSystem = sr.find<CameraControllerSystem>();
        input.emplace_mouse_down(MouseButton::Left, [cameraControllerSystem, windowPtr](MouseClickEventArgs const &args)
                                 {
                                     if (cameraControllerSystem->mouseDown)
                                     {
                                         // free to move
                                         cameraControllerSystem->mouseDown = false;
                                         windowPtr->set_cursor_mode(CursorMode::Normal);
                                     }
                                     else
                                     {
                                         // lock movement, move camera around
                                         cameraControllerSystem->mouseDown = true;
                                         windowPtr->set_cursor_mode(CursorMode::Disabled);
                                     } });

        input.emplace_mouse_move([cameraControllerSystem](MouseMoveEventArgs const &args)
                                 {
            if(cameraControllerSystem->mouseDown)
            {
                cameraControllerSystem->pitch -= args.dy;
                cameraControllerSystem->yaw += args.dx;
            } });

        windowPtr->set_input(&input);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to init: \"" << e.what() << '"' << std::endl;
    }

    console::log("Game start.");

    return 0;
}

// called when the engine is destroyed
int destroy(Runtime &runtime)
{
    console::log("Game over.");

    // wait for input to close
    // console::wait();

    return 0;
}