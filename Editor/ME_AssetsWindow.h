#pragma once
#include "ME_EditorWindow.h"

#include <vector>

namespace mintye
{
	class AssetsWindow
		: public EditorWindow
	{
	private:
		minty::Path _path;

		std::vector<minty::Path> _directories;
		std::vector<minty::Path> _files;

	public:
		AssetsWindow(Application& application);

		void set_project(Project* const project) override;

		void draw() override;

		void reset() override;

	private:
		void set_path(minty::Path const& path);
	};
}