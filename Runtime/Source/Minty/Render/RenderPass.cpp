#include "pch.h"
#include "RenderPass.h"

#include "Minty/Asset/AssetManager.h"
#include "Minty/Render/RenderTarget.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanRenderPass.h"

using namespace Minty;

Minty::RenderPass::~RenderPass()
{
    // TODO: destroy resources?
}

Ref<RenderTarget> Minty::RenderPass::create_render_target(RenderTargetBuilder const& builder)
{
    m_targets.push_back(AssetManager::create<RenderTarget>(builder));

    Ref<RenderTarget> renderTarget = m_targets.back();

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
