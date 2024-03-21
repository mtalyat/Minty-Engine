#pragma once
#include "ME_EditorWindow.h"

#include <vector>

namespace mintye
{
	class AssetsWindow
		: public EditorWindow
	{
	private:
		struct FileData
		{
			minty::Path path;
			bool canIncludeInScene;
			bool includedInScene;
		};

	private:
		minty::Path _path;

		std::vector<minty::Path> _directories;
		std::vector<FileData> _files;

	public:
		AssetsWindow(EditorApplication& application);

		void set_project(Project* const project) override;

		void draw() override;

		void reset() override;

		void refresh() override;

	private:
		minty::Path get_path(minty::Path const& file) const;

		void set_path(minty::Path const& path);
	};
}