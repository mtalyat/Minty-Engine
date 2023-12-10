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
    // console::log(std::format("Vertex3D: {}", sizeof(basic::Vertex3D)));

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

            // basic::create_basic_renderer_builder(rb);
            renderer->init(rb);

            // create textures
            // std::vector<TextureBuilder> textureBuilders =
            //     {
            //         TextureBuilder("Assets/Textures/funny2.png"),
            //         TextureBuilder("Assets/Textures/funny.jpg"),
            //         TextureBuilder("Assets/Textures/texture.jpg"),
            //         TextureBuilder("Assets/Textures/brian.png"),
            //         TextureBuilder("Assets/Textures/crosshair.png"),
            //         TextureBuilder("Assets/Textures/ui.png"),
            //     };
            TextureBuilder tb {
                .name = "funny",
                .path = "Textures/funny.jpg",
                .pixelFormat = TextureBuilder::PixelFormat::RedGreenBlueAlpha,
                .filter = VkFilter::VK_FILTER_NEAREST,
                .addressMode = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT,
                .mipmapMode = VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST,
                .format = VkFormat::VK_FORMAT_R8G8B8A8_SRGB,
            };
            renderer->create_texture(tb);
            tb.name = "funny2";
            tb.path = "Textures/funny2.png";
            renderer->create_texture(tb);
            tb.name = "texture.jpg";
            tb.path = "Textures/texture.jpg";
            renderer->create_texture(tb);

            // create shader
            ShaderBuilder sb;
            sb.name = "shader";
            // basic::create_basic_shader_builder_3d(rb, sb);
            // add uniform data that should be part of every shader
            // shaderBuilder.emplace_uniform_constant(UniformConstantInfo("camera", VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, sizeof(CameraBufferObject), DESCRIPTOR_SET_SHADER, 0));
            sb.uniformConstantInfos.emplace("camera", UniformConstantInfo{
                                                          .name = "camera",
                                                          .type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                          .set = DESCRIPTOR_SET_SHADER,
                                                          .binding = 0,
                                                          .count = 1,
                                                          .size = sizeof(CameraBufferObject),
                                                          .stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
                                                          .ids = {},
                                                      });
            // add push constant that should be part of every shader
            // shaderBuilder.emplace_push_constant(PushConstantInfo("object", VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(DrawCallObject3D)));
            sb.pushConstantInfos.emplace("object", PushConstantInfo{
                                                       .name = "object",
                                                       .stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
                                                       .offset = 0,
                                                       .size = sizeof(DrawCallObject3D),
                                                   });
            // sb.emplace_uniform_constant(UniformConstantInfo(
            //     "material",
            //     VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
            //     VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            //     sizeof(MaterialBufferObject),
            //     DESCRIPTOR_SET_MATERIAL,
            //     0));
            sb.uniformConstantInfos.emplace("material", UniformConstantInfo{
                                                            .name = "material",
                                                            .type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                            .set = DESCRIPTOR_SET_MATERIAL,
                                                            .binding = 0,
                                                            .count = 1,
                                                            .size = sizeof(MaterialBufferObject),
                                                            .stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
                                                            .ids = {},
                                                        });
            // sb.emplace_uniform_constant(UniformConstantInfo(
            //     "textures",
            //     VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
            //     VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            //     sizeof(VkSampler),
            //     DESCRIPTOR_SET_MATERIAL,
            //     1,
            //     4,
            //     {0, 1, 2, 3}));
            sb.uniformConstantInfos.emplace("textures", UniformConstantInfo{
                                                            .name = "textures",
                                                            .type = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                            .set = DESCRIPTOR_SET_MATERIAL,
                                                            .binding = 1,
                                                            .count = 4,
                                                            .size = sizeof(VkSampler),
                                                            .stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
                                                            .ids = {0, 1, 2, 0},
                                                        });
            // ID shaderId = rb.emplace_shader(sb);
            ID shaderId = renderer->create_shader(sb);

            // create shader pass
            ShaderPassBuilder spb;
            spb.name = "shaderPass";
            spb.shaderId = shaderId;
            spb.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            spb.cullMode = VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
            spb.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
            spb.frontFace = VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
            spb.lineWidth = 1.0f;
            // basic::create_basic_shader_pass_builder_3d(rb, spb);
            // shaderPassBuilder.emplace_vertex_binding(0, sizeof(Vertex3D));
            spb.vertexBindings.push_back(VkVertexInputBindingDescription{
                .binding = 0,
                .stride = sizeof(basic::Vertex3D),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
            });
            // shaderPassBuilder.emplace_vertex_attribute(0, 0, sizeof(Vector3), offsetof(Vertex3D, pos), VkFormat::VK_FORMAT_R32G32B32_SFLOAT);
            spb.vertexAttributes.push_back(VkVertexInputAttributeDescription{
                .location = 0,
                .binding = 0,
                .format = VkFormat::VK_FORMAT_R32G32B32_SFLOAT,
                .offset = offsetof(basic::Vertex3D, pos),
            });
            // shaderPassBuilder.emplace_vertex_attribute(0, 1, sizeof(Vector3), offsetof(Vertex3D, normal), VkFormat::VK_FORMAT_R32G32B32_SFLOAT);
            spb.vertexAttributes.push_back(VkVertexInputAttributeDescription{
                .location = 1,
                .binding = 0,
                .format = VkFormat::VK_FORMAT_R32G32B32_SFLOAT,
                .offset = offsetof(basic::Vertex3D, normal),
            });
            // shaderPassBuilder.emplace_vertex_attribute(0, 2, sizeof(Vector2), offsetof(Vertex3D, coord), VkFormat::VK_FORMAT_R32G32_SFLOAT);
            spb.vertexAttributes.push_back(VkVertexInputAttributeDescription{
                .location = 2,
                .binding = 0,
                .format = VkFormat::VK_FORMAT_R32G32_SFLOAT,
                .offset = offsetof(basic::Vertex3D, coord),
            });
            // spb.emplace_stage(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, "Assets/Shaders/vert.spv", *renderer);
            spb.stages.push_back(ShaderPassBuilder::ShaderStageInfo{
                .stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
                .code = assets::load_chars("Shaders/vert.spv"),
                .entry = "main",
            });
            // spb.emplace_stage(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, "Assets/Shaders/frag.spv", *renderer);
            spb.stages.push_back(ShaderPassBuilder::ShaderStageInfo{
                .stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
                .code = assets::load_chars("Shaders/frag.spv"),
                .entry = "main",
            });

            // ID shaderPassId = rb.emplace_shader_pass(spb);
            ID shaderPassId = renderer->create_shader_pass(spb);

            // create material templates
            MaterialTemplateBuilder mtb;
            mtb.name = "materialTemplate";
            mtb.shaderPassIds = {shaderPassId};
            MaterialBufferObject mbo{
                .textureId = 0,
                .color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)};
            // mtb.emplace_default_value("material", Dynamic(&mbo, sizeof(MaterialBufferObject)));
            mtb.defaultValues.emplace("material", Dynamic(&mbo, sizeof(MaterialBufferObject)));
            // ID materialTemplateId = rb.emplace_material_template(mtb);
            ID materialTemplateId = renderer->create_material_template(mtb);

            // create materials
            MaterialBuilder mb; // world
            mb.templateId = materialTemplateId;
            mb.name = "material1";
            renderer->create_material(mb);

            mb.name = "material2";
            mbo.textureId = 1;
            mb.values["material"] = Dynamic(&mbo, sizeof(MaterialBufferObject));
            renderer->create_material(mb);

            mb.name = "material3";
            mbo.textureId = 2;
            mb.values["material"] = Dynamic(&mbo, sizeof(MaterialBufferObject));
            renderer->create_material(mb);
            // MaterialBuilder mb2(materialTemplateId2); // ui

            // init renderer with builders
            // renderer->init(rb);
        }

        // {
        //     Info info("TestProject", 1, 0, 0);

        //     RendererBuilder rb;
        //     rb.info = &info;

        //     renderer->init(rb);

        //     renderer->load_texture("Textures/funny.jpg");
        //     renderer->load_texture("Textures/funny2.png");
        //     renderer->load_texture("Textures/texture.jpg");
        //     renderer->load_texture("Textures/brian.png");

        //     renderer->load_shader("Shaders/shader.minty");
        //     renderer->load_shader_pass("Shaders/shaderPass.minty");
        //     renderer->load_material_template("Materials/materialTemplate.minty");
        //     renderer->load_material("Materials/material1.minty");
        //     renderer->load_material("Materials/material2.minty");
        //     renderer->load_material("Materials/material3.minty");
        //     renderer->load_material("Materials/material4.minty");
        // }

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