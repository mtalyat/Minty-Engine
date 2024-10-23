#pragma once

#include "Minty/Data/Container.h"
#include "Minty/Component/ScriptObjectComponent.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/String.h"
#include "Minty/Script/ScriptArguments.h"
#include "Minty/Script/ScriptField.h"
#include "Minty/Script/ScriptMethod.h"
#include "Minty/Script/ScriptProperty.h"
#include <unordered_map>

namespace Minty
{
	class ScriptClass;

	struct ScriptObjectBuilder
	{
		UUID id = {};
		Ref<ScriptClass> klass;
	};

	/// <summary>
	/// Represents an instance of a ScriptClass.
	/// </summary>
	class ScriptObject
		: public ScriptObjectComponent, public Source<ScriptObject>
	{
	private:
		Ref<ScriptClass> m_class;

	protected:
		std::unordered_map<String, Owner<ScriptMethod>> m_methods;
		std::unordered_map<String, Owner<ScriptField>> m_fields;
		std::unordered_map<String, Owner<ScriptProperty>> m_properties;

	public:
		ScriptObject(ScriptObjectBuilder const& builder);

	public:
		virtual ~ScriptObject() = default;

	public:
		virtual void* get_native() const = 0;

		Ref<ScriptClass> get_class() const { return m_class; }

		Ref<ScriptField> get_field(String const& name) const;

		std::vector<Ref<ScriptField>> get_fields() const;

		Ref<ScriptProperty> get_property(String const& name) const;

		std::vector<Ref<ScriptProperty>> get_properties() const;

		Ref<ScriptMethod> get_method(String const& name) const;

		std::vector<Ref<ScriptMethod>> get_methods() const;

		void invoke(String const& name) const;

		void invoke(String const& name, void** const argv, Size const argc) const;

		Bool try_invoke(String const& name) const;

		Bool try_invoke(String const& name, void** const argv, Size const argc) const;

		void set_field(String const& name, void* const value) const;

		void get_field(String const& name, void* const value) const;

		void set_property(String const& name, void** const value) const;

		void get_property(String const& name, void** const value) const;

	protected:
		virtual void initialize_fields(std::unordered_map<String, Owner<ScriptField>>& fields) = 0;
		
		virtual void initialize_properties(std::unordered_map<String, Owner<ScriptProperty>>& properties) = 0;

		virtual void initialize_methods(std::unordered_map<String, Owner<ScriptMethod>>& methods) = 0;

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;

	public:
		static Owner<ScriptObject> create(ScriptObjectBuilder const& builder);

		static Owner<ScriptObject> create(ScriptObjectBuilder const& builder, ScriptArguments& args);
	};
}