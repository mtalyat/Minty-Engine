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
        
        // load scene from disk
        ID sceneId = sceneManager.create_scene("Scenes/test.minty");
        Scene &scene = sceneManager.get_scene(sceneId);
        EntityRegistry& er = scene.get_entity_registry();
        SystemRegistry& sr = scene.get_system_registry();

        // set the camera to one of the cameras in the scene
        RenderSystem* renderSystem = sr.find<RenderSystem>();
        renderSystem->set_main_camera(er.find<CameraComponent>());

        // load the scene we created, with the camera
        sceneManager.load_scene(sceneId);

        // debug print scene:
        {
            Node node;
            SerializationData data{
                .scene = &scene,
                .entity = NULL_ENTITY};
            Writer writer(node, &data);
            writer.write("Scene", scene);
            minty::console::print(node);
        }

        //          audio

        // play audio
        // audio.play_background("blinking-forest", 0.2f);

        AudioSystem *audioPtr = sr.find<AudioSystem>();
        Window *windowPtr = &window;
        EntityRegistry* erPtr = &er;

        input.emplace_key_down(Key::E, [audioPtr, erPtr](KeyPressEventArgs const &args)
                               { audioPtr->play_spatial("ding", erPtr->find("Model")); });
        input.emplace_key_up(Key::E, [audioPtr, erPtr](KeyPressEventArgs const &args)
                             { audioPtr->play_spatial("dong", erPtr->find("Model")); });

        // quit on key close
        input.emplace_key_down(Key::Escape, [windowPtr](KeyPressEventArgs const &args)
                               { windowPtr->close(); });

        // animator
        input.emplace_key_down(Key::D1, [erPtr](KeyPressEventArgs const& args)
        {
            Entity spriteEntity = erPtr->find("Sprite");
            AnimatorComponent& animatorComp = erPtr->get<AnimatorComponent>(spriteEntity);
            animatorComp.animator.set_variable("move", (animatorComp.animator.get_variable("move") + 1) & 1);
        });
        input.emplace_key_down(Key::D2, [erPtr](KeyPressEventArgs const& args)
        {
            Entity spriteEntity = erPtr->find("Sprite");
            AnimatorComponent& animatorComp = erPtr->get<AnimatorComponent>(spriteEntity);
            animatorComp.animator.set_variable("flicker", (animatorComp.animator.get_variable("flicker") + 1) & 1);
        });
        input.emplace_key_down(Key::D3, [erPtr](KeyPressEventArgs const& args)
        {
            Entity spriteEntity = erPtr->find("Model");
            AnimatorComponent& animatorComp = erPtr->get<AnimatorComponent>(spriteEntity);
            animatorComp.animator.set_variable("move", (animatorComp.animator.get_variable("move") + 1) & 1);
        });
        input.emplace_key_down(Key::D4, [erPtr](KeyPressEventArgs const& args)
        {
            Entity spriteEntity = erPtr->find("Model");
            AnimatorComponent& animatorComp = erPtr->get<AnimatorComponent>(spriteEntity);
            animatorComp.animator.set_variable("flicker", (animatorComp.animator.get_variable("flicker") + 1) & 1);
        });

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
        console::error(std::format("Failed to init: \"{}\"", e.what()));
        // std::cerr << "Failed to init: \"" << e.what() << '"' << std::endl;
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