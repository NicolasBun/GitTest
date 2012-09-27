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

#include "openglrenderer.h"
#include "math/random.h"
#include "shapes/trianglemesh.h"
#include "shapes/trianglemesh_normals.h"
#include "shapes/triangle.h"
#include "cameras/pinholecamera.h"
#include "materials/assimp.h"
#include "materials/obj.h"
#include "materials/matte.h"
#include "materials/plastic.h"
#include "materials/dielectric.h"
#include "lights/trianglelight.h"
#include <typeinfo>


namespace embree
{
  uint OpenGLRenderer::fb_size_w = 128;
  uint OpenGLRenderer::fb_size_h = 128;
  bool OpenGLRenderer::is_initialized = false;
  unsigned char *OpenGLRenderer::color_buffer;
  uint OpenGLRenderer::img_render = 0;
  uint OpenGLRenderer::fbo_render = 0;
  uint OpenGLRenderer::fb_depth = 0;
  uint OpenGLRenderer::n_mesh = 0;
  std::vector<float*> OpenGLRenderer::mesh_positions;
  std::vector<float*> OpenGLRenderer::mesh_normals;
  ;
  void checkFramebufferStatus() {
    GLenum status;
    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      std::cout << "status:  GL_FRAMEBUFFER_COMPLETE_EXT  ...continuing" << std::endl;
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      std::cout << "status:  GL_FRAMEBUFFER_UNSUPPORTED_EXT  ...exiting" << std::endl;
      exit(0);
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      std::cout << "status : FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT  ...exiting" << std::endl;
      exit(0);
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      std::cout << "status : FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT  ...exiting" << std::endl;
      exit(0);
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      std::cout << "status : FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT  ...exiting" << std::endl;
      exit(0);
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      std::cout << "status : FRAMEBUFFER_INCOMPLETE_FORMATS_EXT  ...exiting" << std::endl;
      exit(0);
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      std::cout << "status : FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT  ...exiting" << std::endl;
      exit(0);
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      std::cout << "status : FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT  ...exiting" << std::endl;
      exit(0);
      break;
    default:
      std::cout << "Shouldn't reach this point.  Something is very wrong.  ...exiting" << std::endl;
      exit(0);
    }
  }


  OpenGLRenderer::OpenGLRenderer(const Parms& parms) {
    OpenGLRenderer::fb_size_w = parms.getInt("width",128);
    OpenGLRenderer::fb_size_h = parms.getInt("height",128);

    linedrawing = parms.getBool("linedraw",true);
  }

  void OpenGLRenderer::initGL(const Ref<BackendScene>& scene) {
    // Initialization of FBOs ...
   
    OpenGLRenderer::color_buffer = new unsigned char[3*fb_size_w*fb_size_h];
    if(!GLEW_EXT_framebuffer_object) {
      std::cerr<<"FBO is not supported!"<<std::endl;
      exit(5);
    }
    glGenFramebuffersEXT(1, &OpenGLRenderer::fbo_render);

    glGenTextures(1,&OpenGLRenderer::img_render);
    glBindTexture(GL_TEXTURE_2D,OpenGLRenderer::img_render);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,OpenGLRenderer::fb_size_w,OpenGLRenderer::fb_size_h,0,
                  GL_RGB,GL_UNSIGNED_BYTE,OpenGLRenderer::color_buffer);
    glBindTexture(GL_TEXTURE_2D,0);

    // Bind the framebuffer...
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_render);
    // Create a Renderbuffer object
    glGenRenderbuffersEXT(1, &fb_depth);
    // Initialize and activate buffer rendering
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fb_depth);
    // Create the rendering buffer (used as depth buffer here)
    //cerr<<"Size introduced : "<<image_source->width()<<" / "<<image_source->height()<<endl;
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, OpenGLRenderer::fb_size_w, OpenGLRenderer::fb_size_h);
    // Attach rendering buffer to framebuffer object
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fb_depth);

	        //std::cout << "     embree::checkFramebufferStatus(); " << std::endl;

    //embree::checkFramebufferStatus();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    //! Storing all mesh in array to optimize (a little bit) the rendering process
    n_mesh = 0;
    std::vector< Ref< Instance > > geom = scene->geometry;
    Ref<Shape> one_shape;
    //std::cerr<<"Initialization ..."<<geom.size()<<std::endl;
    for(std::vector< Ref< Instance > >::iterator it = geom.begin();it != geom.end();it++) {
      one_shape = (*it)->shape;
      if (TriangleMesh* tm = dynamic_cast<TriangleMesh*>(one_shape.ptr)) {
        n_mesh++;
        //! We are force to copy the data...
        float *points = new float[3*tm->position.size()];
        for(uint i=0;i<tm->position.size();i++) {
          points[3*i]   = tm->position[i].x;
          points[3*i+1] = tm->position[i].y;
          points[3*i+2] = tm->position[i].z;
        }
        mesh_positions.push_back(points);
        if (tm->normal.size()>0) {
          float *normals = new float[3*tm->normal.size()];
          for(uint i=0;i<tm->normal.size();i++) {
            normals[3*i]   = tm->normal[i].x;
            normals[3*i+1] = tm->normal[i].y;
            normals[3*i+2] = tm->normal[i].z;
          }
          mesh_normals.push_back(normals);
        }
        else {
          mesh_normals.push_back(NULL);
        }
      }
      else if (Triangle* tr = dynamic_cast<Triangle*>(one_shape.ptr)) {
        // Nothing to do here : we do not store simple triangles...
      }
      else if (TriangleMeshWithNormals* trn = dynamic_cast<TriangleMeshWithNormals*>(one_shape.ptr)) {
        n_mesh++;
        
        //std::cerr<<"WE have a triangle mesh normal n°"<<n_mesh<<std::endl;
        //! We are force to copy the data...
        float *points = new float[3*trn->vertices.size()];
        float *normals = new float[3*trn->vertices.size()];
        for(uint i=0;i<trn->vertices.size();i++) {
          normals[3*i]   = trn->vertices[i].n.x;
          normals[3*i+1] = trn->vertices[i].n.y;
          normals[3*i+2] = trn->vertices[i].n.z;
          points [3*i]   = trn->vertices[i].p.x;
          points [3*i+1] = trn->vertices[i].p.y;
          points [3*i+2] = trn->vertices[i].p.z;
        }
        mesh_positions.push_back(points);
        mesh_normals.push_back(normals);
      }
      //! Handle here all other cases
      else {
        std::cerr<<" Is a "<<typeid(*one_shape).name()<<std::endl;
      }
    }
    glFinish();
    OpenGLRenderer::is_initialized = true;
  }

  void handleMaterial(const Ref<Material>& mat) {
    Col3f color(1.0,1.0,1.0);
    CompositedBRDF brdfs;
    
    if (Assimp* mat_assimp = dynamic_cast<Assimp*>(mat.ptr)) {
      color = mat_assimp->getDiffuse();
    }
    if (Matte* mat_matte = dynamic_cast<Matte*>(mat.ptr)) {
      color = mat_matte->getDiffuse();
    }
    if (Obj* mat_obj = dynamic_cast<Obj*>(mat.ptr)) {
      color = mat_obj->getDiffuse();
    }
    if (Plastic* mat_plastic = dynamic_cast<Plastic*>(mat.ptr))
    {
		color = mat_plastic->getPigmentColor(); 
	}
	/*if (Dielectric* mat_dielectric = dynamic_cast<Dielectric*>(mat.ptr))
	{
		
	}*/
		
    glColor3f(color.r,color.g,color.b);
    float data[3];
    float black[3] = {0.};
    data[0] = color.r;data[1]=color.g;data[2]=color.b;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,data);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
  }

  void handleLight(uint n_lght,const Ref<Light>& lght) {
    float data[4];
    float black[3] = {0.};
    DifferentialGeometry dg;
    if (TriangleLight* lght_tr = dynamic_cast<TriangleLight*>(lght.ptr)) 
    {
      glEnable(GL_LIGHT0+n_lght);
      Vec3f lpos = (lght_tr->v0+lght_tr->v1+lght_tr->v2)/3.0f;
      //std::cerr<<"Handle light ("<<n_lght<<"): "<<lpos<<std::endl;
      data[0] = lpos.x;data[1]=lpos.y;data[2]=lpos.z;data[3]=1.0;
      glLightfv(GL_LIGHT0+n_lght,GL_POSITION,data);
      Col3f L = lght_tr->Le(dg,lpos);
      data[0] = L.r;data[1]=L.g;data[2]=L.b;
      glLightfv(GL_LIGHT0+n_lght,GL_DIFFUSE,data);
      glLightfv(GL_LIGHT0+n_lght,GL_AMBIENT,black);
      glLightfv(GL_LIGHT0+n_lght,GL_SPECULAR,black);
      glLightf(GL_LIGHT0+n_lght,GL_CONSTANT_ATTENUATION,0.0);
      glLightf(GL_LIGHT0+n_lght,GL_LINEAR_ATTENUATION,1.0);
      glLightf(GL_LIGHT0+n_lght,GL_QUADRATIC_ATTENUATION,0.0);
    }
  }

  void drawTriangle(const Triangle* tr) {
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_TRIANGLES);
      glNormal3f(tr->Ng.x,tr->Ng.y,tr->Ng.z);
      glVertex3f(tr->v0.x,tr->v0.y,tr->v0.z);
      glVertex3f(tr->v1.x,tr->v1.y,tr->v1.z);
      glVertex3f(tr->v2.x,tr->v2.y,tr->v2.z);
    glEnd();
  }

  void OpenGLRenderer::drawTriangleMesh(uint id_mesh,const TriangleMesh* tm) {
    uint nbrFace = tm->triangles.size();
    uint *indices = (uint*)((tm->triangles).begin());
    bool hasNormal = (mesh_normals[id_mesh]!=NULL);

    glEnableClientState(GL_VERTEX_ARRAY);
    if (hasNormal) glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,mesh_positions[id_mesh]);
    if (hasNormal) glNormalPointer(GL_FLOAT,0,mesh_normals[id_mesh]);
    glDrawElements(GL_TRIANGLES,3*nbrFace,GL_UNSIGNED_INT,indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (hasNormal) glDisableClientState(GL_VERTEX_ARRAY);
  }

  void OpenGLRenderer::drawTriangleMeshNormals(uint id_mesh,const TriangleMeshWithNormals* trn) {
    uint nbrFace = trn->triangles.size();
    uint *indices = (uint*)((trn->triangles).begin());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,mesh_positions[id_mesh]);
    glNormalPointer(GL_FLOAT,0,mesh_normals[id_mesh]);
    glDrawElements(GL_TRIANGLES,3*nbrFace,GL_UNSIGNED_INT,indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  void OpenGLRenderer::renderFrame(const Ref<Camera>& camera, const Ref<BackendScene>& scene,  Ref<Film>& film)
  {
    /*! render frame */
    double t = getSeconds();
    this->camera = camera;
    this->scene = scene;
    this->film = film;

    /*! precompute some values */
    
    if (!OpenGLRenderer::is_initialized){
      OpenGLRenderer::initGL(scene);
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, OpenGLRenderer::fbo_render);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D,OpenGLRenderer::img_render, 0);
                                  
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (linedrawing) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glViewport(0,0,fb_size_w,fb_size_h);

    PinholeCamera* cam = dynamic_cast<PinholeCamera*>(camera.ptr);
    if (!cam) {
      std::cerr<<"OpenGL Renderer use only pinehole camera ! "<<std::endl;
      exit(1);
    }
    AffineSpace worldToCam = rcp(cam->localToWorld);

    float mat[16];
    worldToCam.to4x4ArrayMatrix(mat);

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    //std::cerr<<"Far plane ..."<<std::endl;
    gluPerspective(cam->angle,cam->aspectRatio,cam->focalDistance,1000.0*cam->focalDistance);
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadMatrixf(mat);
    //glMultMatrixf(mat);

    //! Handle lights
    if (scene->allLights.size()>0) {
      glEnable(GL_LIGHTING);
      uint ind_lght = 0;
      std::vector< Ref< Light > > lights = scene->allLights;
      for(std::vector< Ref< Light > >::iterator it = lights.begin();it != lights.end();it++) {
        handleLight(ind_lght++,*it);
      }
    }

    glColor3f(1.0,0.0,0.0);
    std::vector< Ref< Instance > > geom = scene->geometry;
    Ref<Shape> one_shape;
    uint ind_mesh = 0;
    
    for(std::vector< Ref< Instance > >::iterator it = geom.begin();it != geom.end();it++) {
      one_shape = (*it)->shape;
      handleMaterial((*it)->material);
      if (TriangleMesh* tm = dynamic_cast<TriangleMesh*>(one_shape.ptr)) {
        drawTriangleMesh(ind_mesh++,tm);
      }
      else if (Triangle* tr = dynamic_cast<Triangle*>(one_shape.ptr)) {
        drawTriangle(tr);
      }
      //! Handle here all other cases
      else if (TriangleMeshWithNormals* trn = dynamic_cast<TriangleMeshWithNormals*>(one_shape.ptr)) {
        drawTriangleMeshNormals(ind_mesh++,trn);
      }
      else {
        //std::cerr<<" Is a "<<typeid(*one_shape).name()<<std::endl;
      }
    }

    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glFinish();
    //! Copy of the GPU framebuffer to the CPU color buffer...
    glReadPixels(0,0,OpenGLRenderer::fb_size_w,OpenGLRenderer::fb_size_h,GL_RGB,
                  GL_UNSIGNED_BYTE,OpenGLRenderer::color_buffer);
    glFinish();
    glDisable(GL_DEPTH_TEST);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glDisable(GL_DEPTH_TEST);
    //! Copy of the CPU color buffer to the film...
    Col3f colcour;
    for(uint i=0;i<fb_size_w;i++) {
      for(uint j=0;j<fb_size_h;j++) {
        colcour.r = OpenGLRenderer::color_buffer[(fb_size_h-1-j)*3*fb_size_w+3*i]/255.0;
        colcour.g = OpenGLRenderer::color_buffer[(fb_size_h-1-j)*3*fb_size_w+3*i+1]/255.0;
        colcour.b = OpenGLRenderer::color_buffer[(fb_size_h-1-j)*3*fb_size_w+3*i+2]/255.0;
        film->set(i,j,colcour);
      }
    }
    this->camera = null;
    this->scene = null;
    this->film = null;
    double dt = getSeconds()-t;

    /*! print framerate */
    std::cout << "\r" << 1.0f/dt << " fps, " << dt*1000.0f << " ms" ;//<< std::endl;
  }
}
