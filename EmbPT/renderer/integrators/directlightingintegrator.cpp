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

#include "integrators/directlightingintegrator.h"

namespace embree
{
  DirectLightingIntegrator::DirectLightingIntegrator(const Parms& parms)
    : lightSampleID(-1), firstScatterSampleID(-1), firstScatterTypeSampleID(-1)
  {
    maxDepth        = parms.getInt  ("maxDepth"       ,1);
    minContribution = parms.getFloat("minContribution",0.001f );
    epsilon         = parms.getFloat("epsilon"        ,128.0f)*float(ulp);
    backplate       = parms.getImage("backplate");
  }

  void DirectLightingIntegrator::requestSamples(Ref<SamplerFactory>& samplerFactory, const Ref<BackendScene>& scene)
  {
    precomputedLightSampleID.resize(scene->allLights.size());

    lightSampleID = samplerFactory->request2D();
    for (size_t i=0; i<scene->allLights.size(); i++) {
      precomputedLightSampleID[i] = -1;
      if (scene->allLights[i]->precompute())
        precomputedLightSampleID[i] = samplerFactory->requestLightSample(lightSampleID, scene->allLights[i]);
    }
    firstScatterSampleID = samplerFactory->request2D((int)maxDepth);
    firstScatterTypeSampleID = samplerFactory->request1D((int)maxDepth);
  }

  Col3f DirectLightingIntegrator::Li(const Ray& ray, const Ref<BackendScene>& scene, Sampler* sampler, size_t& numRays)
  {
		BRDFType directLightingBRDFTypes = (BRDFType)(DIFFUSE);

		Col3f L = zero;

		/*! Traverse ray. */
		DifferentialGeometry dg;
		scene->accel->intersect(ray,dg);
		scene->postIntersect(ray,dg);
		const Vec3f wo = ray.rdir;
		numRays++;

		/*! Environment shading when nothing hit. */
		if (!dg)
		{
			if (backplate) {
				Vec2f raster = sampler->getPrimary();
				int width = sampler->getImageSize().x;
				int height = sampler->getImageSize().y;
				int x = (int)((raster.x / width) * backplate->width);
				x = clamp(x, 0, int(backplate->width)-1);
				int y = (int)((raster.y / height) * backplate->height);
				y = clamp(y, 0, int(backplate->height)-1);
				L = backplate->get(x, y);
			}
			else {
				for (size_t i=0; i<scene->envLights.size(); i++)
					L += scene->envLights[i]->Le(wo);
			}
			return L;
		}

		/*! Shade surface. */
		CompositedBRDF brdfs;
		if (dg.material) dg.material->shade(ray, Medium::Vacuum(), dg, brdfs);

		/*! face forward normals */
		bool backfacing = false;
		if (dot(dg.Ng, ray.dir) > 0) {
			backfacing = true; dg.Ng = -dg.Ng; dg.Ns = -dg.Ns;
		}

		/*! Add light emitted by hit area light source. */
		if (/*!lightPath.ignoreVisibleLights && */dg.light && !backfacing)
			L += dg.light->Le(dg,wo);

		/*! Global illumination. Pick one BRDF component and sample it. */
		// NOT DONE since it's direct illumination

		/*! Check if any BRDF component uses direct lighting. */
		for (size_t i=0; i<scene->allLights.size(); i++)
		{
			/*! Either use precomputed samples for the light or sample light now. */
			LightSample ls;
			if (scene->allLights[i]->precompute()) ls = sampler->getLightSample(precomputedLightSampleID[i]);
			else ls.L = scene->allLights[i]->sample(dg, ls.wi, ls.tMax, sampler->getVec2f(lightSampleID));

			/*! Ignore zero radiance or illumination from the back. */
			if (ls.L == Col3f(zero) || ls.wi.pdf == 0.0f || dot(dg.Ns,Vec3f(ls.wi)) <= 0.0f) continue;

			/*! Test for shadows. */
			bool inShadow = scene->accel->occluded(Ray(dg.P, ls.wi, dg.error*epsilon, ls.tMax-dg.error*epsilon));
			numRays++;
			if (inShadow) continue;

			/*! Evaluate BRDF. */
			L += ls.L * brdfs.eval(wo, dg, ls.wi, directLightingBRDFTypes) * rcp(ls.wi.pdf);
		}

    return L;
  }

}

