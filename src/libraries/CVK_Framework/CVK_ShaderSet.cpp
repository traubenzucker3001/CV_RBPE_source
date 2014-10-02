#include <string>
#include <fstream>

#include "CVK_ShaderSet.h"

CVK::ShaderSet::ShaderSet()
{
}

// ShaderNames must be in order Vertx, Tess_Control, Tess_Eval, Geometry, Fragment, Compute
CVK::ShaderSet::ShaderSet( GLuint shader_mask, const char** shaderPaths)
{
	GenerateShaderProgramm( shader_mask, shaderPaths);
}

GLuint CVK::ShaderSet::getProgramID()
{
	return( m_ProgramID);
}

void CVK::ShaderSet::useProgram()
{
	glUseProgram(m_ProgramID);
}

CVK::ShaderSet::~ShaderSet()
{
	if (m_ProgramID != INVALID_OGL_VALUE) glDeleteProgram( m_ProgramID);
}

void CVK::ShaderSet::setTextureInput(int num, GLuint texture)
{
	if(num >= m_textures.size())
		m_textures.resize(num+1);
	m_textures[num] = texture;
}

void CVK::ShaderSet::setValue(const char* variableName, float value)
{
	GLuint variableID = glGetUniformLocation(m_ProgramID, variableName);
	glUniform1f(variableID, value);
}

void CVK::ShaderSet::GenerateShaderProgramm(GLuint shader_mask, const char** shaderPaths)
{
    GLuint vertexShaderID, tessControlShaderID, tessEvalShaderID;
	GLuint geometryShaderID, fragmentShaderID, computeShaderID;

    int next_name = 0;
    m_shader_mask = shader_mask;

	m_ProgramID = INVALID_OGL_VALUE;

	if (shader_mask & VERTEX_SHADER_BIT)
	{
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		loadShaderSource(vertexShaderID, shaderPaths[next_name++]);
		glCompileShader(vertexShaderID);
		checkShader(vertexShaderID);
	}

	if (shader_mask & TESS_CONTROL_BIT)
	{
		tessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		loadShaderSource(tessControlShaderID, shaderPaths[next_name++]);
		glCompileShader(tessControlShaderID);
		checkShader(tessControlShaderID);
	}

	if (shader_mask & TESS_EVAL_BIT)
	{
		tessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER );
		loadShaderSource(tessEvalShaderID, shaderPaths[next_name++]);
		glCompileShader(tessEvalShaderID);
		checkShader(tessEvalShaderID);
	}

	if (shader_mask & GEOMETRY_SHADER_BIT)
	{
		geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		loadShaderSource(geometryShaderID, shaderPaths[next_name++]);
		glCompileShader(geometryShaderID);
		checkShader(geometryShaderID);
	}

	if (shader_mask & FRAGMENT_SHADER_BIT)
	{    
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		loadShaderSource(fragmentShaderID, shaderPaths[next_name++]);
		glCompileShader(fragmentShaderID);
		checkShader(fragmentShaderID);
	}

	if (shader_mask & COMPUTE_SHADER_BIT)
	{
		computeShaderID = glCreateShader(GL_COMPUTE_SHADER);
		loadShaderSource(computeShaderID, shaderPaths[next_name++]);
		glCompileShader(computeShaderID);
		checkShader(computeShaderID);
	}

	//link shader programs
	m_ProgramID = glCreateProgram();

	if (shader_mask & VERTEX_SHADER_BIT) glAttachShader(m_ProgramID, vertexShaderID);
	if (shader_mask & TESS_CONTROL_BIT) glAttachShader(m_ProgramID, tessControlShaderID);
	if (shader_mask & TESS_EVAL_BIT) glAttachShader(m_ProgramID, tessEvalShaderID);
	if (shader_mask & GEOMETRY_SHADER_BIT) glAttachShader(m_ProgramID, geometryShaderID);
	if (shader_mask & FRAGMENT_SHADER_BIT) glAttachShader(m_ProgramID, fragmentShaderID);
	if (shader_mask & COMPUTE_SHADER_BIT) glAttachShader(m_ProgramID, computeShaderID);

	glLinkProgram(m_ProgramID);
	checkProgram(m_ProgramID);
}

//checks a shader for compiler errors
void CVK::ShaderSet::checkShader(GLuint shaderID) {
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);

        std::cout << "ERROR: Unable to compile shader" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    } else {
        std::cout << "SUCCESS: Shader compiled" << std::endl;
    }
}

//checks a program
void CVK::ShaderSet::checkProgram(GLuint programID) {
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(programID, infoLogLength, NULL, infoLog);

        std::cout << "ERROR: Unable to link ShaderSet" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    } else {
        std::cout << "SUCCESS: ShaderSet linked" << std::endl;
    }
}

//reads a file and returns the content as a pointer to chars
void CVK::ShaderSet::loadShaderSource(GLint shaderID, const char* fileName) {
    std::string fileContent;
    std::string line;

    //open file and "parse" input
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (!file.eof()){
            getline (file, line);
            fileContent += line + "\n";
        }
        file.close();
        std::cout << "SUCCESS: Opened file " << fileName << std::endl;
    }
    else
        std::cout << "ERROR: Unable to open file " << fileName << std::endl;

    const char* source = fileContent.c_str();
    const GLint source_size = strlen(source);
    
    glShaderSource(shaderID, 1, &source, &source_size);
}
