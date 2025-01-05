#pragma once

#include "Minty/Serialize/Serializable.h"

namespace Minty
{
	/// <summary>
	/// Base class for all components.
	/// </summary>
	struct Component 
		: public Serializable
	{
		virtual void serialize(Writer& writer) const override {}
		virtual void deserialize(Reader& reader) override {}
	};
}