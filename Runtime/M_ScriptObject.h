#pragma once
#include "M_Object.h"

#include "M_Mono.h"

namespace minty
{
	class Assembly;
}

namespace minty::Scripting
{
	class Script;

	class ScriptObject
		: public Object
	{
	private:
		Script* _script;
		Assembly* _assembly;
		MonoClass* _class;
		MonoObject* _object;

	public:

		
	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	private:
		void serialize_field(Writer& writer, String const& name, MonoClassField* const field) const;
		void deserialize_field(Reader const& reader, String const& name, MonoClassField* const field);
	};
}