#include "Material.h"

Material::Material(void)
{
}

Material::~Material(void)
{
}

void Material::setOpenGLMaterial() const
{
#ifdef USING_OPENGL
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
#endif
}
