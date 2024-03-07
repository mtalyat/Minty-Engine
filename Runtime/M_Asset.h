#pragma once
#include "M_RuntimeObject.h"

#include "M_Node.h"
#include "M_Dynamic.h"
#include "M_UUID.h"

#include <filesystem>
#include <array>

namespace minty
{
	class Asset
		: public RuntimeObject
	{
	private:
		UUID _id;
		Path _path;

	public:
		Asset();

		Asset(UUID const id, Path const& path, Runtime& runtime);

		virtual ~Asset();

		UUID get_id() const;

		Path const& get_path() const;

		virtual String get_name() const;

		friend bool operator==(Asset const& left, Asset const& right);
		friend bool operator!=(Asset const& left, Asset const& right);

#pragma region Static

		/// <summary>
		/// Gets the meta path of the given asset path.
		/// </summary>
		/// <param name="assetPath"></param>
		/// <returns></returns>
		static Path get_meta_path(Path const& assetPath);

#pragma endregion
	};
}