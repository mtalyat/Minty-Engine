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
void init(Runtime &runtime)
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

        Engine *engine = runtime.get_engine();
        Window *window = engine->get_window();
        Renderer *renderer = engine->get_renderer();
        AudioEngine *audio = engine->get_audio_engine();
        SceneManager *sceneManager = engine->get_scene_manager();

        // create renderer
        Info info("TestProject", 1, 0, 0);

        // use defaults for most things
        RendererBuilder rb(&info);

        // basic::create_basic_renderer_builder(rb);
        renderer->init(rb);

        // create textures
        renderer->load_texture("Textures/oak_planks.png");
        renderer->load_texture("Textures/pattern.png");
        renderer->load_texture("Textures/texture.jpg");
        renderer->load_texture("Resources/Models/pumpkin_tex.jpg");

        // create shader
        renderer->load_shader("Shaders/shader.minty");

        // create shader pass
        renderer->load_shader_pass("Shaders/shaderPass.minty");

        // create material template
        renderer->load_material_template("Materials/materialTemplate.minty");

        // create materials
        renderer->load_material("Materials/material1.minty");
        renderer->load_material("Materials/material2.minty");
        renderer->load_material("Materials/material3.minty");
        renderer->load_material("Materials/pumpkinMat.minty");

        // create models
        renderer->load_mesh("Resources/Models/pumpkin.obj");

        // create audio
        audio->load_clip("Audio/blinking-forest.wav");
        audio->load_clip("Audio/cow-bells.wav");
        audio->load_clip("Audio/ding.wav");
        audio->load_clip("Audio/dong.wav");

        // load scene from disk
        ID sceneId = sceneManager->create_scene("Scenes/test.minty");
        Scene &scene = sceneManager->get_scene(sceneId);
        EntityRegistry *er = scene.get_entity_registry();
        SystemRegistry *sr = scene.get_system_registry();

        // load the scene we created, with the camera
        sceneManager->load_scene(sceneId);

        // debug print scene:
        {
            Node node;
            SerializationData data{
                .scene = &scene,
                .entity = NULL_ENTITY};
            Writer writer(node, &data);
            scene.serialize(writer);
            node.print();
        }

        //          audio

        // play audio
        audio->play_background("blinking-forest", 0.2f);

        //          input
        input.emplace_key_down(Key::E, [audio, er](KeyPressEventArgs const &args)
                               { audio->play_spatial("ding", er->find_by_name("Model")); });
        input.emplace_key_up(Key::E, [audio, er](KeyPressEventArgs const &args)
                               { audio->play_spatial("dong", er->find_by_name("Model")); });

        // quit on key close
        input.emplace_key_down(Key::Escape, [window](KeyPressEventArgs const &args)
                               { window->close(); });

        MoveSystem *moveSystem = sr->find_by_type<MoveSystem>();
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

        TestSystem *testSystem = sr->find_by_type<TestSystem>();
        input.emplace_key_down(Key::Pause, [testSystem](KeyPressEventArgs const &args)
                               { testSystem->toggle_pause(); });
        input.emplace_key_down(Key::Insert, [testSystem](KeyPressEventArgs const &args)
                               { testSystem->reset(); });

        CameraControllerSystem *cameraControllerSystem = sr->find_by_type<CameraControllerSystem>();
        input.emplace_mouse_down(MouseButton::Left, [cameraControllerSystem, window](MouseClickEventArgs const &args)
                                 {
                                     if (cameraControllerSystem->mouseDown)
                                     {
                                         // free to move
                                         cameraControllerSystem->mouseDown = false;
                                         window->set_cursor_mode(CursorMode::Normal);
                                     }
                                     else
                                     {
                                         // lock movement, move camera around
                                         cameraControllerSystem->mouseDown = true;
                                         window->set_cursor_mode(CursorMode::Disabled);
                                     } });

        input.emplace_mouse_move([cameraControllerSystem](MouseMoveEventArgs const &args)
                                 {
            if(cameraControllerSystem->mouseDown)
            {
                cameraControllerSystem->pitch -= args.dy;
                cameraControllerSystem->yaw += args.dx;
            } });

        window->set_input(&input);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    console::log("Game start.");
}

// called when the engine is destroyed
void destroy(Runtime &runtime)
{
    console::log("Game over.");

    // wait for input to close
    // console::wait();
}