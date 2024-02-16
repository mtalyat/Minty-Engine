#pragma once
#include "M_Component.h"

struct _MonoObject;
typedef struct _MonoObject MonoObject;
struct _MonoClassField;
typedef struct _MonoClassField MonoClassField;

namespace minty
{
	class Script;

	class ScriptObject
		: public Component
	{
	private:
		Script const* _script;
		MonoObject* _object;

	public:
		ScriptObject(Script const& script);

		~ScriptObject();

		void invoke(String const& name) const;
		
	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	private:
		void serialize_field(Writer& writer, String const& name, MonoClassField* const field) const;
		void deserialize_field(Reader const& reader, String const& name, MonoClassField* const field);
	};
}