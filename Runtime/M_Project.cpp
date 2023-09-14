#include "pch.h"
#include "M_Project.h"

using namespace minty;

Project::Project(std::string const& path)
	: _base(std::filesystem::absolute(path))
{

}

std::string const Project::getBasePath() const
{
	return _base.string();
}

std::string const Project::getAssetsPath() const
{
	return (_base / "Assets").string();
}

std::string const Project::getBuildPath() const
{
	return (_base / "Build").string();
}
