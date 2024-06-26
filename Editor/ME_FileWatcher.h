#pragma once
#include "ME_Minty.h"

#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace Mintye
{
	class FileWatcher
		: public Minty::Object
	{
	public:
		enum class FileStatus
		{
			Created,
			Modified,
			Deleted,
		};

		typedef std::function<void(Minty::Path const&, FileStatus const)> OnChangeFunc;

	private:
		struct FileData
		{
			std::filesystem::file_time_type lastModification;
		};

	private:
		Minty::Path _directory;
		OnChangeFunc _action;
		std::unordered_map<Minty::Path, FileData> _files;
		std::unordered_set<Minty::Path> _changed;

	public:
		FileWatcher(Minty::Path const& directory, OnChangeFunc const& action);

		/// <summary>
		/// Checks the directory for changes.
		/// </summary>
		void update();

	public:
		void serialize(Minty::Writer& writer) const override;
		void deserialize(Minty::Reader const& reader) override;
	};
}