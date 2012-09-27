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

#ifndef __EMBREE_OPENGL_RENDERER_H__
#define __EMBREE_OPENGL_RENDERER_H__

/* include GLUT for display */
#include <GL/glew.h>
#include <GL/glext.h>

#include "renderers/renderer.h"
#include "shapes/trianglemesh.h"
#include "shapes/trianglemesh_normals.h"

namespace embree
{
  /*! OpenGL renderer for testing the scene.
	 * This particular rendered allows to use OpenGL for preview of the scene. It handles simple illumination.
	 * We use OpenGL version 2.1 for convenience.
	 * Some geometric data are stored at initialization so the scene can not be modified after this initialization.
	 * \todo Switch to OpenGL 3+
	 */
  class OpenGLRenderer : public Renderer
  {

  public:

    /*! Construction from parameters. */
    OpenGLRenderer (const Parms& parms);

    /*! Renders a single frame. */
    void renderFrame(const Ref<Camera>& camera, const Ref<BackendScene>& scene, Ref<Film>& film);

  private:
		void initGL(const Ref<BackendScene>& scene);
		void drawTriangleMesh(uint id_mesh,const TriangleMesh* tm);
		void drawTriangleMeshNormals(uint id_mesh,const TriangleMeshWithNormals* trn);
		
    /*! Configuration */
  private:
		//!< Flag to know is GL elements (FBO & texture) are initialized (we have to wait for the GL context to be created...)
		static bool is_initialized;
		static unsigned int fbo_render;			//!< FBO with indirect rendering
		static unsigned int fb_depth;			//!< FBO with indirect rendering
		static unsigned int img_render;			//!< Texture to store the image
		static unsigned int fb_size_w;			//!< FB width
		static unsigned int fb_size_h;			//!< FB height
		static unsigned char* color_buffer;	//!< CPU color buffer
		static unsigned int n_mesh;					//!< nb meshes...
		static std::vector<float*> mesh_positions;		//!< One position array for each mesh (triangle mesh)
		static std::vector<float*> mesh_normals;			//!< One normal array for each (triangle mesh)

		bool linedrawing;

		/*! Arguments of renderFrame function */
  private:
    Ref<Camera> camera;            //!< Camera to render from
    Ref<BackendScene> scene;       //!< Scene to render
    Ref<Film> film;                //!< Framebuffer to render into

    /*! Precomputations. */
  private:
  };
}

#endif
