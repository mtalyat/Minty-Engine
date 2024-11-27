#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Audio/Attenuation.h"
#include "Minty/Core/Color.h"
#include "Minty/Core/Encoding.h"
#include "Minty/Core/Macros.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include "Minty/Data/ConstantContainer.h"
#include "Minty/Data/DynamicContainer.h"
#include "Minty/Data/StaticContainer.h"
#include "Minty/FSM/Conditional.h"
#include "Minty/Render/CoordinateMode.h"
#include "Minty/Render/Format.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Render/Perspective.h"
#include "Minty/Render/Shader.h"
#include "Minty/Render/Space.h"
#include "Minty/Serialize/Node.h"
#include "Minty/UI/AnchorMode.h"

namespace Minty
{
	template<typename T>
	String to_string(T const& obj)
	{
		return std::to_string(obj);
	}

	template<>
	inline String to_string(String const& obj)
	{
		return obj;
	}

	String to_string(Float const obj);

	String to_string(Double const obj);

	String to_string(Bool const obj);

	String to_string(Char const obj);

	String to_string(UUID const obj);

	String to_string(Type const obj);

	String to_string(TypeID const& obj);

	String to_string(TypeInfo const& obj);

	String to_string(Bool2 const& obj);

	String to_string(Bool3 const& obj);

	String to_string(Bool4 const& obj);

	String to_string(Int2 const& obj);

	String to_string(Int3 const& obj);

	String to_string(Int4 const& obj);

	String to_string(UInt2 const& obj);

	String to_string(UInt3 const& obj);

	String to_string(UInt4 const& obj);

	String to_string(Float2 const& obj);

	String to_string(Float3 const& obj);

	String to_string(Float4 const& obj);

	String to_string(ShaderStage const obj);

	String to_string(ShaderInputType const obj);

	String to_string(Format const obj);

	String to_string(ShaderPrimitiveTopology const obj);

	String to_string(ShaderCullMode const obj);

	String to_string(ShaderFrontFace const obj);

	String to_string(ShaderPolygonMode const obj);

	String to_string(AssetType const obj);

	String to_string(Path const& obj);

	String to_string(MeshType const obj);

	String to_string(AnchorMode const obj);

	String to_string(Perspective const obj);

	String to_string(Color const obj);

	String to_string(Conditional const obj);

	String to_string(Attenuation const obj);

	String to_string(ShaderInputRate const obj);

	String to_string(CoordinateMode const obj);

	String to_string(Space const obj);

	String to_string(Node const& obj);

	String to_string(Container const& obj);

	String to_string(ConstantContainer const& obj);

	String to_string(DynamicContainer const& obj);

	String to_string(StaticContainer const& obj);
}