#include "ME_FileWatcher.h"

using namespace minty;
using namespace mintye;

mintye::FileWatcher::FileWatcher(minty::Path const& directory, OnChangeFunc const& action)
	: _directory(directory)
	, _action(action)
{}

void mintye::FileWatcher::update()
{
	// check for deletions
	auto it = _files.begin();
	while (it != _files.end())
	{
		if (!std::filesystem::exists(it->first))
		{
			_action(it->first, FileStatus::Deleted);
			it = _files.erase(it);
		}
		else
		{
			it++;
		}
	}

	// check for creations/modifications
	for (auto const& entry : std::filesystem::recursive_directory_iterator(_directory))
	{
		Path path = std::filesystem::absolute(entry.path());

		auto lastWriteTime = std::filesystem::last_write_time(path);

		if (!_files.contains(path))
		{
			_files.emplace(path, FileData
				{
					.lastModification = lastWriteTime
				});
			_action(path, FileStatus::Created);
		}
		else if(_files.at(path).lastModification != lastWriteTime)
		{
			_files.at(path).lastModification = lastWriteTime;
			_action(path, FileStatus::Modified);
		}
	}
}

void mintye::FileWatcher::serialize(minty::Writer& writer) const
{
	
}

void mintye::FileWatcher::deserialize(minty::Reader const& reader)
{

}