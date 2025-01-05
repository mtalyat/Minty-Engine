#include "pch.h"
#include "RenderPass.h"

#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Minty/Render/RenderTarget.h"
#include "Platform/Vulkan/VulkanRenderer.h"

using namespace Minty;

Ref<RenderTarget> Minty::RenderPass::create_render_target(RenderTargetBuilder const& builder)
{
    m_targets.push_back(RenderTarget::create(builder));

    Ref<RenderTarget> renderTarget = m_targets.back().create_ref();

    if (m_activeTarget == nullptr)
    {
        m_activeTarget = renderTarget;
    }

    return renderTarget;
}

void Minty::RenderPass::destroy_render_target(Ref<RenderTarget> const& target)
{
    for (Size i = 0; i < m_targets.size(); i++)
    {
        if (m_targets.at(i).get() == target.get())
        {
            m_targets.erase(m_targets.begin() + i);
            break;
        }
    }
}

void Minty::RenderPass::reinitialize_render_targets(RenderTargetBuilder const& builder)
{
    for (auto const& target : m_targets)
    {
        target->reinitialize(builder);
    }
}

Owner<RenderPass> Minty::RenderPass::create(RenderPassBuilder const& builder)
{
#if defined(MINTY_VULKAN)
    return Owner<VulkanRenderPass>(builder);
#else
    return Owner<RenderPass>();
#endif // MINTY_VULKAN
}
