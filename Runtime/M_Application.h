#pragma once
#include "M_Object.h"

#include "M_Info.h"

namespace minty
{
	struct RuntimeBuilder;
	class Runtime;
	class Window;

	class Application
		: public Object
	{
	private:
		Info _info;
		Path _path;
		Window* _window;
		Runtime* _runtime;

	public:
		Application(Info const& info, Path const& path);

		virtual ~Application();

	protected:
		virtual void create();

		virtual void init(RuntimeBuilder* builder = nullptr);

		virtual void start();

		virtual void loop();

		virtual void stop();

		virtual void destroy();

	public:
		virtual int run();

		int get_exit_code() const;

	protected:
		virtual Window* create_window();

		virtual Runtime* create_runtime();

	public:
		void load_assemblies(std::vector<Path> const& paths);

		Window& get_window() const;

		Runtime& get_runtime() const;

		Path const& get_path() const;

		Info const& get_info() const;
	};
}