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

#ifndef __EMBREE_BLACK_MATTER_BRDF_H__
#define __EMBREE_BLACK_MATTER_BRDF_H__

#include "brdfs/brdf.h"

namespace embree
{
  /*! Black Matter BRDF. A black matter surface is a surface that reflects
   *  nothing, converting all received energy to heat. We use this BRDF for a 
   *  simple flat shading since we return the "color" of the surface.
	 *  Used for simple coloring case like flat painting...
	 * \warning This is not physically possible unless the color is black.
	 */
  class BlackMatter : public BRDF
  {
  public:

    /*! BlackMatter BRDF constructor. This is a diffuse reflection BRDF. */
    __forceinline BlackMatter(const Col3f& R) : BRDF(DIFFUSE_REFLECTION), R(R) {}

    __forceinline Col3f eval(const Vec3f& wo, const DifferentialGeometry& dg, const Vec3f& wi) const {
      return R;
    }

    Col3f sample(const Vec3f& wo, const DifferentialGeometry& dg, Sample3f& wi, const Vec2f& s) const {
      return eval(wo, dg, wo/* this one does matter */);
    }

    float pdf(const Vec3f& wo, const DifferentialGeometry& dg, const Vec3f& wi) const {
      return 1.0;
    }

  private:

    /*! The color parameter.*/
    Col3f R;
  };
}

#endif
