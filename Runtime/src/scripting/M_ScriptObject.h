#pragma once
#include "components/M_Component.h"

#include "types/M_UUID.h"

struct _MonoObject;
typedef struct _MonoObject MonoObject;
struct _MonoClassField;
typedef struct _MonoClassField MonoClassField;

namespace minty
{
	class ScriptClass;
	class ScriptArguments;

	class ScriptObject
		: public Component
	{
		friend class ScriptEngine;

	private:
		ScriptClass const* _script;
		MonoObject* _object;
		uint32_t _handle; // GC handle

	public:
		ScriptObject(UUID const id, ScriptClass const& script);

		ScriptObject(UUID const id, ScriptClass const& script, ScriptArguments& arguments);

		~ScriptObject();

		void destroy();

		void invoke(String const& name) const;

		void invoke(String const& name, ScriptArguments& arguments) const;

		bool try_invoke(String const& name) const;

		bool try_invoke(String const& name, ScriptArguments& arguments) const;

		ScriptClass const& get_class() const;
		
		void set_field(String const& name, void* const value) const;

		void get_field(String const& name, void* const value) const;

		void set_property(String const& name, void* const value) const;

		void get_property(String const& name, void* const value) const;

		MonoObject* data() const;

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

	private:
		void serialize_field(Writer& writer, String const& name, MonoClassField* const field, MonoObject* const object) const;
		void deserialize_field(Reader const& reader, String const& name, MonoClassField* const field, MonoObject* const object);
	};
}