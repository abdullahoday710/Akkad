#pragma once

#include "Texture.h"

#include <glm/glm.hpp>
namespace Akkad {
	namespace Graphics {

		struct FrameBufferDescriptor {
			int width, height = 0;

			bool hasColorAttachment = true;
			bool hasDepthAttachment = false;
			bool hasStencilAttachment = false;
			bool usesRenderBuffer = false;

			TextureFormat ColorAttachmentFormat = TextureFormat::R32_FLOAT;
		};

		class FrameBuffer {
		public:
			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void SetSize(unsigned int width, unsigned int height) = 0;
			virtual glm::vec4 ReadPixels(int x, int y) = 0;

			virtual unsigned int GetColorAttachmentTexture() = 0;

			virtual FrameBufferDescriptor& GetDescriptor() = 0;
		};
	}
}