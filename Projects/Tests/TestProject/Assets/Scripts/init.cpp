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
Mesh *mesh;

// called when the engine is initialized
void init(Runtime &runtime)
{
    int const ENTITY_COUNT = 10;

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

        {
            // create renderer
            Info info("TestProject", 1, 0, 0);

            // use defaults for most things
            RendererBuilder rb(&info);

            basic::create_basic_renderer_builder(rb);

            rb.set_max_shaders(2);
            rb.set_max_textures(6);
            rb.set_max_materials(8);

            // create textures
            TextureBuilder tb0; // for "pixel art" textures
            tb0.set_filter(VkFilter::VK_FILTER_NEAREST);
            TextureBuilder tb1; // for "normal" textures

            rb.emplace_texture(&tb1, "Assets/Textures/funny.jpg");
            rb.emplace_texture(&tb1, "Assets/Textures/funny2.png");
            rb.emplace_texture(&tb1, "Assets/Textures/texture.jpg");
            rb.emplace_texture(&tb0, "Assets/Textures/brian.png");
            rb.emplace_texture(&tb0, "Assets/Textures/crosshair.png");
            rb.emplace_texture(&tb0, "Assets/Textures/ui.png");

            // create shader
            ShaderBuilder sb;
            basic::create_basic_shader_builder_3d(rb, sb);
            sb.set_material_size(sizeof(MaterialBufferObject));

            sb.emplace_uniform_constant(
                "materials",
                VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
                0, 2,
                VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                sizeof(MaterialBufferObject) * rb.get_max_materials(),
                1);

            rb.emplace_shader(&sb, "Assets/Shaders/vert.spv", "Assets/Shaders/frag.spv");
            
            ShaderBuilder sb2;
            basic::create_basic_shader_builder_ui(rb, sb2);
            sb2.set_material_size(sizeof(MaterialBufferObject));
            
            sb2.emplace_uniform_constant(
                "materials",
                VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
                0, 1,
                VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                sizeof(MaterialBufferObject) * rb.get_max_materials(),
                1);

            rb.emplace_shader(&sb2, "Assets/Shaders/uivert.spv", "Assets/Shaders/uifrag.spv");

            // create materials
            MaterialBuilder mb(0);  // world
            MaterialBuilder mb2(1); // ui

            Vector4 whiteColor(1.0f, 1.0f, 1.0f, 1.0f);

            MaterialBufferObject mbo = {
                .textureId = 0,
                .color = whiteColor
            };
            rb.emplace_material(&mb, &mbo);
            mbo.textureId = 1;
            rb.emplace_material(&mb, &mbo);
            mbo.textureId = 2;
            rb.emplace_material(&mb, &mbo);
            mbo.textureId = 3;
            rb.emplace_material(&mb, &mbo);
            mbo.textureId = 4;
            rb.emplace_material(&mb2, &mbo);
            mbo.textureId = 5;
            rb.emplace_material(&mb2, &mbo);

            // init renderer with builders
            renderer->init(rb);
        }

        // load scene from disk
        ID sceneId = sceneManager->create_scene("Assets/Scenes/test.minty");
        Scene &scene = sceneManager->get_scene(sceneId);
        EntityRegistry *er = scene.get_entity_registry();
        SystemRegistry *sr = scene.get_system_registry();

        // get camera
        Entity cameraEntity = er->find_by_type<CameraComponent>();
        // PositionComponent& cameraPosition = er->get<PositionComponent>(cameraEntity);
        // cameraPosition.position = Vector3(0.0f, 0.0f, ENTITY_COUNT * -2.0f);

        // load the scene we created, with the camera
        sceneManager->load_scene(sceneId, cameraEntity);

        // cubes all use same mesh, but different materials
        mesh = new Mesh(*renderer);
        basic::create_basic_cube(*mesh);
        for (int i = 0; i < ENTITY_COUNT; i++)
        {
            Entity e = er->find_by_name(std::format("Cube {}", i));

            if (e == NULL_ENTITY)
            {
                break;
            }

            MeshComponent &meshComponent = er->get<MeshComponent>(e);
            meshComponent.mesh = mesh;
            meshComponent.materialId = static_cast<ID>(i % 4);
        }

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
        {
            moveSystem->forward += 1.0f;
        });
        input.emplace_key_up(Key::W, [moveSystem](KeyPressEventArgs const &args)
        {
            moveSystem->forward -= 1.0f;
        });
        input.emplace_key_down(Key::S, [moveSystem](KeyPressEventArgs const &args) 
        {
            moveSystem->forward += -1.0f;
        });
        input.emplace_key_up(Key::S, [moveSystem](KeyPressEventArgs const &args)
        {
            moveSystem->forward -= -1.0f;
        });
        input.emplace_key_down(Key::D, [moveSystem](KeyPressEventArgs const &args) 
        {
            moveSystem->right += 1.0f;
        });
        input.emplace_key_up(Key::D, [moveSystem](KeyPressEventArgs const &args)
        {
            moveSystem->right -= 1.0f;
        });
        input.emplace_key_down(Key::A, [moveSystem](KeyPressEventArgs const &args) 
        {
            moveSystem->right += -1.0f;
        });
        input.emplace_key_up(Key::A, [moveSystem](KeyPressEventArgs const &args)
        {
            moveSystem->right -= -1.0f;
        });
        input.emplace_key_down(Key::Space, [moveSystem](KeyPressEventArgs const &args) 
        {
            moveSystem->up += -1.0f;
        });
        input.emplace_key_up(Key::Space, [moveSystem](KeyPressEventArgs const &args)
        {
            moveSystem->up -= -1.0f;
        });
        input.emplace_key_down(Key::LeftShift, [moveSystem](KeyPressEventArgs const &args) 
        {
            moveSystem->up += 1.0f;
        });
        input.emplace_key_up(Key::LeftShift, [moveSystem](KeyPressEventArgs const &args)
        {
            moveSystem->up -= 1.0f;
        });

        input.emplace_key_down(Key::LeftControl, [moveSystem](KeyPressEventArgs const& args)
        {
            moveSystem->faster = true;
        });
        input.emplace_key_up(Key::LeftControl, [moveSystem](KeyPressEventArgs const& args)
        {
            moveSystem->faster = false;
        });

        CameraControllerSystem* cameraControllerSystem = sr->find_by_type<CameraControllerSystem>();

        input.emplace_mouse_down(MouseButton::Left, [cameraControllerSystem, window](MouseClickEventArgs const& args)
        {
            if(cameraControllerSystem->mouseDown)
            {
                // free to move
                cameraControllerSystem->mouseDown = false;
                window->set_cursor_mode(CursorMode::Normal);
            } else
            {
                // lock movement, move camera around
                cameraControllerSystem->mouseDown = true;
                window->set_cursor_mode(CursorMode::Disabled);
            }
            
        });

        input.emplace_mouse_move([cameraControllerSystem](MouseMoveEventArgs const& args)
        {
            if(cameraControllerSystem->mouseDown)
            {
                cameraControllerSystem->pitch -= args.dy;
                cameraControllerSystem->yaw += args.dx;
            }
        });

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

    delete mesh;

    // wait for input to close
    // console::wait();
}