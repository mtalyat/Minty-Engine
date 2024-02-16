#include "init.h"

#include "Scripts/types.h"

// components
#include "Scripts/Components/MoveComponent.h"

// system
#include "Scripts/Systems/CameraControllerSystem.h"
#include "Scripts/Systems/MoveSystem.h"

#include <iostream>

using namespace minty;
using namespace game;

InputMap input;

// called when the engine is initialized
int init(Runtime& runtime)
{
    try
    {
        // components
        EntityRegistry::register_component<game::MoveComponent>("Move");

        // systems
        SystemRegistry::register_system<game::CameraControllerSystem>("CameraController");
        SystemRegistry::register_system<game::MoveSystem>("Move");

        Window &window = runtime.get_window();
        // window.maximize();
        RenderEngine &renderer = runtime.get_render_engine();
        AudioEngine &audio = runtime.get_audio_engine();
        SceneManager &sceneManager = runtime.get_scene_manager();

        // initialize renderer for this project
        {
            RenderEngineBuilder rb
            {
                .info= &runtime.get_info(),
                .window = &window,
            };
            renderer.init(rb);
        }
        
        // load scene from disk
        ID sceneId = sceneManager.create_scene("Scenes/test.mscene");
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
            Console::print(node);
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
        Console::error(std::format("Failed to init: \"{}\"", e.what()));
        // std::cerr << "Failed to init: \"" << e.what() << '"' << std::endl;
    }

    Console::log("Game start.");

    return 0;
}

// called when the engine is destroyed
int destroy(Runtime& runtime)
{
    Console::log("Game over.");

    // wait for input to close
    // console::wait();

    return 0;
}