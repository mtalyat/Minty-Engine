#include "pch.h"
#include "vulkan.h"

#include "M_Error.h"

using namespace minty;
using namespace minty::vk;

String minty::vk::to_string(VkResult const result)
{
	switch (result)
	{
	case VK_SUCCESS:
		return "VK_SUCCESS";
	case VK_NOT_READY:
		return "VK_NOT_READY";
	case VK_TIMEOUT:
		return "VK_TIMEOUT";
	case VK_EVENT_SET:
		return "VK_EVENT_SET";
	case VK_EVENT_RESET:
		return "VK_EVENT_RESET";
	case VK_INCOMPLETE:
		return "VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:
		return "VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_FRAGMENTED_POOL:
		return "VK_ERROR_FRAGMENTED_POOL";
	case VK_ERROR_UNKNOWN:
		return "VK_ERROR_UNKNOWN";
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return "VK_ERROR_OUT_OF_POOL_MEMORY";
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
	case VK_ERROR_FRAGMENTATION:
		return "VK_ERROR_FRAGMENTATION";
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
		return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
	case VK_PIPELINE_COMPILE_REQUIRED:
		return "VK_PIPELINE_COMPILE_REQUIRED";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "VK_ERROR_SURFACE_LOST_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_SUBOPTIMAL_KHR:
		return "VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_ERROR_INVALID_SHADER_NV:
		return "VK_ERROR_INVALID_SHADER_NV";
	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
		return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
	case VK_ERROR_NOT_PERMITTED_KHR:
		return "VK_ERROR_NOT_PERMITTED_KHR";
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
		return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
	case VK_THREAD_IDLE_KHR:
		return "VK_THREAD_IDLE_KHR";
	case VK_THREAD_DONE_KHR:
		return "VK_THREAD_DONE_KHR";
	case VK_OPERATION_DEFERRED_KHR:
		return "VK_OPERATION_DEFERRED_KHR";
	case VK_OPERATION_NOT_DEFERRED_KHR:
		return "VK_OPERATION_NOT_DEFERRED_KHR";
	default:
		return "Unknown";
	}
}

String minty::vk::to_string(VkFilter const value)
{
	switch (value)
	{
	case VkFilter::VK_FILTER_NEAREST: return "NEAREST";
	case VkFilter::VK_FILTER_LINEAR: return "LINEAR";
	case VkFilter::VK_FILTER_CUBIC_IMG: return "CUBIC_IMG";
	default: return "";
	}
}

VkFilter minty::vk::from_string_vk_filter(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "NEAREST") return VkFilter::VK_FILTER_NEAREST;
	if (value2 == "LINEAR") return VkFilter::VK_FILTER_LINEAR;
	if (value2 == "CUBIC_IMG") return VkFilter::VK_FILTER_CUBIC_IMG;

	return static_cast<VkFilter>(0);
}

String minty::vk::to_string(VkFormat const value)
{
	switch (value)
	{
	case VkFormat::VK_FORMAT_UNDEFINED: return "UNDEFINED";
	case VkFormat::VK_FORMAT_R4G4_UNORM_PACK8: return "R4G4_UNORM_PACK8";
	case VkFormat::VK_FORMAT_R4G4B4A4_UNORM_PACK16: return "R4G4B4A4_UNORM_PACK16";
	case VkFormat::VK_FORMAT_B4G4R4A4_UNORM_PACK16: return "B4G4R4A4_UNORM_PACK16";
	case VkFormat::VK_FORMAT_R5G6B5_UNORM_PACK16: return "R5G6B5_UNORM_PACK16";
	case VkFormat::VK_FORMAT_B5G6R5_UNORM_PACK16: return "B5G6R5_UNORM_PACK16";
	case VkFormat::VK_FORMAT_R5G5B5A1_UNORM_PACK16: return "R5G5B5A1_UNORM_PACK16";
	case VkFormat::VK_FORMAT_B5G5R5A1_UNORM_PACK16: return "B5G5R5A1_UNORM_PACK16";
	case VkFormat::VK_FORMAT_A1R5G5B5_UNORM_PACK16: return "A1R5G5B5_UNORM_PACK16";
	case VkFormat::VK_FORMAT_R8_UNORM: return "R8_UNORM";
	case VkFormat::VK_FORMAT_R8_SNORM: return "R8_SNORM";
	case VkFormat::VK_FORMAT_R8_USCALED: return "R8_USCALED";
	case VkFormat::VK_FORMAT_R8_SSCALED: return "R8_SSCALED";
	case VkFormat::VK_FORMAT_R8_UINT: return "R8_UINT";
	case VkFormat::VK_FORMAT_R8_SINT: return "R8_SINT";
	case VkFormat::VK_FORMAT_R8_SRGB: return "R8_SRGB";
	case VkFormat::VK_FORMAT_R8G8_UNORM: return "R8G8_UNORM";
	case VkFormat::VK_FORMAT_R8G8_SNORM: return "R8G8_SNORM";
	case VkFormat::VK_FORMAT_R8G8_USCALED: return "R8G8_USCALED";
	case VkFormat::VK_FORMAT_R8G8_SSCALED: return "R8G8_SSCALED";
	case VkFormat::VK_FORMAT_R8G8_UINT: return "R8G8_UINT";
	case VkFormat::VK_FORMAT_R8G8_SINT: return "R8G8_SINT";
	case VkFormat::VK_FORMAT_R8G8_SRGB: return "R8G8_SRGB";
	case VkFormat::VK_FORMAT_R8G8B8_UNORM: return "R8G8B8_UNORM";
	case VkFormat::VK_FORMAT_R8G8B8_SNORM: return "R8G8B8_SNORM";
	case VkFormat::VK_FORMAT_R8G8B8_USCALED: return "R8G8B8_USCALED";
	case VkFormat::VK_FORMAT_R8G8B8_SSCALED: return "R8G8B8_SSCALED";
	case VkFormat::VK_FORMAT_R8G8B8_UINT: return "R8G8B8_UINT";
	case VkFormat::VK_FORMAT_R8G8B8_SINT: return "R8G8B8_SINT";
	case VkFormat::VK_FORMAT_R8G8B8_SRGB: return "R8G8B8_SRGB";
	case VkFormat::VK_FORMAT_B8G8R8_UNORM: return "B8G8R8_UNORM";
	case VkFormat::VK_FORMAT_B8G8R8_SNORM: return "B8G8R8_SNORM";
	case VkFormat::VK_FORMAT_B8G8R8_USCALED: return "B8G8R8_USCALED";
	case VkFormat::VK_FORMAT_B8G8R8_SSCALED: return "B8G8R8_SSCALED";
	case VkFormat::VK_FORMAT_B8G8R8_UINT: return "B8G8R8_UINT";
	case VkFormat::VK_FORMAT_B8G8R8_SINT: return "B8G8R8_SINT";
	case VkFormat::VK_FORMAT_B8G8R8_SRGB: return "B8G8R8_SRGB";
	case VkFormat::VK_FORMAT_R8G8B8A8_UNORM: return "R8G8B8A8_UNORM";
	case VkFormat::VK_FORMAT_R8G8B8A8_SNORM: return "R8G8B8A8_SNORM";
	case VkFormat::VK_FORMAT_R8G8B8A8_USCALED: return "R8G8B8A8_USCALED";
	case VkFormat::VK_FORMAT_R8G8B8A8_SSCALED: return "R8G8B8A8_SSCALED";
	case VkFormat::VK_FORMAT_R8G8B8A8_UINT: return "R8G8B8A8_UINT";
	case VkFormat::VK_FORMAT_R8G8B8A8_SINT: return "R8G8B8A8_SINT";
	case VkFormat::VK_FORMAT_R8G8B8A8_SRGB: return "R8G8B8A8_SRGB";
	case VkFormat::VK_FORMAT_B8G8R8A8_UNORM: return "B8G8R8A8_UNORM";
	case VkFormat::VK_FORMAT_B8G8R8A8_SNORM: return "B8G8R8A8_SNORM";
	case VkFormat::VK_FORMAT_B8G8R8A8_USCALED: return "B8G8R8A8_USCALED";
	case VkFormat::VK_FORMAT_B8G8R8A8_SSCALED: return "B8G8R8A8_SSCALED";
	case VkFormat::VK_FORMAT_B8G8R8A8_UINT: return "B8G8R8A8_UINT";
	case VkFormat::VK_FORMAT_B8G8R8A8_SINT: return "B8G8R8A8_SINT";
	case VkFormat::VK_FORMAT_B8G8R8A8_SRGB: return "B8G8R8A8_SRGB";
	case VkFormat::VK_FORMAT_A8B8G8R8_UNORM_PACK32: return "A8B8G8R8_UNORM_PACK32";
	case VkFormat::VK_FORMAT_A8B8G8R8_SNORM_PACK32: return "A8B8G8R8_SNORM_PACK32";
	case VkFormat::VK_FORMAT_A8B8G8R8_USCALED_PACK32: return "A8B8G8R8_USCALED_PACK32";
	case VkFormat::VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return "A8B8G8R8_SSCALED_PACK32";
	case VkFormat::VK_FORMAT_A8B8G8R8_UINT_PACK32: return "A8B8G8R8_UINT_PACK32";
	case VkFormat::VK_FORMAT_A8B8G8R8_SINT_PACK32: return "A8B8G8R8_SINT_PACK32";
	case VkFormat::VK_FORMAT_A8B8G8R8_SRGB_PACK32: return "A8B8G8R8_SRGB_PACK32";
	case VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32: return "A2R10G10B10_UNORM_PACK32";
	case VkFormat::VK_FORMAT_A2R10G10B10_SNORM_PACK32: return "A2R10G10B10_SNORM_PACK32";
	case VkFormat::VK_FORMAT_A2R10G10B10_USCALED_PACK32: return "A2R10G10B10_USCALED_PACK32";
	case VkFormat::VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return "A2R10G10B10_SSCALED_PACK32";
	case VkFormat::VK_FORMAT_A2R10G10B10_UINT_PACK32: return "A2R10G10B10_UINT_PACK32";
	case VkFormat::VK_FORMAT_A2R10G10B10_SINT_PACK32: return "A2R10G10B10_SINT_PACK32";
	case VkFormat::VK_FORMAT_A2B10G10R10_UNORM_PACK32: return "A2B10G10R10_UNORM_PACK32";
	case VkFormat::VK_FORMAT_A2B10G10R10_SNORM_PACK32: return "A2B10G10R10_SNORM_PACK32";
	case VkFormat::VK_FORMAT_A2B10G10R10_USCALED_PACK32: return "A2B10G10R10_USCALED_PACK32";
	case VkFormat::VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return "A2B10G10R10_SSCALED_PACK32";
	case VkFormat::VK_FORMAT_A2B10G10R10_UINT_PACK32: return "A2B10G10R10_UINT_PACK32";
	case VkFormat::VK_FORMAT_A2B10G10R10_SINT_PACK32: return "A2B10G10R10_SINT_PACK32";
	case VkFormat::VK_FORMAT_R16_UNORM: return "R16_UNORM";
	case VkFormat::VK_FORMAT_R16_SNORM: return "R16_SNORM";
	case VkFormat::VK_FORMAT_R16_USCALED: return "R16_USCALED";
	case VkFormat::VK_FORMAT_R16_SSCALED: return "R16_SSCALED";
	case VkFormat::VK_FORMAT_R16_UINT: return "R16_UINT";
	case VkFormat::VK_FORMAT_R16_SINT: return "R16_SINT";
	case VkFormat::VK_FORMAT_R16_SFLOAT: return "R16_SFLOAT";
	case VkFormat::VK_FORMAT_R16G16_UNORM: return "R16G16_UNORM";
	case VkFormat::VK_FORMAT_R16G16_SNORM: return "R16G16_SNORM";
	case VkFormat::VK_FORMAT_R16G16_USCALED: return "R16G16_USCALED";
	case VkFormat::VK_FORMAT_R16G16_SSCALED: return "R16G16_SSCALED";
	case VkFormat::VK_FORMAT_R16G16_UINT: return "R16G16_UINT";
	case VkFormat::VK_FORMAT_R16G16_SINT: return "R16G16_SINT";
	case VkFormat::VK_FORMAT_R16G16_SFLOAT: return "R16G16_SFLOAT";
	case VkFormat::VK_FORMAT_R16G16B16_UNORM: return "R16G16B16_UNORM";
	case VkFormat::VK_FORMAT_R16G16B16_SNORM: return "R16G16B16_SNORM";
	case VkFormat::VK_FORMAT_R16G16B16_USCALED: return "R16G16B16_USCALED";
	case VkFormat::VK_FORMAT_R16G16B16_SSCALED: return "R16G16B16_SSCALED";
	case VkFormat::VK_FORMAT_R16G16B16_UINT: return "R16G16B16_UINT";
	case VkFormat::VK_FORMAT_R16G16B16_SINT: return "R16G16B16_SINT";
	case VkFormat::VK_FORMAT_R16G16B16_SFLOAT: return "R16G16B16_SFLOAT";
	case VkFormat::VK_FORMAT_R16G16B16A16_UNORM: return "R16G16B16A16_UNORM";
	case VkFormat::VK_FORMAT_R16G16B16A16_SNORM: return "R16G16B16A16_SNORM";
	case VkFormat::VK_FORMAT_R16G16B16A16_USCALED: return "R16G16B16A16_USCALED";
	case VkFormat::VK_FORMAT_R16G16B16A16_SSCALED: return "R16G16B16A16_SSCALED";
	case VkFormat::VK_FORMAT_R16G16B16A16_UINT: return "R16G16B16A16_UINT";
	case VkFormat::VK_FORMAT_R16G16B16A16_SINT: return "R16G16B16A16_SINT";
	case VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT: return "R16G16B16A16_SFLOAT";
	case VkFormat::VK_FORMAT_R32_UINT: return "R32_UINT";
	case VkFormat::VK_FORMAT_R32_SINT: return "R32_SINT";
	case VkFormat::VK_FORMAT_R32_SFLOAT: return "R32_SFLOAT";
	case VkFormat::VK_FORMAT_R32G32_UINT: return "R32G32_UINT";
	case VkFormat::VK_FORMAT_R32G32_SINT: return "R32G32_SINT";
	case VkFormat::VK_FORMAT_R32G32_SFLOAT: return "R32G32_SFLOAT";
	case VkFormat::VK_FORMAT_R32G32B32_UINT: return "R32G32B32_UINT";
	case VkFormat::VK_FORMAT_R32G32B32_SINT: return "R32G32B32_SINT";
	case VkFormat::VK_FORMAT_R32G32B32_SFLOAT: return "R32G32B32_SFLOAT";
	case VkFormat::VK_FORMAT_R32G32B32A32_UINT: return "R32G32B32A32_UINT";
	case VkFormat::VK_FORMAT_R32G32B32A32_SINT: return "R32G32B32A32_SINT";
	case VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT: return "R32G32B32A32_SFLOAT";
	case VkFormat::VK_FORMAT_R64_UINT: return "R64_UINT";
	case VkFormat::VK_FORMAT_R64_SINT: return "R64_SINT";
	case VkFormat::VK_FORMAT_R64_SFLOAT: return "R64_SFLOAT";
	case VkFormat::VK_FORMAT_R64G64_UINT: return "R64G64_UINT";
	case VkFormat::VK_FORMAT_R64G64_SINT: return "R64G64_SINT";
	case VkFormat::VK_FORMAT_R64G64_SFLOAT: return "R64G64_SFLOAT";
	case VkFormat::VK_FORMAT_R64G64B64_UINT: return "R64G64B64_UINT";
	case VkFormat::VK_FORMAT_R64G64B64_SINT: return "R64G64B64_SINT";
	case VkFormat::VK_FORMAT_R64G64B64_SFLOAT: return "R64G64B64_SFLOAT";
	case VkFormat::VK_FORMAT_R64G64B64A64_UINT: return "R64G64B64A64_UINT";
	case VkFormat::VK_FORMAT_R64G64B64A64_SINT: return "R64G64B64A64_SINT";
	case VkFormat::VK_FORMAT_R64G64B64A64_SFLOAT: return "R64G64B64A64_SFLOAT";
	case VkFormat::VK_FORMAT_B10G11R11_UFLOAT_PACK32: return "B10G11R11_UFLOAT_PACK32";
	case VkFormat::VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return "E5B9G9R9_UFLOAT_PACK32";
	case VkFormat::VK_FORMAT_D16_UNORM: return "D16_UNORM";
	case VkFormat::VK_FORMAT_X8_D24_UNORM_PACK32: return "X8_D24_UNORM_PACK32";
	case VkFormat::VK_FORMAT_D32_SFLOAT: return "D32_SFLOAT";
	case VkFormat::VK_FORMAT_S8_UINT: return "S8_UINT";
	case VkFormat::VK_FORMAT_D16_UNORM_S8_UINT: return "D16_UNORM_S8_UINT";
	case VkFormat::VK_FORMAT_D24_UNORM_S8_UINT: return "D24_UNORM_S8_UINT";
	case VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT: return "D32_SFLOAT_S8_UINT";
	case VkFormat::VK_FORMAT_BC1_RGB_UNORM_BLOCK: return "BC1_RGB_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC1_RGB_SRGB_BLOCK: return "BC1_RGB_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return "BC1_RGBA_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return "BC1_RGBA_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_BC2_UNORM_BLOCK: return "BC2_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC2_SRGB_BLOCK: return "BC2_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_BC3_UNORM_BLOCK: return "BC3_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC3_SRGB_BLOCK: return "BC3_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_BC4_UNORM_BLOCK: return "BC4_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC4_SNORM_BLOCK: return "BC4_SNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC5_UNORM_BLOCK: return "BC5_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC5_SNORM_BLOCK: return "BC5_SNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC6H_UFLOAT_BLOCK: return "BC6H_UFLOAT_BLOCK";
	case VkFormat::VK_FORMAT_BC6H_SFLOAT_BLOCK: return "BC6H_SFLOAT_BLOCK";
	case VkFormat::VK_FORMAT_BC7_UNORM_BLOCK: return "BC7_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_BC7_SRGB_BLOCK: return "BC7_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return "ETC2_R8G8B8_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return "ETC2_R8G8B8_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return "ETC2_R8G8B8A1_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return "ETC2_R8G8B8A1_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return "ETC2_R8G8B8A8_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return "ETC2_R8G8B8A8_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_EAC_R11_UNORM_BLOCK: return "EAC_R11_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_EAC_R11_SNORM_BLOCK: return "EAC_R11_SNORM_BLOCK";
	case VkFormat::VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return "EAC_R11G11_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return "EAC_R11G11_SNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return "ASTC_4x4_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return "ASTC_4x4_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return "ASTC_5x4_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return "ASTC_5x4_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return "ASTC_5x5_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return "ASTC_5x5_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return "ASTC_6x5_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return "ASTC_6x5_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return "ASTC_6x6_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return "ASTC_6x6_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return "ASTC_8x5_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return "ASTC_8x5_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return "ASTC_8x6_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return "ASTC_8x6_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return "ASTC_8x8_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return "ASTC_8x8_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return "ASTC_10x5_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return "ASTC_10x5_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return "ASTC_10x6_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return "ASTC_10x6_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return "ASTC_10x8_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return "ASTC_10x8_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return "ASTC_10x10_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return "ASTC_10x10_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return "ASTC_12x10_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return "ASTC_12x10_SRGB_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return "ASTC_12x12_UNORM_BLOCK";
	case VkFormat::VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return "ASTC_12x12_SRGB_BLOCK";
	default: return "";
	}
}

VkFormat minty::vk::from_string_vk_format(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "UNDEFINED") return VkFormat::VK_FORMAT_UNDEFINED;
	if (value2 == "R4G4_UNORM_PACK8") return VkFormat::VK_FORMAT_R4G4_UNORM_PACK8;
	if (value2 == "R4G4B4A4_UNORM_PACK16") return VkFormat::VK_FORMAT_R4G4B4A4_UNORM_PACK16;
	if (value2 == "B4G4R4A4_UNORM_PACK16") return VkFormat::VK_FORMAT_B4G4R4A4_UNORM_PACK16;
	if (value2 == "R5G6B5_UNORM_PACK16") return VkFormat::VK_FORMAT_R5G6B5_UNORM_PACK16;
	if (value2 == "B5G6R5_UNORM_PACK16") return VkFormat::VK_FORMAT_B5G6R5_UNORM_PACK16;
	if (value2 == "R5G5B5A1_UNORM_PACK16") return VkFormat::VK_FORMAT_R5G5B5A1_UNORM_PACK16;
	if (value2 == "B5G5R5A1_UNORM_PACK16") return VkFormat::VK_FORMAT_B5G5R5A1_UNORM_PACK16;
	if (value2 == "A1R5G5B5_UNORM_PACK16") return VkFormat::VK_FORMAT_A1R5G5B5_UNORM_PACK16;
	if (value2 == "R8_UNORM") return VkFormat::VK_FORMAT_R8_UNORM;
	if (value2 == "R8_SNORM") return VkFormat::VK_FORMAT_R8_SNORM;
	if (value2 == "R8_USCALED") return VkFormat::VK_FORMAT_R8_USCALED;
	if (value2 == "R8_SSCALED") return VkFormat::VK_FORMAT_R8_SSCALED;
	if (value2 == "R8_UINT") return VkFormat::VK_FORMAT_R8_UINT;
	if (value2 == "R8_SINT") return VkFormat::VK_FORMAT_R8_SINT;
	if (value2 == "R8_SRGB") return VkFormat::VK_FORMAT_R8_SRGB;
	if (value2 == "R8G8_UNORM") return VkFormat::VK_FORMAT_R8G8_UNORM;
	if (value2 == "R8G8_SNORM") return VkFormat::VK_FORMAT_R8G8_SNORM;
	if (value2 == "R8G8_USCALED") return VkFormat::VK_FORMAT_R8G8_USCALED;
	if (value2 == "R8G8_SSCALED") return VkFormat::VK_FORMAT_R8G8_SSCALED;
	if (value2 == "R8G8_UINT") return VkFormat::VK_FORMAT_R8G8_UINT;
	if (value2 == "R8G8_SINT") return VkFormat::VK_FORMAT_R8G8_SINT;
	if (value2 == "R8G8_SRGB") return VkFormat::VK_FORMAT_R8G8_SRGB;
	if (value2 == "R8G8B8_UNORM") return VkFormat::VK_FORMAT_R8G8B8_UNORM;
	if (value2 == "R8G8B8_SNORM") return VkFormat::VK_FORMAT_R8G8B8_SNORM;
	if (value2 == "R8G8B8_USCALED") return VkFormat::VK_FORMAT_R8G8B8_USCALED;
	if (value2 == "R8G8B8_SSCALED") return VkFormat::VK_FORMAT_R8G8B8_SSCALED;
	if (value2 == "R8G8B8_UINT") return VkFormat::VK_FORMAT_R8G8B8_UINT;
	if (value2 == "R8G8B8_SINT") return VkFormat::VK_FORMAT_R8G8B8_SINT;
	if (value2 == "R8G8B8_SRGB") return VkFormat::VK_FORMAT_R8G8B8_SRGB;
	if (value2 == "B8G8R8_UNORM") return VkFormat::VK_FORMAT_B8G8R8_UNORM;
	if (value2 == "B8G8R8_SNORM") return VkFormat::VK_FORMAT_B8G8R8_SNORM;
	if (value2 == "B8G8R8_USCALED") return VkFormat::VK_FORMAT_B8G8R8_USCALED;
	if (value2 == "B8G8R8_SSCALED") return VkFormat::VK_FORMAT_B8G8R8_SSCALED;
	if (value2 == "B8G8R8_UINT") return VkFormat::VK_FORMAT_B8G8R8_UINT;
	if (value2 == "B8G8R8_SINT") return VkFormat::VK_FORMAT_B8G8R8_SINT;
	if (value2 == "B8G8R8_SRGB") return VkFormat::VK_FORMAT_B8G8R8_SRGB;
	if (value2 == "R8G8B8A8_UNORM") return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
	if (value2 == "R8G8B8A8_SNORM") return VkFormat::VK_FORMAT_R8G8B8A8_SNORM;
	if (value2 == "R8G8B8A8_USCALED") return VkFormat::VK_FORMAT_R8G8B8A8_USCALED;
	if (value2 == "R8G8B8A8_SSCALED") return VkFormat::VK_FORMAT_R8G8B8A8_SSCALED;
	if (value2 == "R8G8B8A8_UINT") return VkFormat::VK_FORMAT_R8G8B8A8_UINT;
	if (value2 == "R8G8B8A8_SINT") return VkFormat::VK_FORMAT_R8G8B8A8_SINT;
	if (value2 == "R8G8B8A8_SRGB") return VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
	if (value2 == "B8G8R8A8_UNORM") return VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
	if (value2 == "B8G8R8A8_SNORM") return VkFormat::VK_FORMAT_B8G8R8A8_SNORM;
	if (value2 == "B8G8R8A8_USCALED") return VkFormat::VK_FORMAT_B8G8R8A8_USCALED;
	if (value2 == "B8G8R8A8_SSCALED") return VkFormat::VK_FORMAT_B8G8R8A8_SSCALED;
	if (value2 == "B8G8R8A8_UINT") return VkFormat::VK_FORMAT_B8G8R8A8_UINT;
	if (value2 == "B8G8R8A8_SINT") return VkFormat::VK_FORMAT_B8G8R8A8_SINT;
	if (value2 == "B8G8R8A8_SRGB") return VkFormat::VK_FORMAT_B8G8R8A8_SRGB;
	if (value2 == "A8B8G8R8_UNORM_PACK32") return VkFormat::VK_FORMAT_A8B8G8R8_UNORM_PACK32;
	if (value2 == "A8B8G8R8_SNORM_PACK32") return VkFormat::VK_FORMAT_A8B8G8R8_SNORM_PACK32;
	if (value2 == "A8B8G8R8_USCALED_PACK32") return VkFormat::VK_FORMAT_A8B8G8R8_USCALED_PACK32;
	if (value2 == "A8B8G8R8_SSCALED_PACK32") return VkFormat::VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
	if (value2 == "A8B8G8R8_UINT_PACK32") return VkFormat::VK_FORMAT_A8B8G8R8_UINT_PACK32;
	if (value2 == "A8B8G8R8_SINT_PACK32") return VkFormat::VK_FORMAT_A8B8G8R8_SINT_PACK32;
	if (value2 == "A8B8G8R8_SRGB_PACK32") return VkFormat::VK_FORMAT_A8B8G8R8_SRGB_PACK32;
	if (value2 == "A2R10G10B10_UNORM_PACK32") return VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32;
	if (value2 == "A2R10G10B10_SNORM_PACK32") return VkFormat::VK_FORMAT_A2R10G10B10_SNORM_PACK32;
	if (value2 == "A2R10G10B10_USCALED_PACK32") return VkFormat::VK_FORMAT_A2R10G10B10_USCALED_PACK32;
	if (value2 == "A2R10G10B10_SSCALED_PACK32") return VkFormat::VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
	if (value2 == "A2R10G10B10_UINT_PACK32") return VkFormat::VK_FORMAT_A2R10G10B10_UINT_PACK32;
	if (value2 == "A2R10G10B10_SINT_PACK32") return VkFormat::VK_FORMAT_A2R10G10B10_SINT_PACK32;
	if (value2 == "A2B10G10R10_UNORM_PACK32") return VkFormat::VK_FORMAT_A2B10G10R10_UNORM_PACK32;
	if (value2 == "A2B10G10R10_SNORM_PACK32") return VkFormat::VK_FORMAT_A2B10G10R10_SNORM_PACK32;
	if (value2 == "A2B10G10R10_USCALED_PACK32") return VkFormat::VK_FORMAT_A2B10G10R10_USCALED_PACK32;
	if (value2 == "A2B10G10R10_SSCALED_PACK32") return VkFormat::VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
	if (value2 == "A2B10G10R10_UINT_PACK32") return VkFormat::VK_FORMAT_A2B10G10R10_UINT_PACK32;
	if (value2 == "A2B10G10R10_SINT_PACK32") return VkFormat::VK_FORMAT_A2B10G10R10_SINT_PACK32;
	if (value2 == "R16_UNORM") return VkFormat::VK_FORMAT_R16_UNORM;
	if (value2 == "R16_SNORM") return VkFormat::VK_FORMAT_R16_SNORM;
	if (value2 == "R16_USCALED") return VkFormat::VK_FORMAT_R16_USCALED;
	if (value2 == "R16_SSCALED") return VkFormat::VK_FORMAT_R16_SSCALED;
	if (value2 == "R16_UINT") return VkFormat::VK_FORMAT_R16_UINT;
	if (value2 == "R16_SINT") return VkFormat::VK_FORMAT_R16_SINT;
	if (value2 == "R16_SFLOAT") return VkFormat::VK_FORMAT_R16_SFLOAT;
	if (value2 == "R16G16_UNORM") return VkFormat::VK_FORMAT_R16G16_UNORM;
	if (value2 == "R16G16_SNORM") return VkFormat::VK_FORMAT_R16G16_SNORM;
	if (value2 == "R16G16_USCALED") return VkFormat::VK_FORMAT_R16G16_USCALED;
	if (value2 == "R16G16_SSCALED") return VkFormat::VK_FORMAT_R16G16_SSCALED;
	if (value2 == "R16G16_UINT") return VkFormat::VK_FORMAT_R16G16_UINT;
	if (value2 == "R16G16_SINT") return VkFormat::VK_FORMAT_R16G16_SINT;
	if (value2 == "R16G16_SFLOAT") return VkFormat::VK_FORMAT_R16G16_SFLOAT;
	if (value2 == "R16G16B16_UNORM") return VkFormat::VK_FORMAT_R16G16B16_UNORM;
	if (value2 == "R16G16B16_SNORM") return VkFormat::VK_FORMAT_R16G16B16_SNORM;
	if (value2 == "R16G16B16_USCALED") return VkFormat::VK_FORMAT_R16G16B16_USCALED;
	if (value2 == "R16G16B16_SSCALED") return VkFormat::VK_FORMAT_R16G16B16_SSCALED;
	if (value2 == "R16G16B16_UINT") return VkFormat::VK_FORMAT_R16G16B16_UINT;
	if (value2 == "R16G16B16_SINT") return VkFormat::VK_FORMAT_R16G16B16_SINT;
	if (value2 == "R16G16B16_SFLOAT") return VkFormat::VK_FORMAT_R16G16B16_SFLOAT;
	if (value2 == "R16G16B16A16_UNORM") return VkFormat::VK_FORMAT_R16G16B16A16_UNORM;
	if (value2 == "R16G16B16A16_SNORM") return VkFormat::VK_FORMAT_R16G16B16A16_SNORM;
	if (value2 == "R16G16B16A16_USCALED") return VkFormat::VK_FORMAT_R16G16B16A16_USCALED;
	if (value2 == "R16G16B16A16_SSCALED") return VkFormat::VK_FORMAT_R16G16B16A16_SSCALED;
	if (value2 == "R16G16B16A16_UINT") return VkFormat::VK_FORMAT_R16G16B16A16_UINT;
	if (value2 == "R16G16B16A16_SINT") return VkFormat::VK_FORMAT_R16G16B16A16_SINT;
	if (value2 == "R16G16B16A16_SFLOAT") return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
	if (value2 == "R32_UINT") return VkFormat::VK_FORMAT_R32_UINT;
	if (value2 == "R32_SINT") return VkFormat::VK_FORMAT_R32_SINT;
	if (value2 == "R32_SFLOAT") return VkFormat::VK_FORMAT_R32_SFLOAT;
	if (value2 == "R32G32_UINT") return VkFormat::VK_FORMAT_R32G32_UINT;
	if (value2 == "R32G32_SINT") return VkFormat::VK_FORMAT_R32G32_SINT;
	if (value2 == "R32G32_SFLOAT") return VkFormat::VK_FORMAT_R32G32_SFLOAT;
	if (value2 == "R32G32B32_UINT") return VkFormat::VK_FORMAT_R32G32B32_UINT;
	if (value2 == "R32G32B32_SINT") return VkFormat::VK_FORMAT_R32G32B32_SINT;
	if (value2 == "R32G32B32_SFLOAT") return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
	if (value2 == "R32G32B32A32_UINT") return VkFormat::VK_FORMAT_R32G32B32A32_UINT;
	if (value2 == "R32G32B32A32_SINT") return VkFormat::VK_FORMAT_R32G32B32A32_SINT;
	if (value2 == "R32G32B32A32_SFLOAT") return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
	if (value2 == "R64_UINT") return VkFormat::VK_FORMAT_R64_UINT;
	if (value2 == "R64_SINT") return VkFormat::VK_FORMAT_R64_SINT;
	if (value2 == "R64_SFLOAT") return VkFormat::VK_FORMAT_R64_SFLOAT;
	if (value2 == "R64G64_UINT") return VkFormat::VK_FORMAT_R64G64_UINT;
	if (value2 == "R64G64_SINT") return VkFormat::VK_FORMAT_R64G64_SINT;
	if (value2 == "R64G64_SFLOAT") return VkFormat::VK_FORMAT_R64G64_SFLOAT;
	if (value2 == "R64G64B64_UINT") return VkFormat::VK_FORMAT_R64G64B64_UINT;
	if (value2 == "R64G64B64_SINT") return VkFormat::VK_FORMAT_R64G64B64_SINT;
	if (value2 == "R64G64B64_SFLOAT") return VkFormat::VK_FORMAT_R64G64B64_SFLOAT;
	if (value2 == "R64G64B64A64_UINT") return VkFormat::VK_FORMAT_R64G64B64A64_UINT;
	if (value2 == "R64G64B64A64_SINT") return VkFormat::VK_FORMAT_R64G64B64A64_SINT;
	if (value2 == "R64G64B64A64_SFLOAT") return VkFormat::VK_FORMAT_R64G64B64A64_SFLOAT;
	if (value2 == "B10G11R11_UFLOAT_PACK32") return VkFormat::VK_FORMAT_B10G11R11_UFLOAT_PACK32;
	if (value2 == "E5B9G9R9_UFLOAT_PACK32") return VkFormat::VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
	if (value2 == "D16_UNORM") return VkFormat::VK_FORMAT_D16_UNORM;
	if (value2 == "X8_D24_UNORM_PACK32") return VkFormat::VK_FORMAT_X8_D24_UNORM_PACK32;
	if (value2 == "D32_SFLOAT") return VkFormat::VK_FORMAT_D32_SFLOAT;
	if (value2 == "S8_UINT") return VkFormat::VK_FORMAT_S8_UINT;
	if (value2 == "D16_UNORM_S8_UINT") return VkFormat::VK_FORMAT_D16_UNORM_S8_UINT;
	if (value2 == "D24_UNORM_S8_UINT") return VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
	if (value2 == "D32_SFLOAT_S8_UINT") return VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT;
	if (value2 == "BC1_RGB_UNORM_BLOCK") return VkFormat::VK_FORMAT_BC1_RGB_UNORM_BLOCK;
	if (value2 == "BC1_RGB_SRGB_BLOCK") return VkFormat::VK_FORMAT_BC1_RGB_SRGB_BLOCK;
	if (value2 == "BC1_RGBA_UNORM_BLOCK") return VkFormat::VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
	if (value2 == "BC1_RGBA_SRGB_BLOCK") return VkFormat::VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
	if (value2 == "BC2_UNORM_BLOCK") return VkFormat::VK_FORMAT_BC2_UNORM_BLOCK;
	if (value2 == "BC2_SRGB_BLOCK") return VkFormat::VK_FORMAT_BC2_SRGB_BLOCK;
	if (value2 == "BC3_UNORM_BLOCK") return VkFormat::VK_FORMAT_BC3_UNORM_BLOCK;
	if (value2 == "BC3_SRGB_BLOCK") return VkFormat::VK_FORMAT_BC3_SRGB_BLOCK;
	if (value2 == "BC4_UNORM_BLOCK") return VkFormat::VK_FORMAT_BC4_UNORM_BLOCK;
	if (value2 == "BC4_SNORM_BLOCK") return VkFormat::VK_FORMAT_BC4_SNORM_BLOCK;
	if (value2 == "BC5_UNORM_BLOCK") return VkFormat::VK_FORMAT_BC5_UNORM_BLOCK;
	if (value2 == "BC5_SNORM_BLOCK") return VkFormat::VK_FORMAT_BC5_SNORM_BLOCK;
	if (value2 == "BC6H_UFLOAT_BLOCK") return VkFormat::VK_FORMAT_BC6H_UFLOAT_BLOCK;
	if (value2 == "BC6H_SFLOAT_BLOCK") return VkFormat::VK_FORMAT_BC6H_SFLOAT_BLOCK;
	if (value2 == "BC7_UNORM_BLOCK") return VkFormat::VK_FORMAT_BC7_UNORM_BLOCK;
	if (value2 == "BC7_SRGB_BLOCK") return VkFormat::VK_FORMAT_BC7_SRGB_BLOCK;
	if (value2 == "ETC2_R8G8B8_UNORM_BLOCK") return VkFormat::VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
	if (value2 == "ETC2_R8G8B8_SRGB_BLOCK") return VkFormat::VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
	if (value2 == "ETC2_R8G8B8A1_UNORM_BLOCK") return VkFormat::VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
	if (value2 == "ETC2_R8G8B8A1_SRGB_BLOCK") return VkFormat::VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
	if (value2 == "ETC2_R8G8B8A8_UNORM_BLOCK") return VkFormat::VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
	if (value2 == "ETC2_R8G8B8A8_SRGB_BLOCK") return VkFormat::VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
	if (value2 == "EAC_R11_UNORM_BLOCK") return VkFormat::VK_FORMAT_EAC_R11_UNORM_BLOCK;
	if (value2 == "EAC_R11_SNORM_BLOCK") return VkFormat::VK_FORMAT_EAC_R11_SNORM_BLOCK;
	if (value2 == "EAC_R11G11_UNORM_BLOCK") return VkFormat::VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
	if (value2 == "EAC_R11G11_SNORM_BLOCK") return VkFormat::VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
	if (value2 == "ASTC_4x4_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
	if (value2 == "ASTC_4x4_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
	if (value2 == "ASTC_5x4_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
	if (value2 == "ASTC_5x4_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
	if (value2 == "ASTC_5x5_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
	if (value2 == "ASTC_5x5_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
	if (value2 == "ASTC_6x5_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
	if (value2 == "ASTC_6x5_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
	if (value2 == "ASTC_6x6_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
	if (value2 == "ASTC_6x6_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
	if (value2 == "ASTC_8x5_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
	if (value2 == "ASTC_8x5_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
	if (value2 == "ASTC_8x6_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
	if (value2 == "ASTC_8x6_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
	if (value2 == "ASTC_8x8_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
	if (value2 == "ASTC_8x8_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
	if (value2 == "ASTC_10x5_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
	if (value2 == "ASTC_10x5_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
	if (value2 == "ASTC_10x6_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
	if (value2 == "ASTC_10x6_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
	if (value2 == "ASTC_10x8_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
	if (value2 == "ASTC_10x8_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
	if (value2 == "ASTC_10x10_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
	if (value2 == "ASTC_10x10_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
	if (value2 == "ASTC_12x10_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
	if (value2 == "ASTC_12x10_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
	if (value2 == "ASTC_12x12_UNORM_BLOCK") return VkFormat::VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
	if (value2 == "ASTC_12x12_SRGB_BLOCK") return VkFormat::VK_FORMAT_ASTC_12x12_SRGB_BLOCK;

	return static_cast<VkFormat>(0);
}

String minty::vk::to_string(VkSamplerAddressMode const value)
{
	switch (value)
	{
	case VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT: return "REPEAT";
	case VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: return "MIRRORED_REPEAT";
	case VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE: return "CLAMP_TO_EDGE";
	case VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER: return "CLAMP_TO_BORDER";
	case VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: return "MIRROR_CLAMP_TO_EDGE";
	default: return "";
	}
}

VkSamplerAddressMode minty::vk::from_string_vk_sampler_address_mode(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "REPEAT") return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
	if (value2 == "MIRRORED_REPEAT") return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	if (value2 == "CLAMP_TO_EDGE") return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	if (value2 == "CLAMP_TO_BORDER") return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	if (value2 == "MIRROR_CLAMP_TO_EDGE") return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;

	return static_cast<VkSamplerAddressMode>(0);
}

String minty::vk::to_string(VkSamplerMipmapMode const value)
{
	switch (value)
	{
	case VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST: return "NEAREST";
	case VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR: return "LINEAR";
	default: return "";
	}
}

VkSamplerMipmapMode minty::vk::from_string_vk_sampler_mipmap_mode(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "NEAREST") return VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST;
	if (value2 == "LINEAR") return VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR;

	return static_cast<VkSamplerMipmapMode>(0);
}

String minty::vk::to_string(VkShaderStageFlagBits const value)
{
	switch (value)
	{
	case VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT: return "VERTEX_BIT";
	case VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return "TESSELLATION_CONTROL_BIT";
	case VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return "TESSELLATION_EVALUATION_BIT";
	case VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT: return "GEOMETRY_BIT";
	case VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT: return "FRAGMENT_BIT";
	case VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT: return "COMPUTE_BIT";
	case VkShaderStageFlagBits::VK_SHADER_STAGE_ALL_GRAPHICS: return "ALL_GRAPHICS";
	default: return "";
	}
}

VkShaderStageFlagBits minty::vk::from_string_vk_shader_stage_flag_bits(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "VERTEX_BIT") return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
	if (value2 == "TESSELLATION_CONTROL_BIT") return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	if (value2 == "TESSELLATION_EVALUATION_BIT") return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	if (value2 == "GEOMETRY_BIT") return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
	if (value2 == "FRAGMENT_BIT") return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
	if (value2 == "COMPUTE_BIT") return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
	if (value2 == "ALL_GRAPHICS") return VkShaderStageFlagBits::VK_SHADER_STAGE_ALL_GRAPHICS;

	return static_cast<VkShaderStageFlagBits>(0);
}

String minty::vk::to_string(VkDescriptorType const value)
{
	switch (value)
	{
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER: return "SAMPLER";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return "COMBINED_IMAGE_SAMPLER";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return "SAMPLED_IMAGE";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE: return "STORAGE_IMAGE";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return "UNIFORM_TEXEL_BUFFER";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return "STORAGE_TEXEL_BUFFER";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return "UNIFORM_BUFFER";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: return "STORAGE_BUFFER";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return "UNIFORM_BUFFER_DYNAMIC";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return "STORAGE_BUFFER_DYNAMIC";
	case VkDescriptorType::VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return "INPUT_ATTACHMENT";
	default: return "";
	}
}

VkDescriptorType minty::vk::from_string_vk_descriptor_type(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "SAMPLER") return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
	if (value2 == "COMBINED_IMAGE_SAMPLER") return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	if (value2 == "SAMPLED_IMAGE") return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	if (value2 == "STORAGE_IMAGE") return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	if (value2 == "UNIFORM_TEXEL_BUFFER") return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
	if (value2 == "STORAGE_TEXEL_BUFFER") return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	if (value2 == "UNIFORM_BUFFER") return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	if (value2 == "STORAGE_BUFFER") return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	if (value2 == "UNIFORM_BUFFER_DYNAMIC") return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	if (value2 == "STORAGE_BUFFER_DYNAMIC") return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
	if (value2 == "INPUT_ATTACHMENT") return VkDescriptorType::VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	return static_cast<VkDescriptorType>(0);
}

String minty::vk::to_string(VkPrimitiveTopology const value)
{
	switch (value)
	{
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST: return "POINT_LIST";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST: return "LINE_LIST";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: return "LINE_STRIP";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: return "TRIANGLE_LIST";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: return "TRIANGLE_STRIP";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN: return "TRIANGLE_FAN";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY: return "LINE_LIST_WITH_ADJACENCY";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY: return "LINE_STRIP_WITH_ADJACENCY";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY: return "TRIANGLE_LIST_WITH_ADJACENCY";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return "TRIANGLE_STRIP_WITH_ADJACENCY";
	case VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_PATCH_LIST: return "PATCH_LIST";
	default: return "";
	}
}

VkPrimitiveTopology minty::vk::from_string_vk_primitive_topology(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "POINT_LIST") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	if (value2 == "LINE_LIST") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	if (value2 == "LINE_STRIP") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	if (value2 == "TRIANGLE_LIST") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	if (value2 == "TRIANGLE_STRIP") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	if (value2 == "TRIANGLE_FAN") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
	if (value2 == "LINE_LIST_WITH_ADJACENCY") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
	if (value2 == "LINE_STRIP_WITH_ADJACENCY") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
	if (value2 == "TRIANGLE_LIST_WITH_ADJACENCY") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
	if (value2 == "TRIANGLE_STRIP_WITH_ADJACENCY") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
	if (value2 == "PATCH_LIST") return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

	return static_cast<VkPrimitiveTopology>(0);
}

String minty::vk::to_string(VkPolygonMode const value)
{
	switch (value)
	{
	case VkPolygonMode::VK_POLYGON_MODE_FILL: return "FILL";
	case VkPolygonMode::VK_POLYGON_MODE_LINE: return "LINE";
	case VkPolygonMode::VK_POLYGON_MODE_POINT: return "POINT";
	default: return "";
	}
}

VkPolygonMode minty::vk::from_string_vk_polygon_mode(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "FILL") return VkPolygonMode::VK_POLYGON_MODE_FILL;
	if (value2 == "LINE") return VkPolygonMode::VK_POLYGON_MODE_LINE;
	if (value2 == "POINT") return VkPolygonMode::VK_POLYGON_MODE_POINT;

	return static_cast<VkPolygonMode>(0);
}

String minty::vk::to_string(VkCullModeFlagBits const value)
{
	switch (value)
	{
	case VkCullModeFlagBits::VK_CULL_MODE_NONE: return "NONE";
	case VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT: return "FRONT_BIT";
	case VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT: return "BACK_BIT";
	case VkCullModeFlagBits::VK_CULL_MODE_FRONT_AND_BACK: return "FRONT_AND_BACK";
	default: return "";
	}
}

VkCullModeFlagBits minty::vk::from_string_vk_cull_mode_flag_bits(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "NONE") return VkCullModeFlagBits::VK_CULL_MODE_NONE;
	if (value2 == "FRONT_BIT") return VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
	if (value2 == "BACK_BIT") return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
	if (value2 == "FRONT_AND_BACK") return VkCullModeFlagBits::VK_CULL_MODE_FRONT_AND_BACK;

	return static_cast<VkCullModeFlagBits>(0);
}

String minty::vk::to_string(VkFrontFace const value)
{
	switch (value)
	{
	case VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE: return "COUNTER_CLOCKWISE";
	case VkFrontFace::VK_FRONT_FACE_CLOCKWISE: return "CLOCKWISE";
	default: return "";
	}
}

VkFrontFace minty::vk::from_string_vk_front_face(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "COUNTER_CLOCKWISE") return VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
	if (value2 == "CLOCKWISE") return VkFrontFace::VK_FRONT_FACE_CLOCKWISE;

	return static_cast<VkFrontFace>(0);
}

String minty::vk::to_string(VkVertexInputRate const value)
{
	switch (value)
	{
	case VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX: return "VERTEX";
	case VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE: return "INSTANCE";
	default: return "";
	}
}

VkVertexInputRate minty::vk::from_string_vk_vertex_input_rate(String const& value)
{
	String value2 = minty::Text::to_upper(value);
	if (value2 == "VERTEX") return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
	if (value2 == "INSTANCE") return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;

	return static_cast<VkVertexInputRate>(0);
}