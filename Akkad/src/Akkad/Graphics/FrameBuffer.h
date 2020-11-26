#pragma once

namespace Akkad {
	namespace Graphics {

		struct FrameBufferDescriptor {
			int width, height;

			bool hasColorAttachment;
			bool hasDepthAttachment;
			bool hasStencilAttachment;
		};

		class FrameBuffer {
		public:
			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void SetSize(unsigned int width, unsigned int height) = 0;

			virtual unsigned int GetColorAttachmentTexture() = 0;

			virtual FrameBufferDescriptor& GetDescriptor() = 0;
		};
	}
}