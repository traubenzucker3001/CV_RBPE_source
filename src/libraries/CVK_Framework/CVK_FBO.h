#ifndef __CVK_FBO_H
#define __CVK_FBO_H

#include "CVK_Defs.h"

namespace CVK
{

/**
 * @brief Class for OpenGL frame buffer objects (FBOs).
 * 
 * A FBO has a given width and height. All textures used by it
 * or created by it have these values for width and height. It
 * is possible to create a fbo with multiple color buffers, each
 * with rgba values per pixel. A fbo can use a depth buffer and
 * a stencil buffer, which are not necesserily needed.
 * 
 * To create a fbo, the constructor has to be called with the 
 * preferred width and height. Additionally the number of color
 * textures and the use of depth or stencil buffer can be assigned.
 * An example creation of a fbo with 1 color buffer and a depth
 * buffer is:
 * CVK::FBO fbo( 400, 300, 1, true);
 * 
 * To use a fbo, it has to be bound to OpenGL. If the standard
 * OpenGL frame buffer should be used, the fbo has to be unbound.
 * fbo.bind();
 * ...
 * fbo.unbind();
 * 
 * To use the result of the fbo, getters for the colorTextures
 * and the depth texture exist.
 * GLuint colorTexture = fbo.getColorTexture(0);
 * GLuint depthTexture = fbo.getDepthTexture();
 */
class FBO
{
public:
	FBO(int width, int height);
	FBO(int width, int height, int numColorTextures);
	FBO(int width, int height, int numColorTextures, bool depthTexture);
	FBO(int width, int height, int numColorTextures, bool depthTexture, bool stencilTexture);
	~FBO();

	// called by constructors
	void create(int width, int height, int numColorTextures, bool depthTexture, bool stencilTexture);
	void reset();
	void resize(int width, int height);

	void bind();
	void unbind();

	// getters for the results of rendering
	GLuint getColorTexture(unsigned int index);
	GLuint getDepthTexture();

private:
	GLuint createTexture();

	int m_width, m_height;
	std::vector<GLuint> m_colorTextures;
	GLuint m_depthTexture, m_stencilTexture;
	GLuint m_frameBufferHandle;
};

};

#endif /* __CVK_FBO_H */

