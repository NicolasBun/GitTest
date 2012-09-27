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

#include <string.h>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <ostream>
#include <assimp/assimp.hpp>
#include <assimp/aiPostProcess.h>
#include <assimp/aiMaterial.h>
#include <assimp/aiScene.h>
#include <assimp/aiMatrix4x4.h>

#include "sys/platform.h"
#include "sys/filename.h"
#include "image/image.h"
#include "scene.h"
#include "embreedevice.h"

namespace embree
{
	/*! device to create meshes and materials */
	extern Ref<Device> g_device;

	Ref<Device::RTImage> loadRTImage(const FileName fileName);
	
	/*! texture parameters struct for Assimp texture */
	
	struct tex_parameters {
		float strength;
		// Add here other things to handle like TexOp TexMapping TexBlend...
	};
	
	/*! Specific load texture for Assimp Texture.
	 * \todo Find a more convenient way of dealing with "complex" texture without changing the whole architecture
	 * */
	Ref<Device::RTTexture> loadTextureAssimp(const FileName fileName,const struct tex_parameters* pars) {
		static std::map<std::string,Ref<Device::RTTexture> > texture_map;

		if (texture_map.find(fileName.str()) != texture_map.end())
			return texture_map[fileName.str()];

		Ref<Device::RTTexture> texture = g_device->rtNewTexture("assimp");
		texture->rtSetFloat1("strength",pars->strength);
		texture->rtSetImage("image",loadRTImage(fileName));
		texture->rtCommit();

		return texture_map[fileName.str()] = texture;
	}

	void showTransfo(const aiMatrix4x4& t) {
		std::cerr<<t.a1<<" "<<t.a2<<" "<<t.a3<<" "<<t.a4<<std::endl;
		std::cerr<<t.b1<<" "<<t.b2<<" "<<t.b3<<" "<<t.b4<<std::endl;
		std::cerr<<t.c1<<" "<<t.c2<<" "<<t.c3<<" "<<t.c4<<std::endl;
		std::cerr<<t.d1<<" "<<t.d2<<" "<<t.d3<<" "<<t.d4<<std::endl;
	}
	
	struct AIImporter {
		/*! directory of input file */
		FileName path;

		/*! output model */
		Ref<GroupNode> model;

		/*! material handling */
		Ref<Device::RTMaterial> curMaterial;
		std::vector<Ref<Device::RTMaterial> > material;

		Vec3f bbox_minimal;
		Vec3f bbox_maximal;


		/*! HANDLING TEXTURES */
		void handleATypeOfTexture(aiTextureType typ,aiMaterial* cur_mat,Ref<Device::RTMaterial> cur) {
			uint nb;
			aiString filename_tx;
			aiTextureMapping mapping;
			uint uvind;
			aiTextureOp op;
			aiTextureMapMode mapmode;
			float strength;
			std::vector<Ref<Device::RTTexture> > texstack;
			struct tex_parameters cur_parameters;

			nb = cur_mat->GetTextureCount(typ);
			if (nb==0) return;
			//std::cerr<<"Type tex "<<typ<<" has "<<nb<<" textures"<<std::endl;
			for(uint i=0;i<nb;i++) {
				// Store texture in stack
				cur_mat->GetTexture(typ,i,&filename_tx,&mapping,&uvind,
														&strength,&op,&mapmode);
				cur_parameters.strength = strength;
				texstack.push_back(loadTextureAssimp(path+std::string(filename_tx.data),&cur_parameters));
			}
			
			char numstr[21]; // enough to hold all numbers up to 64-bits
			sprintf(numstr, "%d", typ);
			std::string code = std::string("tx_stack_") + numstr;

			cur->rtSetTextureStack(code.c_str(),texstack);
		}
		
		/*! Loading all material from aiScene.
		 * \todo Handling some specific caracteristic (op, mapping) for textures...
		 */
		void handleMaterials(const aiScene* scene) {
			Ref<Device::RTMaterial> cur = null;
			aiMaterial* cur_mat;
			aiString cur_mat_name;

			// We consider that there is no material duplicates
			for(uint i=0;i<scene->mNumMaterials;i++) {
				cur_mat = scene->mMaterials[i];
				cur_mat->Get(AI_MATKEY_NAME,cur_mat_name);
				// Material is not stored
				cur = g_device->rtNewMaterial("Assimp");
				material.push_back(cur);
				
				// All parameters handling

				aiColor3D res(0.,0.,0.1);
				int res_i = 0;
				float res_f = 0.;
				cur->rtSetString("name" ,cur_mat_name.data);

				cur_mat->Get(AI_MATKEY_COLOR_DIFFUSE,res);
				cur->rtSetFloat3("Kd",Vec3f(res.r,res.g,res.b));
				cur_mat->Get(AI_MATKEY_COLOR_SPECULAR,res);
				cur->rtSetFloat3("Ks",Vec3f(res.r,res.g,res.b));
				cur_mat->Get(AI_MATKEY_COLOR_AMBIENT,res);
				cur->rtSetFloat3("Ka",Vec3f(res.r,res.g,res.b));
				cur_mat->Get(AI_MATKEY_COLOR_EMISSIVE,res);
				cur->rtSetFloat3("Ke",Vec3f(res.r,res.g,res.b));

				cur_mat->Get(AI_MATKEY_SHADING_MODEL,res_i);
				cur->rtSetInt1("shading",res_i);

				cur_mat->Get(AI_MATKEY_OPACITY,res_f);
				cur->rtSetFloat1("opacity",res_f);
				cur_mat->Get(AI_MATKEY_SHININESS,res_f);
				cur->rtSetFloat1("shininess",res_f);
				cur_mat->Get(AI_MATKEY_REFRACTI,res_f);
				cur->rtSetFloat1("refract_ind",res_f);

				// Handling texture stacks ...
				// For each type of texture, we store them...
				handleATypeOfTexture(aiTextureType_DIFFUSE,cur_mat,cur);			// Type 1
				handleATypeOfTexture(aiTextureType_SPECULAR,cur_mat,cur);			// Type 2
				handleATypeOfTexture(aiTextureType_AMBIENT,cur_mat,cur);			// Type 3
				handleATypeOfTexture(aiTextureType_EMISSIVE,cur_mat,cur);			// Type 4
				handleATypeOfTexture(aiTextureType_HEIGHT,cur_mat,cur);				// Type 5
				handleATypeOfTexture(aiTextureType_NORMALS,cur_mat,cur);			// Type 6
				handleATypeOfTexture(aiTextureType_SHININESS,cur_mat,cur);		// etc.
				handleATypeOfTexture(aiTextureType_OPACITY,cur_mat,cur);			//
				handleATypeOfTexture(aiTextureType_DISPLACEMENT,cur_mat,cur);	//
				handleATypeOfTexture(aiTextureType_LIGHTMAP,cur_mat,cur);			//
				handleATypeOfTexture(aiTextureType_REFLECTION,cur_mat,cur);		//
				handleATypeOfTexture(aiTextureType_UNKNOWN,cur_mat,cur);			//

				cur->rtCommit();
			}
		}

		/*! Handling one mesh.
		 * \todo Collect several tx coordinates
		 * \todo Collect tangents & bitangents
		 * \todo Handle 1D and 3D tx coordinates
		 */
		void handleAMesh(const aiScene* scn,uint mesh_indx,const aiMatrix4x4& transf) {
			//! WARNING : We do not take care of sub_object in this case...

			const aiMesh* the_mesh = scn->mMeshes[mesh_indx];
			//std::cerr<<"This mesh is "<<the_mesh->mName.data<<" ("<<mesh_indx<<")"<<std::endl;
			if(the_mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
				std::cerr<<"This mesh is not a triangle mesh ! We do not care !!!"<<std::endl;
				return;
			}
			if (!the_mesh->HasFaces()) {
				std::cerr<<"This mesh has no face ! We do not care !!!"<<std::endl;
				return;
			}
			// Collect coordinates, normals and uvtexcoords (if any)
      // temporary data arrays
      std::vector<Vec3f> positions;
			positions.clear();
      std::vector<Vec3f> normals;
			normals.clear();
      std::vector<Vec2f> texcoords;
      std::vector<Vec3i> triangles;
			triangles.clear();
			aiMatrix3x3 normalMatrix(transf);
			normalMatrix.Inverse();
			normalMatrix.Transpose();

			/* create triangle mesh */
      Ref<Device::RTShape> mesh = g_device->rtNewShape("trianglemesh");
			/* Recover coordinates, normals and texcoord (only one) in positions array */
			aiVector3D p,n,tx;
			
			for(uint i=0;i<the_mesh->mNumVertices;i++) {
				p = transf*the_mesh->mVertices[i];

				bbox_minimal = min(bbox_minimal,Vec3f(p.x,p.y,p.z));
				bbox_maximal = max(bbox_maximal,Vec3f(p.x,p.y,p.z));
				positions.push_back(Vec3f(p.x,p.y,p.z));

				if (the_mesh->HasNormals()) {
					//std::cerr<<"At charge : one mesh has normals..."<<std::endl;
					n = normalMatrix*the_mesh->mNormals[i];
					//std::cerr<<Vec3f(n.x,n.y,n.z)<<std::endl;
					normals.push_back(Vec3f(n.x,n.y,n.z));
				}
			}
			// We do a separate loop for tx coordinates to avoid too much test
			// TODO Transform to a for loop
			if (the_mesh->HasTextureCoords(0)) {
				// Check dimension of tex coordinates
				if (the_mesh->mNumUVComponents[0] == 2) {
					for(uint i=0;i<the_mesh->mNumVertices;i++) {
						tx = the_mesh->mTextureCoords[0][i];
						texcoords.push_back(Vec2f(tx.x,tx.y));
					}
				}
				// Handle other dimension (1D/3D... Textures) here
			}
			// We handle triangle faces
			for(uint i=0;i<the_mesh->mNumFaces;i++) {
				if ((the_mesh->mFaces[i]).mNumIndices != 3) {
					std::cerr<<"We got a face which is not a triangle ! Abort !"<<std::endl;
					exit(1);
				}
				triangles.push_back(Vec3i((the_mesh->mFaces[i]).mIndices[0],(the_mesh->mFaces[i]).mIndices[1],(the_mesh->mFaces[i]).mIndices[2]));
			}
			
			mesh->rtSetArray("positions","float3",&positions[0],positions.size(),sizeof(Vec3f));
			mesh->rtSetArray("indices"  ,"int3"  ,&triangles[0],triangles.size(),sizeof(Vec3i));
			if (normals.size()  ) {mesh->rtSetArray("normals"  ,"float3",&normals[0]  ,normals.size()  ,sizeof(Vec3f));}
			if (texcoords.size()) mesh->rtSetArray("texcoords","float2",&texcoords[0],texcoords.size(),sizeof(Vec2f));
			
			mesh->rtCommit();

			*model += new ShapeNode(mesh,material[the_mesh->mMaterialIndex]);
			return;
		}


		/*! Handling one node of the graph scene.
		 */
		void handleANode(const aiScene* scn,aiNode* nod,const aiMatrix4x4& transf,int bs) {
			// Handling transformation
			aiMatrix4x4 cur_trans = nod->mTransformation*transf;

			//std::cerr<<"This node "<<nod->mName.data<<" has "<<nod->mNumMeshes<<" meshes"<<std::endl;
			for(uint i=0;i<nod->mNumMeshes;i++) {
				handleAMesh(scn,nod->mMeshes[i],cur_trans);
			}
			for(uint i=0;i<nod->mNumChildren;i++) {
				handleANode(scn,nod->mChildren[i],cur_trans,bs+2);
			}
			return;
		}
		
		
		AIImporter(const FileName& fileName) : path(fileName.path()), model(new GroupNode)		{
			std::cerr<<"Begin to read file "<<fileName<<" with Assimp support"<<std::endl;
			// Create an instance of the Importer class. Should be destroy at the end...
			Assimp::Importer importer;
			// Create a default material
      Ref<Device::RTMaterial> defaultMaterial = g_device->rtNewMaterial("matte");
      defaultMaterial->rtSetFloat3("reflectance",Vec3f(0.5f));
      defaultMaterial->rtCommit();
      curMaterial = defaultMaterial;

			// Update the bbox
			bbox_minimal = Vec3f(1e10,1e10,1e10);
			bbox_maximal = Vec3f(-1e10,-1e10,-1e10);
			
			// And have it read the given file with some example postprocessing
			// Usually - if speed is not the most important aspect for you - you'll
			// propably to request more postprocessing than we do in this example.
			const aiScene* scene = importer.ReadFile( fileName.c_str(),
																								aiProcess_Triangulate            |
																								aiProcess_JoinIdenticalVertices  |
																								aiProcess_GenNormals             |
/*																								aiProcess_PreTransformVertices   |*/
																								aiProcess_SortByPType);
			// If the import failed, report it
			if(!scene)
			{
				std::cerr<<"ERROR AI Import : "<<importer.GetErrorString()<<std::endl;
				exit(1);
			}

			// Handling scene...

			/// \note We could also handle all texture at once 

			// First of all : Handling materials
			if (scene->HasMaterials()) {
				std::cerr<<"Nb of material : "<<scene->mNumMaterials<<std::endl;
				handleMaterials(scene);
			}
			else {
				std::cerr<<"No material in the scene !"<<std::endl;
			}

			// Secondly : handle meshes
			if (scene->HasMeshes()/* && ((scene->mRootNode) != null)*/) {
				// We have to cross the scene graph
				aiNode* cur_node;
				std::cerr<<"We have "<<scene->mNumMeshes<<" meshes "<<std::endl;
				cur_node = scene->mRootNode;
				aiMatrix4x4 mat;
				handleANode(scene,cur_node,mat,0);
			}
			else {
				std::cerr<<"No mesh in the scene !"<<std::endl;
			}
			//*
			std::cerr<<"Bounding Box : "<<std::endl;
			std::cerr<<bbox_minimal.x<<" / "<<bbox_maximal.x<<std::endl;
			std::cerr<<bbox_minimal.y<<" / "<<bbox_maximal.y<<std::endl;
			std::cerr<<bbox_minimal.z<<" / "<<bbox_maximal.z<<std::endl;
			//*/

			std::cerr<<"Loading done ..."<<std::endl;
		}
		
	};

	Ref<Scene> loadAI(const FileName& fileName) {
		return AIImporter(fileName).model.cast<Scene>();
	}
}
