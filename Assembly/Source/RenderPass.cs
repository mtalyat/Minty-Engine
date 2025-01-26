using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    //public class RenderPass : Asset
    //{
    //    private RenderAttachment _colorAttachment;
    //    private RenderAttachment _depthAttachment;

    //    public RenderAttachment ColorAttachment => _colorAttachment;
    //    public RenderAttachment DepthAttachment => _depthAttachment;

    //    public RenderPass(RenderAttachment colorAttachment, RenderAttachment depthAttachment = null)
    //        : base(Runtime.RenderPass_Create())
    //    {
    //        if (colorAttachment == null)
    //        {
    //            Debug.Error("Cannot create a RenderPass with no color attachment.");
    //            return;
    //        }

    //        if (colorAttachment.Type != RenderAttachmentType.Color)
    //        {
    //            Debug.Error($"Attempting to create a RenderPass with a color attachment that has attachment type \"{colorAttachment.Type}\".");
    //            return;
    //        }

    //        if (depthAttachment != null)
    //        {
    //            if (depthAttachment.Type != RenderAttachmentType.Depth)
    //            {
    //                Debug.Error($"Attempting to create a RenderPass with a depth attachment that has attachment type \"{colorAttachment.Type}\".");
    //                return;
    //            }

    //            UpdateAttachment(depthAttachment);
    //        }

    //        UpdateAttachment(colorAttachment);

    //        Apply();
    //    }

    //    public bool UpdateAttachment(RenderAttachment attachment)
    //    {
    //        switch (attachment.Type)
    //        {
    //            case RenderAttachmentType.Color:
    //                _colorAttachment = attachment;
    //                break;
    //            case RenderAttachmentType.Depth:
    //                _depthAttachment = attachment;
    //                break;
    //        }

    //        return Runtime.RenderPass_UpdateAttachment(ID, (int)attachment.Type, (int)attachment.Format, (int)attachment.LoadOperation, (int)attachment.StoreOperation, (int)attachment.InitialLayout, (int)attachment.FinalLayout);
    //    }

    //    public bool Apply()
    //    {
    //        return Runtime.RenderPass_Apply(ID);
    //    }
    //}
}
