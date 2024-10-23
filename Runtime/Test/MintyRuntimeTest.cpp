#include "MintyLib.h"

#include <stdexcept>

using namespace Minty;

#define TEST 1

int main()
{
#if TEST == -1

	ApplicationBuilder appBuilder{};
	Application application(appBuilder);

	return 0;

#elif TEST == 0

	String const path = "IDs.txt";

	PhysicalFile file(path, File::Flags::Write);

	for (Size i = 0; i < 100; i++)
	{
		UUID id = UUID::create();
		String idString = to_string(id);
		String line = std::format("{}\n", idString);
		file.write(line.data(), line.size());
	}

	file.flush();
	file.close();

	Operations::open(path);

	return 0;

#elif TEST == 1
	ApplicationBuilder appBuilder{};
	Application app(appBuilder);

	int result = 1;

	try
	{
		result = app.run();
	}
	catch (const std::runtime_error& error)
	{
		String message = std::format("Runtime error: {}", error.what());
		MINTY_ABORT(message);
		return EXIT_FAILURE;
	}
	catch (const std::exception& exception)
	{
		String message = std::format("Runtime exception: {}", exception.what());
		MINTY_ABORT(message);
		return EXIT_FAILURE;
	}
	catch (...)
	{
		String message = "Unidentified exception.";
		MINTY_ABORT(message);
		return EXIT_FAILURE;
	}

	Console::log_color("Program finished successfully.", Console::Color::BrightGreen);

	return result;
#elif TEST == 2
	// input
	PhysicalFile readFile("Assets/test.scene", File::Flags::Read);
	TextFileReader reader(&readFile);
	reader.get_current_node().print();
	Console::log("-------------------");

	{
		// output
		PhysicalFile writeFile("Assets/test_out.scene", File::Flags::Write);
		TextFileWriter writer(&writeFile);

		// write input file
		std::vector<Byte> buffer;
		buffer.reserve(256);
		std::vector<std::pair<size_t, const Node*>> stack;
		stack.push_back({ 0, &reader.get_current_node() });
		while (!stack.empty())
		{
			auto const [indent, node] = stack.back();
			stack.pop_back();

			// adust input to match
			while (writer.depth() < indent)
			{
				writer.indent();
			}
			while (writer.depth() > indent)
			{
				writer.outdent();
			}

			// write node
			std::vector<Byte> data = node->get_data().get_data_vector<Byte>();
			data.push_back(0);
			Char* dataPtr = static_cast<Char*>(static_cast<void*>(data.data()));
			String dataStr(dataPtr);
			writer.write_string(node->get_name(), dataStr);

			// add children in reverse order
			const std::vector<Node>& children = node->get_children();

			for (auto it = children.rbegin(); it != children.rend(); it++)
			{
				stack.push_back({ indent + 1, &(*it) });
			}
		}
		Console::log("-------------------");
	}

	// input2
	PhysicalFile readFile2("Assets/test.scene", File::Flags::Read);
	TextFileReader reader2(&readFile2);
	reader2.get_current_node().print();
	Console::log("-------------------");

	return 0;
#elif TEST == 3

	PhysicalFile writeFile("Assets/test.txt", File::Flags::Write);
	TextFileWriter writer(&writeFile);

#define VALUES { 1.0f, 2.0f, -4.0f, 8.0f, 16.0f }

	Debug::log("Array");

	std::array<Float, 5> arrayValues = VALUES;
	writer.write("Array", arrayValues);

	Debug::log("Vector");

	std::vector<Float> vectorValues = VALUES;
	writer.write("Vector", vectorValues);

	Debug::log("Unordered Set");

	std::unordered_set<Float> uSetValues = VALUES;
	writer.write("Unordered Set", uSetValues);

	Debug::log("Set");

	std::set<Float> setValues = VALUES;
	writer.write("Set", setValues);

#undef VALUES

#define VALUES { { 0, 1.0f }, {2, -4.0f}, {4, 16.0f }, {1, 2.0f}, {3, 8.0f } }

	Debug::log("Unordered Map");

	std::unordered_map<Int, Float> uMapValues = VALUES;
	writer.write("Unordered Map", uMapValues);

	Debug::log("Map");

	std::map<Int, Float> mapValues = VALUES;
	writer.write("Map", mapValues);

	writeFile.flush();

	Debug::log("Math");

	Float2 f2value = { 1.0f, -3.2f };
	writer.write("f2", f2value);

#undef VALUES

	PhysicalFile readFile("Assets/test.txt", File::Flags::Read);
	TextFileReader reader(&readFile);

	reader.get_current_node().print();

	Debug::log("Array");

	reader.read("Array", arrayValues);

	Debug::log("Vector");

	reader.read("Vector", vectorValues);

	Debug::log("Unordered Set");

	reader.read("Unordered Set", uSetValues);

	Debug::log("Set");

	reader.read("Set", setValues);

	Debug::log("Unordered Map");

	reader.read("Unordered Map", uMapValues);

	Debug::log("Map");

	reader.read("Map", mapValues);

	Debug::log("Math");

	reader.read("f2", f2value);

	Debug::log(std::format("f2 value read: {}", to_string(f2value)));

	reader.get_current_node().print();

#elif TEST == 4

	ApplicationBuilder appBuilder{};
	Application app(appBuilder);

	Ref<ShaderModule> vertexShaderModule = AssetManager::load<ShaderModule>("Assets/vert.spv");
	Ref<ShaderModule> fragShaderModule = AssetManager::load<ShaderModule>("Assets/frag.spv");

	Ref<Shader> shader = AssetManager::load<Shader>("Assets/test.shader");

	return 0;

#elif TEST == 5

ApplicationBuilder appBuilder{};
Application application(appBuilder);

try
{
	SceneBuilder sceneBuilder{};
	sceneBuilder.id = UUID::create();

	Scene scene(sceneBuilder);
	EntityRegistry& registry = scene.get_entity_registry();

	Entity entity = registry.create();

	NameComponent& nameComp = registry.emplace<NameComponent>(entity);
	nameComp.name = "Test Entity";

	TagComponent& tagComp = registry.emplace<TagComponent>(entity);
	tagComp.tag = "Test Tag";

	registry.emplace<EnabledComponent>(entity);

	DynamicContainer container{};
	TextMemoryWriter writer(&container);

	registry.serialize_entity(writer, entity);

	Char eol = '\0';
	container.append(&eol, sizeof(Char));

	String text(static_cast<Char const*>(container.data()));

	// show results manually

	Console::log("Serialization:");
	Console::log(text);

	// destroy existing entity so we know what we read in was from the stream
	registry.destroy_immediate(entity);

	// read entity
	TextMemoryReader reader(&container);
	entity = registry.deserialize_entity(reader);

	// show results
	Console::log("Deserialization:");
	registry.print(entity);
}
catch (const std::runtime_error& error)
{
	String message = std::format("Runtime error: {}.", error.what());
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}
catch (const std::exception& exception)
{
	String message = std::format("Runtime exception: {}.", exception.what());
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}
catch (...)
{
	String message = "Unidentified exception.";
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}

return 0;

#elif TEST == 6

ApplicationBuilder appBuilder{};
Application application(appBuilder);

try
{
	SceneBuilder sceneBuilder{};
	sceneBuilder.id = UUID::create();

	Scene scene(sceneBuilder);
	SystemRegistry& registry = scene.get_system_registry();

	DynamicContainer container{};
	TextMemoryWriter writer(&container);

	registry.serialize(writer);

	Console::log("Serialization:");

	Char eol = '\0';
	container.append(&eol, sizeof(Char));
	String text = static_cast<const Char*>(container.data());
	Console::log(text);

	registry.clear();

	TextMemoryReader reader(&container);
	registry.deserialize(reader);

	Console::log("Deserialization:");

	// serialize again to visually see it
	container.clear();
	writer = TextMemoryWriter(&container);
	registry.serialize(writer);

	container.append(&eol, sizeof(Char));
	text = static_cast<const Char*>(container.data());
	Console::log(text);
}
catch (const std::runtime_error& error)
{
	String message = std::format("Runtime error: {}.", error.what());
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}
catch (const std::exception& exception)
{
	String message = std::format("Runtime exception: {}.", exception.what());
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}
catch (...)
{
	String message = "Unidentified exception.";
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}

return 0;

#elif TEST == 7

ApplicationBuilder appBuilder{};

Application application(appBuilder);

try
{
	Ref<Scene> scene = AssetManager::load<Scene>("Assets/test.scene");

	DynamicContainer container{};
	TextMemoryWriter writer(&container);

	scene->serialize(writer);

	Console::log(std::format("Scene: {}", to_string(scene->id())));

	Char eol = '\0';
	container.append(&eol, sizeof(Char));
	String text = static_cast<const Char*>(container.data());
	Console::log(text);
}
catch (const std::runtime_error& error)
{
	String message = std::format("Runtime error: {}.", error.what());
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}
catch (const std::exception& exception)
{
	String message = std::format("Runtime exception: {}.", exception.what());
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}
catch (...)
{
	String message = "Unidentified exception.";
	MINTY_ABORT(message);
	return EXIT_FAILURE;
}

return 0;

#endif
}