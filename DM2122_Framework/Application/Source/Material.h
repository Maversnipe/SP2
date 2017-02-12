#ifndef MATERIAL_H_
#define MATERIAL_H_

struct Component
{
	float r, g, b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f)
	{
		Set(r, g, b);
	}
	void Set(float r, float g, float b)
	{
		this->r = r; this->g = g; this->b = b;
	}
};
struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess = 0;
	Material()
	{
		kAmbient.Set(0.25f, 0.25f, 0.25f);
		kDiffuse.Set(0.7f, 0.7f, 0.7f);
		kSpecular.Set(0.5f, 0.5f, 0.5f);
		kShininess = 1.f;
	}
};

#endif