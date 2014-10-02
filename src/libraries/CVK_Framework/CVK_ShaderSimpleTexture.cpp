#include "CVK_ShaderSimpleTexture.h"

CVK::ShaderSimpleTexture::ShaderSimpleTexture( GLuint shader_mask, const char** shaderPaths) : CVK::ShaderPostProcessing(shader_mask, shaderPaths)
{	
	m_colorTextureID = glGetUniformLocation( m_ProgramID, "colortexture"); 
}

void CVK::ShaderSimpleTexture::update()
{
	if (m_textures.size() > 0)
	{
		glUniform1i( m_colorTextureID, 0);
		glActiveTexture( GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, m_textures[0]);
	}
}

