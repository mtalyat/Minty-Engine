#include "FileWatcher.h"

using namespace Minty;
using namespace Mintye;

Mintye::FileWatcher::FileWatcher(Minty::Path const& directory, OnChangeFunc const& action)
	: m_directory(directory)
	, m_action(action)
{}

void Mintye::FileWatcher::update()
{
	// check for deletions
	auto it = m_files.begin();
	while (it != m_files.end())
	{
		if (!std::filesystem::exists(it->first))
		{
			m_action(it->first, FileStatus::Deleted);
			it = m_files.erase(it);
		}
		else
		{
			it++;
		}
	}

	// check for creations/modifications
	for (auto const& entry : std::filesystem::recursive_directory_iterator(m_directory))
	{
		Path path = std::filesystem::absolute(entry.path());

		auto lastWriteTime = std::filesystem::last_write_time(path);

		if (!m_files.contains(path))
		{
			m_files.emplace(path, FileData
				{
					.lastModification = lastWriteTime
				});
			m_action(path, FileStatus::Created);
		}
		else if(m_files.at(path).lastModification != lastWriteTime)
		{
			m_files.at(path).lastModification = lastWriteTime;
			m_action(path, FileStatus::Modified);
		}
	}
}