#ifndef __EMBREE_GLASS_H
#define __EMBREE_GLASS_H

#include "materials/material.h"
#include "brdfs/reflection.h"

namespace embree
{
	class Glass : public Material
	{
		Glass(const Parms& parms) 
		{
			reflectance = parms.getCol3f("reflectance",one);
		}

		void shade(const Ray& ray, const Medium& currentMedium, const DifferentialGeometry& dg, CompositedBRDF& brdfs) const 
		{
		  brdfs.add(NEW_BRDF(Reflection)(reflectance));
		}
		
		Col3f getReflectance() {return reflectance;}

		private:
			Col3f reflectance;  //!< Reflectivity of the glass
	};	
	
}
