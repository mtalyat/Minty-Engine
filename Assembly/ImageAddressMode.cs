using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public enum ImageAddressMode
    {
        Undefined = 0,
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
		MirroredClampToEdge,
	};
}
