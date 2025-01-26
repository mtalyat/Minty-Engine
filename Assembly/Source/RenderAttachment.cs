using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class RenderAttachment
    {
        private RenderAttachmentType _type = RenderAttachmentType.Color;
        private Format _format = Format.Undefined;
        private RenderAttachmentLoadOperation _loadOperation = RenderAttachmentLoadOperation.DontCare;
        private RenderAttachmentStoreOperation _storeOperation = RenderAttachmentStoreOperation.DontCare;
        private ImageLayout _initialLayout = ImageLayout.Undefined;
        private ImageLayout _finalLayout = ImageLayout.Undefined;

        public RenderAttachmentType Type => _type;
        public Format Format => _format;
        public RenderAttachmentLoadOperation LoadOperation => _loadOperation;
        public RenderAttachmentStoreOperation StoreOperation => _storeOperation;
        public ImageLayout InitialLayout => _initialLayout;
        public ImageLayout FinalLayout => _finalLayout;
    }
}
