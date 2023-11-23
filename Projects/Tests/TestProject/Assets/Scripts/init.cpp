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

            // create textures
            std::vector<TextureBuilder> textureBuilders =
                {
                    TextureBuilder("Assets/Textures/funny.jpg"),
                    TextureBuilder("Assets/Textures/funny2.png"),
                    TextureBuilder("Assets/Textures/texture.jpg"),
                    TextureBuilder("Assets/Textures/brian.png"),
                    TextureBuilder("Assets/Textures/crosshair.png"),
                    TextureBuilder("Assets/Textures/ui.png"),
                };
            for (TextureBuilder &builder : textureBuilders)
            {
                // builder.set_filter(VkFilter::VK_FILTER_NEAREST);
                rb.emplace_texture(builder);
            }

            // create shader
            ShaderBuilder sb;
            basic::create_basic_shader_builder_3d(rb, sb);
            sb.emplace_uniform_constant(UniformConstantInfo(
                "material",
                VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
                VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                sizeof(MaterialBufferObject),
                DESCRIPTOR_SET_MATERIAL,
                0));
            sb.emplace_uniform_constant(UniformConstantInfo(
                "textures",
                VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
                VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                sizeof(VkSampler),
                DESCRIPTOR_SET_MATERIAL,
                1,
                // 4,
                // {0, 1, 2, 3}
                1,
                {0}));
            ID shaderId = rb.emplace_shader(sb);

            // create shader pass
            ShaderPassBuilder spb(shaderId);
            basic::create_basic_shader_pass_builder_3d(rb, spb);
            spb.emplace_stage(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, "Assets/Shaders/vert.spv", *renderer);
            spb.emplace_stage(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, "Assets/Shaders/frag.spv", *renderer);
            // spb.set_front_face(VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE);
            // spb.set_cull_mode(VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT);
            ID shaderPassId = rb.emplace_shader_pass(spb);

            // // create ui shader
            // ShaderBuilder sb2;
            // basic::create_basic_shader_builder_ui(rb, sb2);
            // sb2.emplace_uniform_constant(UniformConstantInfo(
            //     "material",
            //     VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
            //     VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            //     sizeof(MaterialBufferObject),
            //     DESCRIPTOR_SET_MATERIAL,
            //     0));
            // ID shaderId2 = rb.emplace_shader(sb2);

            // // create ui shader pass
            // ShaderPassBuilder spb2(shaderId2);
            // basic::create_basic_shader_pass_builder_3d(rb, spb2);
            // spb2.emplace_stage(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, "Assets/Shaders/uivert.spv", *renderer);
            // spb2.emplace_stage(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, "Assets/Shaders/uifrag.spv", *renderer);
            // ID shaderPassId2 = rb.emplace_shader_pass(spb2);

            // create material templates
            MaterialTemplateBuilder mtb({shaderPassId});
            MaterialBufferObject mbo{
                .textureId = 0,
                .color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)};
            mtb.emplace_default_value("material", Dynamic(&mbo, sizeof(MaterialBufferObject)));
            ID materialTemplateId = rb.emplace_material_template(mtb);

            // MaterialTemplateBuilder mtb2({shaderPassId2});
            // MaterialBufferObject mbo{
            //     .color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)};
            // mtb2.emplace_default_value("material", Dynamic(&mbo, sizeof(MaterialBufferObject)));
            // ID materialTemplateId2 = rb.emplace_material_template(mtb2);

            // create materials
            MaterialBuilder mb(materialTemplateId); // world
            rb.emplace_material(mb);
            // MaterialBuilder mb2(materialTemplateId); // world 2
            // mbo.textureId = 1;
            // mbo.color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
            // mb2.emplace_value("material", Dynamic(&mbo, sizeof(MaterialBufferObject)));
            // rb.emplace_material(mb2);
            // MaterialBuilder mb2(materialTemplateId2); // ui

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
            meshComponent.materialId = static_cast<ID>(0);
            // meshComponent.materialId = static_cast<ID>(i % 4);
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

    delete mesh;

    // wait for input to close
    // console::wait();
}