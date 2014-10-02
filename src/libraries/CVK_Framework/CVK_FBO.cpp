#include "CVK_FBO.h"

CVK::FBO::FBO(int width, int height)
{
	create(width, height, 1, false, false);
}

CVK::FBO::FBO(int width, int height, int numColorTextures)
{
	create(width, height, numColorTextures, false, false);
}

CVK::FBO::FBO(int width, int height, int numColorTextures, bool depthTexture)
{
	create(width, height, numColorTextures, depthTexture, false);
}

CVK::FBO::FBO(int width, int height, int numColorTextures, bool depthTexture, bool stencilTexture)
{
	create(width, height, numColorTextures, depthTexture, stencilTexture);
}

CVK::FBO::~FBO()
{
	reset();
}

void CVK::FBO::create(int width, int height, int numColorTextures, bool depthTexture, bool stencilTexture)
{
	reset();

	m_width = width;
	m_height = height;

	// generate fbo
	glGenFramebuffers(1, &m_frameBufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
	
	// generate color textures
	std::vector<GLenum> drawBuffers;
	for(int i = 0; i < numColorTextures; i++)
	{
		GLuint colorTextureHandle = createTexture();
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F, m_width, m_height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+m_colorTextures.size(), GL_TEXTURE_2D, colorTextureHandle, 0);
		m_colorTextures.push_back(colorTextureHandle);
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0+i);
	}
	// use color textures in fbo
	glDrawBuffers(numColorTextures, drawBuffers.data());

	if(depthTexture)
	{
		// generate depth texture
		m_depthTexture = createTexture();
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, m_width, m_height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	}

	if(stencilTexture)
	{
		// generate stencil texture
		m_stencilTexture = createTexture();
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH24_STENCIL8, m_width, m_height, 0,GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilTexture, 0);
	}
	
	// Any errors while generating fbo ?
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR: Unable to create FBO.\n");
		reset();
	}
	
	// cleanup
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CVK::FBO::reset()
{
	// delete all textures
	for(unsigned int i = 0; i < m_colorTextures.size(); i++)
	{
		glDeleteTextures(1, &m_colorTextures[i]);
	}
	m_colorTextures.clear();
	if(m_depthTexture != INVALID_OGL_VALUE)
		glDeleteTextures(1, &m_depthTexture);
	if(m_stencilTexture != INVALID_OGL_VALUE)
		glDeleteTextures(1, &m_stencilTexture);
	if(m_frameBufferHandle != INVALID_OGL_VALUE)
		glDeleteFramebuffers(1, &m_frameBufferHandle);
	
	// reset values
	m_depthTexture = INVALID_OGL_VALUE;
	m_stencilTexture = INVALID_OGL_VALUE;
	m_frameBufferHandle = INVALID_OGL_VALUE;
}

void CVK::FBO::resize(int width, int height)
{
	int numColorTextures = m_colorTextures.size();
	bool depthTexture = false;
	if(m_depthTexture != INVALID_OGL_VALUE) 
		depthTexture = true;
	bool stencilTexture = false;
	if(m_stencilTexture != INVALID_OGL_VALUE) 
		stencilTexture = true;

	reset();
	create(width, height, numColorTextures, depthTexture, stencilTexture);
}

void CVK::FBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
	glViewport( 0, 0, m_width, m_height);
}

void CVK::FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint CVK::FBO::getColorTexture(unsigned int index)
{
	// does color buffer with given index exist ?
	if(index < m_colorTextures.size())
		return m_colorTextures[index];
	return 0;
}

GLuint CVK::FBO::getDepthTexture()
{
	// does depth buffer exist ?
	if(m_depthTexture == INVALID_OGL_VALUE)
		return 0;
	return m_depthTexture;
}

GLuint CVK::FBO::createTexture()
{
	// generate fresh texture in OpenGL
	GLuint textureID;
	glGenTextures( 1, &textureID);
	glBindTexture( GL_TEXTURE_2D, textureID);
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	return textureID;
}

