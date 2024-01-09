#pragma once

namespace minty
{
	/// <summary>
	/// Determines when to compress data being stored in a wrap file.
	/// </summary>
	enum class CompressionLevel : int
	{
		Level0 = 0,
		Level1 = 1,
		Level2 = 2,
		Level3 = 3,
		Level4 = 4,
		Level5 = 5,
		Level6 = 6,
		Level7 = 7,
		Level8 = 8,
		Level9 = 9,

		/// <summary>
		/// Do not compress the data.
		/// </summary>
		None = Level0,

		/// <summary>
		/// Lightly compresses the data, quickly.
		/// </summary>
		Fast = Level1,
		/// <summary>
		/// Heavily compresses the data, slowly.
		/// </summary>
		Slow = Level9,

		/// <summary>
		/// Lightly compress the data.
		/// </summary>
		Low = Level1,
		/// <summary>
		/// Heavily compress the data.
		/// </summary>
		High = Level9,

		/// <summary>
		/// Default compression.
		/// </summary>
		Default = Level6,
	};

	unsigned long compress_bound(unsigned long const sourceSize);

	int compress(void* const destination, unsigned long& destinationSize, void* const source, unsigned long const sourceSize, CompressionLevel const level = CompressionLevel::Default);

	int uncompress(void* const destination, unsigned long& destinationSize, void* const source, unsigned long& sourceSize);
}