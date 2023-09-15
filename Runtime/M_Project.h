#pragma once

#include <string>
#include <filesystem>

namespace minty
{
	class Project
	{
	private:
		std::filesystem::path const _base;

	public:
		Project(std::string const& path);

		std::string const getBasePath() const;

		std::string const getAssetsPath() const;

		std::string const getBuildPath() const;
	};
}