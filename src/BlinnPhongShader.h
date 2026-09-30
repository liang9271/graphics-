#pragma once

#include "Shader.h"

class BlinnPhongShader : public Shader {

public: 
BlinnPhongShader(const color& diffuseColoer,
		 const color& specularColor,
		 double phongExponent);

color rayColor(const HitSphere& h) override;

private:

color diffuseColor; // the basic color
color specularColor; // shiny light color
double phongExponent; // light concentration

};
