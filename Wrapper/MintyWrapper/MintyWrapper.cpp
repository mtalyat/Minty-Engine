// MintyWrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Minty.h>

using namespace minty;

typedef CommandLineParser::Parameter Param;
typedef CommandLineParser::Argument Arg;

void print_wrap_stats(Path const& path, std::vector<std::pair<String, String>> const& stats)
{
	int const reportMinWidth = 20;
	int reportWidth = reportMinWidth;

	// get max width from stats
	for (auto const& [left, right] : stats)
	{
		int width = static_cast<int>(left.length() + right.length());

		if (width > reportWidth)
		{
			reportWidth = width;
		}
	}

	// print the table
	String line = "+-" + String(reportWidth, '-') + "-+";

	std::cout
		<< line << "\n"
		<< "| WRAP FILE REPORT    " << String(reportWidth - reportMinWidth, ' ') << " |\n"
		<< line << "\n";

	for (auto const& [left, right] : stats)
	{
		int width = static_cast<int>(left.length() + right.length());

		std::cout << "| " << left << String(reportWidth - width, ' ') << right << " |\n";
	}

	std::cout
		<< line << "\n";
}

int main(int argc, char const* argv[])
{
	CommandLineParser parser;

	parser.add_parameter(Param("file", 0));					// the path to the program
	parser.add_parameter(Param("path", 1));					// the path to the file to wrap
	parser.add_parameter(Param("name", "-n"));				// the name of the wrap being created
	parser.add_parameter(Param("compression", "-c"));		// the default compression level
	parser.add_parameter(Param("base", "-b"));				// the base file path for all files added to this wrap file
	parser.add_parameter(Param("version", "-v"));			// the content version
	parser.add_parameter(Param("help", "--help", 0));		// shows the help text
	parser.add_parameter(Param("update", "--update", 0));	// updates the wrap file
	parser.add_parameter(Param("info", "--info", 0));		// prints the info for the wrap file

	parser.parse(argc, argv);

	if (parser.get_argument("help"))
	{
		std::cout
			<< "Command: wrap [file path] [flags]\n"
			<< "Flags:\n"
			<< "--help ........... prints the help text.\n"
			<< "--info ........... prints the info of the Wrap file, when [file path] is a Wrap file.\n"
			<< "--update ......... updates the appropriate wrap file and replaces it.\n"
			<< "-n <name> ........ specifies the name of the new Wrap file.\n"
			<< "-c <[0-9]> ....... specifies the level of compression.\n"
			<< "-b <path> ........ specifies the base virtual path shared by all of the files in the Wrap file.\n"
			<< "-v <number> ...... specifies the content version number.\n";

		if (!parser.get_argument("path")) return 0;

		std::cout << '\n';
	}

	Arg arg;

	if (!parser.get_argument("path", arg))
	{
		std::cout << "Missing path.\n";
		return 1;
	}

	Path path = Path(arg.args.front());

	if (!std::filesystem::exists(path))
	{
		std::cerr << "Path does not exist: " << path.string() << "\n";
		return 2;
	}

	if (parser.get_argument("info"))
	{
		// if path is a file, load that wrap file
		
		if (!std::filesystem::is_regular_file(path) || path.extension() != EXTENSION_WRAP)
		{
			std::cout << "Path is not a wrap file: " << path.string() << "\n";
			return 8;
		}

		Wrap wrap(path);

		std::vector<std::pair<String, String>> stats
		{
			{ "File: ", path.generic_string() },
			{ "Name: ", wrap.get_name() },
			{ "Version: ", std::to_string(wrap.get_content_version()) },
			{ "Entry Count: ", std::to_string(wrap.get_entry_count()) },
			{ "Base Path: ", wrap.get_base_path() },
			{ "Size: ", std::to_string(wrap.get_size()) + 'b' },
		};

		print_wrap_stats(path, stats);

		return 0;
	}

	if (!std::filesystem::is_directory(path))
	{
		std::cout << "Path is not a directory: " << path.string() << "\n";
		return 3;
	}

	String name;

	if (parser.get_argument("name", arg))
	{
		name = arg.args.front();
	}
	else
	{
		name = path.stem().string();
	}

	Path wrapPath = path.parent_path() / (name + EXTENSION_WRAP);

	// if updating, load the wrap
	Wrap* wrap = nullptr;

	if (parser.get_argument("update"))
	{
		wrap = new Wrap(wrapPath);
	}

	Path base;

	if (parser.get_argument("base", arg))
	{
		base = Path(arg.args.front());
	}
	else if (wrap)
	{
		base = wrap->get_base_path();
	}
	else
	{
		base = path;
	}

	uint32_t version = 0;

	if (parser.get_argument("version", arg))
	{
		if (!Parse::try_uint(arg.args.front(), version))
		{
			std::cerr << "Failed to parse the given version argument: " << arg.args.front() << "\n";
			return 4;
		}
	}
	else if (wrap)
	{
		// if updating, set version to wrap version + 1
		version = wrap->get_content_version() + 1;
	}

	CompressionLevel compression = CompressionLevel::Default;

	if (parser.get_argument("compression", arg))
	{
		int compressionLevel;

		if (!Parse::try_int(arg.args.front(), compressionLevel))
		{
			std::cerr << "Failed to parse the given compression argument: " << arg.args.front() << "\n";
			return 5;
		}
		else if (compressionLevel < 0 || compressionLevel > 9)
		{
			std::cerr << "The given compression level is out of range [0-9]: " << arg.args.front() << "\n";
			return 6;
		}

		compression = static_cast<CompressionLevel>(compressionLevel);
	}

	std::vector<Path> directoriesToCheck;
	std::vector<Path> paths;

	directoriesToCheck.push_back(path);

	// collect all files within the directory at the given path
	while (!directoriesToCheck.empty())
	{
		Path directory = directoriesToCheck.back();
		directoriesToCheck.pop_back();

		for (auto const& entry : std::filesystem::directory_iterator(directory))
		{
			if (entry.is_directory())
			{
				directoriesToCheck.push_back(entry.path());
			}
			else if (entry.is_regular_file())
			{
				paths.push_back(entry.path());
			}
		}
	}

	if (paths.size() > UINT32_MAX)
	{
		std::cerr << "Too many files to wrap.";
		return 7;
	}

	uint32_t count = static_cast<uint32_t>(paths.size());

	// add each file to it
	std::cout << "\nWrapping " << path.generic_string() << " into " << wrapPath.generic_string() << "\n";

	Stopwatch watch = Stopwatch::start_new();

	// create a wrap file using an input file that uses regex to determine how to compress each file type

	if (wrap != nullptr)
	{
		delete wrap;
	}

	wrap = new Wrap(wrapPath, name, count, base, version);

	for (auto const& p : paths)
	{
		std::cout << "  " << p.generic_string() << "\n";

		Path relativePath = p.lexically_relative(path);

		wrap->emplace(p, relativePath, compression);
	}

	watch.stop();

	size_t wrapSize = wrap->get_size();

	std::cout << '\n';

	std::vector<std::pair<String, String>> stats
	{
		{ "File: ", wrapPath.generic_string() },
		{ "Name: ", name },
		{ "Time Elapsed: ", std::to_string(watch.elapsed_s()) + 's' },
		{ "Version: ", std::to_string(version) },
		{ "Entry Count: ", std::to_string(count) },
		{ "Base Path: ", base.generic_string() },
		{ "Size: ", std::to_string(wrapSize) + 'b' },
	};

	print_wrap_stats(wrapPath, stats);

	delete wrap;

	return 0;
}
