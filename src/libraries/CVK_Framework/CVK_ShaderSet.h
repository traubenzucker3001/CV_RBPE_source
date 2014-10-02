#ifndef __CVK_SHADERSET_H
#define __CVK_SHADERSET_H

#include "CVK_Defs.h"
#include "CVK_State.h"

namespace CVK
{

class ShaderSet
{
public:
	ShaderSet();
	ShaderSet( GLuint shader_mask, const char** ShaderNames);
	~ShaderSet();

	void GenerateShaderProgramm( GLuint shader_mask, const char** ShaderNames);
	GLuint getProgramID();
	void useProgram();

    void setTextureInput(int num, GLuint texture);
	void setValue(const char* variableName, float value);

private: 
	void checkShader(GLuint shaderID);
	void checkProgram(GLuint programID);
	void loadShaderSource(GLint shaderID, const char* fileName);

	GLuint m_shader_mask;

protected:
	GLuint m_ProgramID;
	std::vector<GLuint> m_textures;
};

};

#endif /*__CVK_SHADERSET_H*/
