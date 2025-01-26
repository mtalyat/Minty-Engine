using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
	/// The format for how pixels are read.
	/// </summary>
	public enum ImagePixelFormat
    {
        /// <summary>
        /// No format.
        /// </summary>
        None = 0,
		/// <summary>
		/// Gray channel only.
		/// </summary>
		Gray = 1,
		/// <summary>
		/// Gray and alpha channels.
		/// </summary>
		GrayAlpha = 2,
		/// <summary>
		/// Red, green and blue channels.
		/// </summary>
		RedGreenBlue = 3,
		/// <summary>
		/// Red, green, blue and alpha channels.
		/// </summary>
		RedGreenBlueAlpha = 4,
	};
}
