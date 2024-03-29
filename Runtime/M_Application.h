#pragma once
#include "M_Object.h"

#include "M_RunMode.h"
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
		RunMode _mode;
		Window* _window;
		Runtime* _runtime;

	public:
		Application(RunMode const mode = RunMode::Normal);

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

	protected:
		virtual Window* create_window();

		virtual Runtime* create_runtime();

	public:
		void load_assemblies(std::vector<Path> const& paths);

		RunMode get_mode() const;

		Window& get_window() const;

		Runtime& get_runtime() const;

		Info const& get_info() const;
	};
}