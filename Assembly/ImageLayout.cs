using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public enum ImageLayout
    {
        Undefined = 0,
		General = 1,
		ColorAttachmentOptimal = 2,
		DepthStencilAttachmentOptimal = 3,
		DepthStencilReadOnlyOptimal = 4,
		ShaderReadOnlyOptimal = 5,
		TransferSrcOptimal = 6,
		TransferDstOptimal = 7,
		Preinitialized = 8,
		DepthReadOnlyStencilAttachmentOptimal = 1000117000,
		DepthAttachmentStencilReadOnlyOptimal = 1000117001,
		DepthAttachmentOptimal = 1000241000,
		DepthReadOnlyOptimal = 1000241001,
		StencilAttachmentOptimal = 1000241002,
		StencilReadOnlyOptimal = 1000241003,
		ReadOnlyOptimal = 1000314000,
		AttachmentOptimal = 1000314001,
		PresentSrcKhr = 1000001002,
		VideoDecodeDstKhr = 1000024000,
		VideoDecodeSrcKhr = 1000024001,
		VideoDecodeDpbKhr = 1000024002,
		SharedPresentKhr = 1000111000,
		FragmentDensityMapOptimalExt = 1000218000,
		FragmentShadingRateAttachmentOptimalKhr = 1000164003,
	};
}
