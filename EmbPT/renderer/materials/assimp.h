// ======================================================================== //
// Copyright 2009-2011 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __EMBREE_ASSIMP_H__
#define __EMBREE_ASSIMP_H__

#include "materials/material.h"
#include "materials/assimp.h"
#include "materials/obj.h"
#include "materials/matte.h"
#include "brdfs/lambertian.h"
#include "brdfs/specular.h"
#include "brdfs/transmission.h"
#include "brdfs/black_matter.h"
#include "textures/texture.h"

#include <iostream>

namespace embree
{
	/*! Shading type enumeration.
	 * This enumeration enumerates all possible shading for a material according to library Assimp
	 */
	enum assipm_shading_type {
		AI_SM_FLAT = 0x1,
		AI_SM_GOURAUD = 0x2,
		AI_SM_PHONG = 0x3,
		AI_SM_BLINN = 0x4,
		AI_SM_TOON = 0x5,
		AI_SM_OREN_NAYAR = 0x6,
		AI_SM_MINNAERT= 0x7,
		AI_SM_COOK_TORRANCE = 0x8,
		AI_SM_NO_SHADING = 0x9,
		AI_SM_FRESNEL = 0xa
	};

	/*! Texture type enumeration.
	 * \ref embree::assipm_texture_type texture type
	 * This is related to different stacks of textures. Each stack relies on a different kind of FX or
	 * material component... Last type is reserved for complex use of textures.
	 */
	enum  assipm_texture_type {
		AI_TT__NONE = 0x0,
		AI_TT__DIFFUSE = 0x1,
		AI_TT__SPECULAR = 0x2,
		AI_TT__AMBIENT = 0x3,
		AI_TT__EMISSIVE = 0x4,
		AI_TT__HEIGHT = 0x5,
		AI_TT__NORMALS = 0x6,
		AI_TT__SHININESS = 0x7,
		AI_TT__OPACITY = 0x8,
		AI_TT__DISPLACEMENT = 0x9,
		AI_TT__LIGHTMAP = 0xA,
		AI_TT__REFLECTION = 0xB,
		AI_TT__UNKNOWN = 0xC
	};

	/*! Implements an Assim format material which is quite generic.
	 * This material is intended to store a range of material effect that can capture the Assim
	 * I/O library. Therefore it is quite generic...
	 * Full description can be seen in \sa http://assimp.sourceforge.net/lib_html/materials.html
	 */
  class Assimp : public Material
  {
  public:

		/*! This structure details the necessary stuff to handle the texture... */
    /*! Construction from parameters. */
    Assimp (const Parms& parms)
    {
			name					= parms.getString("name","");
			Kd						= parms.getCol3f("Kd",zero);
			Ks						= parms.getCol3f("Ks",zero);
			Ka						= parms.getCol3f("Ka",zero);
			Ke						= parms.getCol3f("Ke",zero);
			shading				= parms.getInt("shading",AI_SM_GOURAUD);
			opacity				= parms.getFloat("opacity",1.);
			shininess			= parms.getFloat("shininess",0.);
			refract_ind		= parms.getFloat("refrac_ind",1.);
			// Texture fetching
			// Diffuse
			tex_stacks[AI_TT__DIFFUSE] = parms.getTextureStack("tx_stack_1");
		}

		/*! Shading for Assimp material
		 * \todo Handling Opacity !
		 * \todo Handling orean nayer + minnaert + fresnel shading model
		 * \todo Handling texture mapping
		 * \param ray incomming ray
		 * \param currentMedium medium from the ray
		 * \param dg point on the surface
		 * \param brdfs composited BRDF generated...
		 */
		void shade(const Ray& ray, const Medium& currentMedium, const DifferentialGeometry& dg, CompositedBRDF& brdfs) const
		{
			// Here we "just" handle reflection...
			// Ambiant illumination is the responsibility of the integrator only
			// Emissivity is the responsibility of the integrator also
			// For bump mapping, this is the parameter Ns in dg that needs to be changed...

			Col3f Kdif = Kd; // Add some other stuf here (texture mapping for example...)
			std::vector< Ref<Texture> > plop;
			if (tex_stacks.find(AI_TT__DIFFUSE) != tex_stacks.end()) {
				plop = tex_stacks.at(AI_TT__DIFFUSE);
				if (plop.size() > 0) {
					Kdif *= Kdif * (tex_stacks.at(AI_TT__DIFFUSE)[0])->get(dg.st);
				}
			}

			switch (shading) {
				case (AI_SM_NO_SHADING) :
					brdfs.add(NEW_BRDF(BlackMatter)(zero));
					break;
				case (AI_SM_FLAT) :
					brdfs.add(NEW_BRDF(BlackMatter)(Kdif));
					break;
				case (AI_SM_GOURAUD) :
				case (AI_SM_TOON) :
					// We handle that as pure lambertian shading
					// Toon effect will be done in the integrator
					brdfs.add(NEW_BRDF(Lambertian)(Kdif));
					break;
				case (AI_SM_PHONG) :
				case (AI_SM_BLINN) :
					// We handle that as pure lambertian shading...
					if (shininess == 0.) {
						brdfs.add(NEW_BRDF(Lambertian)(Kdif));
					}
					else {
						brdfs.add(NEW_BRDF(Lambertian)(Kdif));
						Col3f Kspec = Ks; // Add some other stuf here (texture mapping for example...)
						float Ns = shininess;	// Add some other stuf here (texture mapping for example...)
						brdfs.add(NEW_BRDF(Specular)(Kspec,Ns));
					}
					break;
				case (AI_SM_OREN_NAYAR) :
				case (AI_SM_COOK_TORRANCE) :
				case (AI_SM_FRESNEL) :
					std::cerr<<"UNDONE YET in Material::assimp !"<<std::endl;
					exit(1);
			}
		}

		Col3f getDiffuse() {return Kd;}

	private:
		std::string	name;					///< Name of the material (not usefull)
		Col3f				Kd;						///< Color for diffuse reflection. Default to black
		Col3f				Ks;						///< Color for specular reflection. Default to black
		Col3f				Ka;						///< Color for ambiant reflection. Default to black
		Col3f				Ke;						///< Color for emissivity. Default to black
		//Col3f			Kt;						///< Color for transparency reflection (?). NOT HANDLED
		//bool			wireframe 		///< Should the mesh be renderer with wireframe NOT HANDLED
		//bool			twosided;			///< Does the material should be applied on both side of the mesh. NOT HANDLED
		int 				shading;			///< Which kind of shading model for the shading model list.
		//int				blendFunc;		///< Describe which blending must be applied. NOT HANDLED
		float				opacity;			///< Opacity. Default to 1.0f
		float				shininess;		///< Shininess for specular (phong) reflection. Default to 0. means lambertian diffuse reflection.
		//float			shin_strength;///< Strength of shininess. NOT HANDLED
		float				refract_ind;	///< Index of refraction.
		///< All texture stacks indexed by \see embree::texture_type 
		std::map<int,std::vector<Ref<Texture> > > tex_stacks;

	};
}

#endif
