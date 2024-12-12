#pragma once
#include "ME_EditorWindow.h"

#include <vector>

namespace Mintye
{
	class AssetsWindow
		: public EditorWindow
	{
	private:
		struct FileData
		{
			Minty::Path path;
			bool canIncludeInScene;
			bool includedInScene;
		};

	private:
		Minty::Path m_path;

		std::vector<Minty::Path> m_directories;
		std::vector<FileData> m_files;

	public:
		AssetsWindow(EditorApplication& application);

		void set_project(Project* const project) override;

		void draw() override;

		void reset() override;

		void refresh() override;

	private:
		Minty::Path get_path(Minty::Path const& file) const;

		void set_path(Minty::Path const& path);
	};
}