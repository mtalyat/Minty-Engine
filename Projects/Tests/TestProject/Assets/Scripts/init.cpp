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

        // load scene from disk
        ID sceneId = sceneManager->create_scene("Scenes/test.minty");
        Scene &scene = sceneManager->get_scene(sceneId);
        EntityRegistry *er = scene.get_entity_registry();
        SystemRegistry *sr = scene.get_system_registry();

        // get camera
        Entity cameraEntity = er->find_by_type<CameraComponent>();
        // PositionComponent& cameraPosition = er->get<PositionComponent>(cameraEntity);
        // cameraPosition.position = Vector3(0.0f, 0.0f, ENTITY_COUNT * -2.0f);

        // load the scene we created, with the camera
        sceneManager->load_scene(sceneId, cameraEntity);

        // debug print scene:
        Node node;
        Writer writer(node, &scene);
        scene.serialize(writer);
        node.print();

        //          input

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