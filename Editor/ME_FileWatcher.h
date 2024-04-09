#pragma once
#include "ME_Minty.h"

#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace mintye
{
	class FileWatcher
		: public minty::Object
	{
	public:
		enum class FileStatus
		{
			Created,
			Modified,
			Deleted,
		};

		typedef std::function<void(minty::Path, FileStatus)> OnChangeFunc;

	private:
		struct FileData
		{
			std::filesystem::file_time_type lastModification;
		};

	private:
		minty::Path _directory;
		OnChangeFunc _action;
		std::unordered_map<minty::Path, FileData> _files;
		std::unordered_set<minty::Path> _changed;

	public:
		FileWatcher(minty::Path const& directory, OnChangeFunc const& action);

		/// <summary>
		/// Checks the directory for changes.
		/// </summary>
		void update();

	public:
		void serialize(minty::Writer& writer) const override;
		void deserialize(minty::Reader const& reader) override;
	};
}