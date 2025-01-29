using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    internal enum Type
    {
        Undefined,
		Bool,
		Bool2,
		Bool3,
		Bool4,
		Char,
		Byte,
		Short,
		UShort,
		Int,
		Int2,
		Int3,
		Int4,
		UInt,
		UInt2,
		UInt3,
		UInt4,
		Long,
		ULong,
		Size,
		Float,
		Float2,
		Float3,
		Float4,
		Double,
		String,
		MultilineString,
		Matrix2,
		Matrix3,
		Matrix4,
		Quaternion,
		Object,
		UUID,
    }

	internal static class TypeExtensions
	{
		internal static Type ToMintyType(this System.Type type)
		{
			// TODO: finish this for all Types above, don't forget the C++ side
			if (type == null)
			{
				return Type.Undefined;
			}
			else if (type == typeof(int))
			{
				return Type.Int;
            }
            else if (type == typeof(string))
            {
                return Type.String;
            }
            else if (type == typeof(UUID))
			{
				return Type.UUID;
			}
			else if (type.IsSubclassOf(typeof(object)))
			{
				return Type.Object;
			}
			else
			{
				Debug.Error($"Unknown System.Type to MintyEngine.Type conversion for \"{type}\".");
				return Type.Undefined;
			}
		}
	}
}
