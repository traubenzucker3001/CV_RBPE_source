#include "CVK_Camera.h"

CVK::Camera::Camera()
{
}

CVK::Camera::~Camera()
{
}

void CVK::Camera::getView( glm::mat4 *view)
{
	*view = m_viewmatrix;
}

void CVK::Camera::getPerspective( glm::mat4 *perspective)
{
	*perspective = m_perspective;
}

void CVK::Camera::setNearFar( float near, float far)
{
	m_znear = near;
	m_zfar = far;
	m_perspective = glm::perspective( m_fov, m_width / (float) m_height, m_znear, m_zfar);    
}

void CVK::Camera::getNearFar( float *near, float *far)
{
	*near = m_znear;
	*far = m_zfar;
}

void CVK::Camera::setWidthHeight( int width, int height)
{
	m_width = width;
	m_height = height;
}
