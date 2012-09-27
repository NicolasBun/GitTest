#include <string.h>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h> // for C qsort 
#include <ostream>
#include <list>

#include "sys/platform.h"
#include "sys/filename.h"
#include "image/image.h"
#include "scene.h"
#include "embreedevice.h"


#define TECHNIQUE 1;

namespace embree
{
	/*! device to create meshes and materials */
	extern Ref<Device> g_device;
	
	typedef struct _seg {
		int a, b;
		double angle;
	} seg; 
	
	//Struct for now...
	class Vector
	{
		public:
			
			double x, y,z;

			Vector(double f = 0.0f)
				: x(f), y(f), z(f) {}

			Vector(double x, double y, double z)
				: x(x), y(y), z(z) {}
				
			double GetX(void) const 
			{ 
			  return x; 
			};
		  
			double GetY(void) const 
			{ 
			  return y; 
			};
			
			void  Set(float _x,float _y)
			{
				x = _x;
				y = _y;
			};
			
			float Dot(Vector v)
			{
				return (v.x * x) + (v.y * y) + (v.z * z);
			};
		
			Vector Cross(Vector v)
			{
				Vector r;
			
				r.x = y * v.z - v.y *z;
				r.y = z * v.x - v.z * x;
				r.z = x * v.y - v.x * y; 
			
				return r;
			}
			
			Vector Dist(Vector v)
			{
				Vector r;
				
				r.x = v.x - x;
				r.y = v.y - y;
				r.z = v.z - z;
				
				return r;
			}
			
			
	};
		
	struct RadImporter
	{
		static const float EPSILON=0.0000000001f;
		
		//Polygone triangulation Typedef
		typedef std::vector< Vector > Vector3dVector;
		typedef std::vector< int > VectorInt;
		
		//Material can have multiples names
		struct Alias
		{
			int id;
			std::vector<std::string> aliasName;
		};
		
		struct ITRIANGLE
		{
		  int p1, p2, p3;
		};
		
		FileName path;
		Ref<GroupNode> model;
		Ref<Device::RTMaterial> curMaterial;
		
		std::vector<Ref<Device::RTMaterial> > materials;
			
		size_t searchComment;
		size_t searchSpace; 
		size_t searchVoid;
		
		std::string strArgument;
		
		//Useful for detect character on each line of rad file
		int ind;
		int oldind;
		int nbTri;
		int idMaterial;
		
		float epsilon;

		std::vector<Alias> listMaterial;
								
		RadImporter(const FileName& fileName, std::ifstream& f2) : path(fileName.path()), model(new GroupNode)
		{
			
			epsilon = 0.000001f;

			//Ref<Device::RTMaterial> defaultMaterial = g_device->rtNewMaterial("matte");
			//defaultMaterial->rtSetFloat3("reflectance",Vec3f(0.5f));
			//std::cout << "in struct RadImporter s = > " << s << std::endl;
			//defaultMaterial->rtCommit();
			//curMaterial = defaultMaterial;
						
			int nbMat = 0;
			
			nbTri = 0;
			
			std::string s(fileName);
			std::string contenu;
						
			while(std::getline(f2, contenu))
			{
				//Search specifics arguments
				size_t  searchScene = contenu.find("scene");
				size_t  searchMaterial = contenu.find("material");
				
				if (searchScene != std::string::npos)
				{
					searchScene = contenu.find("=");
					if (searchScene != std::string::npos)
					{
						ind = (int) searchScene;
						oldind = ind;
						ind += 2;
						
						std::string  text = fileName.path();
						text += "/";
						text += contenu.substr(ind, contenu.length() - ind);
						
						//Read the rad file		
						const char * c = text.c_str();
						std::ifstream f(c, std::ios::in);
						
						if (f)
						{
							std::string scriptText;
							while(std::getline(f, scriptText))
							{
								//Comment
								searchComment = scriptText.find("#");
								if (!(searchComment != std::string::npos))
								{
									if (scriptText.length() > 0)
									{
										size_t foundShapes = scriptText.find("polygon");
										searchSpace = scriptText.find(" ");		
										if (foundShapes != std::string::npos)
										{
											if(int(searchSpace) != -1)
											{
												handleShape(f, scriptText);
											}
										}
									}
								}															
							}	
						}
					}
				}
				else if (searchMaterial != std::string::npos)
				{
					searchMaterial = contenu.find("=");
					if (searchMaterial != std::string::npos)
					{
						ind = (int) searchMaterial;
						oldind = ind;
						ind += 2;
						
						//Create the path
						std::string  text = fileName.path();
						text += "/";
						text += contenu.substr(ind, contenu.length() - ind);
							
						//std::cout << " text " << text << std::endl;	
						
						const char * c = text.c_str();
						std::ifstream f(c, std::ios::in);
						
						if (f)
						{
							std::string scriptText;
							while(std::getline(f, scriptText))
							{
								//std::cout << scriptText << std::endl;
								searchComment = scriptText.find("#");
								if (!(searchComment != std::string::npos))
								{
									if (scriptText.length() > 0)
									{
										searchVoid = scriptText.find("void");						
										if(searchVoid != std::string::npos)
										{
											if (int(searchVoid) == 0)
											{
												handleMaterials(f, scriptText);
												nbMat++;	
											}
										}
									}
								}
							}	
						}
					}
				}
			}			
		}
		
		int Intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) 
		{
			// Return:
			//~ fprintf(stderr, "(%f %f)-(%f %f) vs (%f %f)-(%f %f)\n", x1, y1, x2, y2, x3, y3, x4, y4);
			double x12 = x2 - x1, y12 = y2 - y1, x34 = x4 - x3, y34 = y4 - y3, x13 = x3 - x1, y13 = y3 - y1;
			double area= (x12 * y34 - x34 * y12);
			if (0 == area) 
			{
				//~ fprintf(stderr, "  //\n" );
				return -1; // parallel
			}
			double t = x13 * y34 - x34 * y13;
			if (((t / area) <= 0)||((t / area) >= 1)) 
			{
				//~ fprintf(stderr, "_|t %f/%f=%f\n", t, area, t/area);
				return 1; // no intersection
			}
			double u = x13 * y12 - x12 * y13;
			if (((u / area) <= 0) || ((u / area) >= 1)) 
			{
				//~ fprintf(stderr, "_|u %f/%f=%f\n", u, area, u/area);
				return 1; // no intersection
			}
			//~ fprintf(stderr, "(%.2f %.2f)-(%.2f %.2f) vs (%.2f %.2f)-(%.2f %.2f)\n", x1, y1, x2, y2, x3, y3, x4, y4);
			//~ fprintf(stderr, "X\n" );
			
			//fprintf(stderr, "(%.2f %.2f)-(%.2f %.2f) vs (%.2f %.2f)-(%.2f %.2f)\n", x1, y1, x2, y2, x3, y3, x4, y4);
			
			return 0; // intersect
			//Intersection coordinates is (beware, t and u may be inverted)
			// (x1+x12*t/area, y1+y12*t/area)
			// and
			// (x3+x34*u/area, y3+y34*u/area)	
		}
		
		void Reorder(Vector3dVector& in , Vector3dVector& out,  VectorInt& in_out)
		{
			double area = Area(in);
			//std::cout << " area " << area << std::endl;
			bool invert = false;
			if (area < 0)
			{
				invert = true;
				for (int i = 0 ; i < (int)in.size() ; ++i)
				{
					in[i].Set(in[i].GetX(), - in[i].GetY());
				}
			}
						
			VectorInt pts;
			
			int sze = in.size();
			
			//fprintf(stderr, "List ID\n");
			
			for(int i = 0; i < sze; ++i) 
			{
				int ref = i;
				for(int j = 0; j < i; ++j) 
				{
					if (((in[i].GetX() == in[j].GetX()) && (in[i].GetY() == in[j].GetY())) && (ref == i)) 
					{
						ref = j;
					}	
				}
				pts.push_back(ref);
				//if (i!=ref) 
					//fprintf(stderr, "%d>%d \n", i, ref);
			}
			
			std::list<seg*> a, b;
			for(int i = 0; i < sze; ++i) 
			{
				 seg* s = new seg();
				 s->a = pts[i];
				 s->b = pts[(i + 1) % sze];
				 s->angle = atan2(in[s->b].GetY() - in[s->a].GetY(), in[s->b].GetX() - in[s->a].GetX());
				 if (i > 0)
				  a.push_back(s);
				 else
				  b.push_back(s);
			}
			
			bool intersect = true;
						
			while(intersect) 
			{
				//fprintf(stderr, "Start looking\n" );
				intersect = false;
				for(std::list<seg*>::iterator it = a.begin(); it != a.end(); ++it) 
				{
					for(std::list<seg*>::iterator it2 = it; it2 != a.end(); ++it2) 
					{
						if ((false == intersect) && (it != it2)) 
						{
							if ((((*it)->a) == ((*it2)->b)) && (((*it2)->a) == ((*it)->b))) 
							{
								//std::cout << "Opening detected , look for crossing" << std::endl;
								//std::cout << (*it)->a << " : " << (*it2)->b << " | " << (*it2)->a << " : " << (*it)->b << std::endl;
								// Opening detected, looks for crossings
								bool found = false;
								for(std::list<seg*>::iterator it3 = a.begin(); it3 != a.end(); ++it3) 
								{
								   /* if ((!found)&&(0==Intersect(
								   in[(*it)->a].GetX(), in[(*it)->a].GetY(),
								   in[(*it)->b].GetX(), in[(*it)->b].GetY(),
								   in[(*it3)->a].GetX(), in[(*it3)->a].GetY(),
								   in[(*it3)->b].GetX(), in[(*it3)->b].GetY()
								   ))) */
								   
								   if ((!found) && (((*it)->a) != ((*it3)->a)) && (((*it)->b)!=((*it3)->a)) && (((*it)->a) != ((*it3)->b)) && (((*it)->b) != ((*it3)->b)) && 
								   (0 == Intersect(in[(*it)->a].GetX(), in[(*it)->a].GetY(),
								   in[(*it)->b].GetX(), in[(*it)->b].GetY(),
								   in[(*it3)->a].GetX(), in[(*it3)->a].GetY(),
								   in[(*it3)->b].GetX(), in[(*it3)->b].GetY()
								   ))) 
								   {
									   // Intersect detected
									   found = true;
									   
									 //  fprintf(stderr, "Intersection detected  \n ");
									  // fprintf(stderr, " a %d b %d",(*it)->a, (*it)->b);
									   
									 /*  fprintf(stderr, "(%.2f %.2f)-(%.2f %.2f) vs (%.2f %.2f)-(%.2f %.2f)\n",
										in[(*it)->a].GetX(), in[(*it)->a].GetY(),
										in[(*it)->b].GetX(), in[(*it)->b].GetY(),
										in[(*it3)->a].GetX(), in[(*it3)->a].GetY(),
										in[(*it3)->b].GetX(), in[(*it3)->b].GetY());*/
									}
								}
								if ((1) &&(found)) 
								{
	
								  //Define inside loop and outside loop
								  std::vector<int> posspnts;
								  for(int i=0; i< (int) in.size(); ++i) 
										posspnts.push_back(0);
									
								  //std::cout << " Out " << (*it)->a << std::endl;
								  //std::cout << " in " << (*it)->b << std::endl;	
									
								  posspnts[(*it)->a]=-1;// Out
								  posspnts[(*it)->b]=1; // In
								  				  
								  int compt = 0;
								  
								  for (int i = 0 ; i < (int) posspnts.size() ; ++i)
								  {
									  if(posspnts[i] != 0)
									  {
										  compt++;
									  }
								  }
								  
								  static int first = 0;
								  static int sel = 0;
								  
								  std::list<seg*> c;
								  for(std::list<seg*>::iterator it3 = a.begin(); it3 != a.end(); ++it3)
									c.push_back((*it3));
								  while(c.size() > 0) 
								  {
									for(std::list<seg*>::iterator it3 = c.begin(); it3 != c.end(); ++it3) 
									{								
										if (posspnts[(*it3)->a] != 0) 
										{
																					
											posspnts[(*it3)->b] = posspnts[(*it3)->a];
											it3 = c.erase(it3);
										} 
										else if (posspnts[(*it3)->b] != 0) 
										{

											posspnts[(*it3)->a] = posspnts[(*it3)->b];
											it3 = c.erase(it3);
										}
									}
								  }
								  
								  //sel++;
								  
								  //fprintf(stderr, "All points inside or outside\n" );
								  // Try to move
								  //fprintf(stderr, "Try to move\n" );
								  double dist = 0.0;
								  int from = -1, to = -1;
								  for(int i = 0; i < (int) in.size(); ++i) 
								  {
									   for(int j = 0; j < (int) in.size(); ++j) 
									   {
										if ((i != j)&&(-1 == posspnts[i]) && (1 == posspnts[j])) 
										{
								//		 fprintf(stderr, "try %d %d\n", i, j);
										 // j in the inner loop, i out of the inner loop
										 double ndist=
										   (in[j].GetX()- in[i].GetX())
										  *(in[j].GetX()- in[i].GetX())
										  +(in[j].GetY()- in[i].GetY())
										  *(in[j].GetY()- in[i].GetY());
										 if ((-1==from)||((ndist > 0.0)&&(ndist < dist))) 
										 {
											bool OK=true;
											for(std::list<seg*>::iterator it3 = a.begin(); it3 != a.end(); ++it3) 
											{
											   if ((OK) && (i != ((*it3)->a)) && (j != ((*it3)->a)) && (i != ((*it3)->b)) && (j!=((*it3)->b)) && (0 == Intersect( 
												in[i].GetX(), in[i].GetY(),
												in[j].GetX(), in[j].GetY(),
												in[(*it3)->a].GetX(), in[(*it3)->a].GetY(),
												in[(*it3)->b].GetX(), in[(*it3)->b].GetY()
												))) 
												{
												// Intersect detected
													OK=false;
												}
											}
											  if (OK) 
											  {
												dist = ndist;
												from = i;
												to = j;
											  }
											}
										}
									}
								 }
								
								 // fprintf(stderr, "Suggest a new opening between %d and %d (dist=%f)\n", from, to, sqrt(dist));
								  //fprintf(stderr, " origine (*it)->a : %d, (*it)->b : %d, (*it2)->a : %d, (*it2)->b %d \n", (*it)->a, (*it)->b, (*it2)->a, (*it2)->b);
								  //fprintf(stderr, "(%f %f)-(%f %f)\n", in[from].GetX(), in[from].GetY(), in[to].GetX(), in[to].GetY());
								  (*it)->a=from;
								  (*it)->b=to;
								  (*it2)->a=to;
								  (*it2)->b=from;
								  // fprintf(stderr, " at the end (*it)->a : %d, (*it)->b : %d, (*it2)->a : %d, (*it2)->b %d \n", (*it)->a, (*it)->b, (*it2)->a, (*it2)->b);
								  intersect=true;
								  //fprintf(stderr, "Will loop again\n" );
								  
								}
							}
						}
					}
				}
			}
			
			// Reorder segments
			bool dout = false;
			while(a.size()>0) 
			{
				int ID = (*(b.rbegin()))->b;
				int IDF = (*(b.rbegin()))->a;
				double cang = (*(b.rbegin()))->angle;
				seg* res = NULL;
				double best = -4.0; // Anything smaller than -PI
				bool reverse = false;
				if (dout) 
					fprintf(stderr, "Search for '%d'\n", ID);
				for(std::list<seg*>::iterator it = a.begin(); it != a.end(); ++it) 
				{
					if (((*it)->a == ID) && ((*it)->b != IDF)) 
					{
						double nval = remainder(((*it)->angle) - cang, 2*M_PI);
						if (nval > best) 
						{ 
							best = nval; 
							res = *it; 
							reverse = false; 
						}
						if (dout) 
							fprintf(stderr, "  %d > %d %f\n", (*it)->a, (*it)->b, nval);
						} 
					else if (((*it)->b == ID)&&((*it)->a != IDF)) 
					{
						double nval = remainder(((*it)->angle) + M_PI-cang, 2*M_PI);
						if (nval>best) 
						{ 
							best=nval; 
							res=*it; 
							reverse=true; 
						}
						if (dout) 
							fprintf(stderr, "  %d < %d %f\n", (*it)->a, (*it)->b, nval);
					} 
					else 
					{
						if (dout) 
							fprintf(stderr, "  %d - %d\n", (*it)->a, (*it)->b);
					}
				}
			
			 dout = false;
			 if (res==NULL) 
			 {
				fprintf(stderr, "Error in function Reorder\n" );
				exit(-1);
			 }
			 b.push_back(res);
			 if (reverse) 
			 { 
				 int tmp = res->a; 
				 res->a = res->b; 
				 res->b = tmp; 
				 res->angle += M_PI; 
			 }
			 //fprintf(stderr, "Chosed %d>%d\n", res->a, res->b);
			 for(std::list<seg*>::iterator it=a.begin(); it!=a.end(); ++it) 
			 {
				if ((*it)==res) 
				{
					(*it)=*(a.begin());
				}
			 }
			 a.pop_front();
			}
			for(std::list<seg*>::iterator it = b.begin(); it != b.end(); ++it) 
			{
				//fprintf(stderr, "%d %d %f %f\n", (*it)->a, (*it)->b,in[(*it)->a].GetX(),in[(*it)->a].GetY());
				out.push_back(Vector(in[(*it)->a].GetX(),in[(*it)->a].GetY(), 0.0f));
				in_out.push_back((*it)->a);
				free(*it);
			}
			if (invert) 
			{
				for(int i=0; i< (int) in.size(); ++i)
					in[i].Set(in[i].GetX(), -in[i].GetY());
			}
											
		}

		
		/**
		 * 
		 * Projection 2D
		 * 
		 * */
		
		bool Project(Vector* in, int N, Vector* out, Vector& u , Vector& v, Vector& w)
		{
			if (N<3) 
			{
				fprintf(stderr, "WARNING: Polygon has less than 3 points (no triangle needed)" );
				 return false;
			}

			//std::cout << in[1].x << " - " << in[0].x << " => " << in[1].x - in[0].x << std::endl;
			//std::cout << in[1].y << " - " << in[0].y << " => " << in[1].y - in[0].y <<std::endl;
			//std::cout << in[1].z << " - " << in[0].z << " => " << in[1].z - in[0].z <<std::endl;

			u.x = in[1].x - in[0].x;
			u.y = in[1].y - in[0].y;
			u.z = in[1].z - in[0].z;
						
			double best = 0.0;
			Vector bw;
			for(int i = 0; i < N; ++i) {
				for(int j = i + 1; j < N; ++j) 
				{
				  for(int k = j + 1; k < N; ++k) 
				  {
					 
					 bw.x = (in[j].y-in[i].y)*(in[k].z-in[i].z) -(in[j].z-in[i].z)*(in[k].y-in[i].y);
					 bw.y = (in[j].z-in[i].z)*(in[k].x-in[i].x) -(in[j].x-in[i].x)*(in[k].z-in[i].z);
					 bw.z = (in[j].x-in[i].x)*(in[k].y-in[i].y)  - (in[j].y-in[i].y)*(in[k].x-in[i].x) ;
				   
				   double s = bw.x * bw.x + bw.y * bw.y + bw.z * bw.z;
				   if (s > best) 
				   {
					   
						w = bw;
						best=s;
				   }
				  }
				 }
				}



				v.x = w.y * u.z - u.y * w.z;
				v.y = w.z * u.x - u.z * w.x;
				v.z = w.x * u.y - u.x * w.y;
				

 				
				// Normalize projection vectors
				double nrm;
				nrm = sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
				if (0 == nrm) 
					return false;

				u.x /= nrm; 
				u.y /= nrm;
				u.z /= nrm;
				
				nrm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z );
				if (0 == nrm) 
					return false;

				v.x /= nrm;
				v.y /= nrm;
				v.z /= nrm;
				
				nrm = sqrt(w.x * w.x + w.y * w.y + w.z * w.z);
				if (0 == nrm) 
					return false;
				
				w.x /= nrm;
				w.y /= nrm;
				w.z /= nrm;
				
				//std::cout << " Fin Projection " << std::endl;
				
				//std::cout << " u " << u.x << " " << u.y << " " << u.z << std::endl;
				//std::cout << " w " << w.x << " " << w.y << " "  << w.z << std::endl;
				//std::cout << " v : " << v.x << " " << v.y << " "  << v.z << std::endl;
				
				for(int i = 0; i < N; ++i) 
				{
					out[i].x=(in[i].x - in[0].x) * u.x + (in[i].y - in[0].y) * u.y + (in[i].z - in[0].z) * u.z;
					out[i].y=((in[i].x - in[0].x) * v.x + (in[i].y - in[0].y) * v.y + (in[i].z - in[0].z) * v.z);
					out[i].z=(in[i].x - in[0].x) * w.x + (in[i].y - in[0].y) * w.y + (in[i].z - in[0].z) * w.z;
					//fprintf(stderr, "%f %f %f\n", out[i].x, out[i].y, out[i].z);
				}
				return true;
		}
		
		/**
		 * 
		 *  Polygone triangulation
		 * 
		 * */
		
		void PolygoneTriangulation(int nbPoint, Vector * tab, Vector * pXYZ, ITRIANGLE * pITriangle, int * nbtri)
		{			
			Vector *projectVector;
			//double u[3], v[3], w[3];
			Vector u,v,w;
			int ntri = 0;
			int count = 0;
								
			projectVector = new Vector[nbPoint];
			
			/*for (int i = 0 ; i < nbPoint ; ++i)
			{
				std::cout << pXYZ[i].x << " : " << pXYZ[i].y << " : " << pXYZ[i].z << std::endl;
			}*/
			

					
			if(Project(pXYZ, nbPoint, projectVector, u ,v, w))
			{
				//std::cout << "Project ok !! " << std::endl;
				
				//fprintf(stderr, " Projection matrix :\n" );
				//fprintf(stderr, "  u: (%f %f %f)\n", u[0], u[1], u[2]);
				//fprintf(stderr, "  v: (%f %f %f)\n", v[0], v[1], v[2]);
				//fprintf(stderr, "  w: (%f %f %f)\n", w[0], w[1], w[2]);
				//fprintf(stderr, " Projected coordinates :\n" );
				//for(int i = 0 ; i < nbPoint ; ++i) 
					//fprintf(stderr, "  [2D]  %d: (%f, %f)  (dist to plane : %f)\n",i, projectVector[i].x, projectVector[i].y, projectVector[i].z);
				
				Vector3dVector a;
				Vector3dVector b;
				VectorInt in_out;
				for(int i = 0; i < nbPoint; ++i)
				{
					//std::cout << " add " << projectVector[i].x << " : " << projectVector[i].y << " : " << projectVector[i].z << std::endl;
					a.push_back(Vector(projectVector[i].x, projectVector[i].y, projectVector[i].z));
				}
				
				double area = Area(a);
							
				VectorInt result;
				
				Reorder(a, b, in_out);
				
				Process(b, result);
																		
				int tcount = result.size() / 3;
					
				for (int i = 0 ; i < tcount ; i++)
				{

					Vector va = pXYZ[in_out[result[i * 3]]];
					Vector vb = pXYZ[in_out[result[i * 3 + 1]]];
					Vector vc = pXYZ[in_out[result[i * 3 + 2]]];
					
					Vector ab = va.Dist(vb);
					Vector bc = vb.Dist(vc);
					Vector abc = ab.Cross(bc);
					float abcw = abc.Dot(w);
					
					/*std::cout << " va " << va.x << " : " << va.y << " : " << va.z << std::endl;
					std::cout << " vb " << vb.x << " : " << vb.y << " : " << vb.z << std::endl;
					std::cout << " vc " << vc.x << " : " << vc.y << " : " << vc.z << std::endl;
					
					std::cout << " ab " << ab.x << " : " << ab.y << " : " << ab.z << std::endl;
					std::cout << " bc " << bc.x << " : " << bc.y << " : " << bc.z << std::endl;
					std::cout << " w "  << w.x << " : " << w.y << " : " << w.z << std::endl;
					
					std::cout << " abc " << abc.x << " : " << abc.y << " : " << abc.z << std::endl;
					
					std::cout << " dot " << abcw << std::endl;*/
																					
					float s = area * abcw;
					
					//std::cout << "areaPoly " << area << std::endl;
					
					//std::cout << " s => " << s << std::endl;
					
					//std::cout << " Area " << area << std::endl;
					
					//S*((AB^BC).w)>0	
					
					if (s >= 0.0)
					{
						//std::cout << " ABC " << std::endl;
						pITriangle[i].p1 = in_out[result[i * 3]];
						pITriangle[i].p2 = in_out[result[i * 3 + 1]];
						pITriangle[i].p3 = in_out[result[i * 3 + 2]];
					}
					else
					{
						//std::cout << " ACB " << std::endl;
						pITriangle[i].p1 = in_out[result[i * 3]];
						pITriangle[i].p2 = in_out[result[i * 3 + 2]];
						pITriangle[i].p3 = in_out[result[i * 3 + 1]];
					}
															
					ntri ++;
				}
								
				/*for (int i = 0; i < tcount; i++) 
				{
					if (comp > 350 && comp < 362)
					{
						fprintf(stderr, "  >%d> %d %d %d\n", i, result[i*3], result[i*3+1], result[i*3+2]);
						fprintf(stderr, "Polygon %d, triangle %d:\n", count + 1, i + 1);
						fprintf(stderr, "  (%f %f %f)\n", pXYZ[result[i * 3]].x, pXYZ[result[i * 3]].y, pXYZ[result[i * 3]].z);
						fprintf(stderr, "  (%f %f %f)\n", pXYZ[result[i * 3 + 1]].x, pXYZ[result[i * 3 + 1]].y, pXYZ[result[i * 3 + 1]].z);
						fprintf(stderr, "  (%f %f %f)\n", pXYZ[result[i * 3 + 2]].x, pXYZ[result[i * 3 + 2]].y, pXYZ[result[i * 3 + 2]].z);	
					}
				}*/
				
				count++;
			}
									
			*nbtri = ntri;
		}
		
		float Area(const Vector3dVector &contour)
		{
			int n = contour.size();
			float A= 0.0f;
			for(int p = n - 1, q = 0; q < n; p = q++)
			{	
				//std::cout << contour[p].GetX() << " * "  <<  contour[q].GetY() << " - " << contour[q].GetX() << " * " << contour[p].GetY() << std::endl; 
				A += contour[p].GetX() * contour[q].GetY() - contour[q].GetX() * contour[p].GetY();
			
			}
			
			//std::cout << "A  => " << A * 0.5f << std::endl;
					
			return A * 0.5f;
		}
		
		bool InsideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Px, float Py)
		{
			  float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
			  float cCROSSap, bCROSScp, aCROSSbp;
			  ax = Cx - Bx;  
			  ay = Cy - By;
			  bx = Ax - Cx;  
			  by = Ay - Cy;
			  cx = Bx - Ax;  
			  cy = By - Ay;
			  apx= Px - Ax;  
			  apy= Py - Ay;
			  bpx= Px - Bx;  
			  bpy= Py - By;
			  cpx= Px - Cx;  
			  cpy= Py - Cy;
			  aCROSSbp = ax * bpy - ay * bpx;
			  cCROSSap = cx * apy - cy * apx;
			  bCROSScp = bx * cpy - by * cpx;
			  if ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f))
			  {
				  return true;
			  }
			  else
			  {
				return false;
			  }
			  //return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
		}
		
		
		bool Snip(const Vector3dVector &contour,int u,int v,int w,int n,int *V)
		{
			int p;
			float Ax, Ay, Bx, By, Cx, Cy, Px, Py;
			Ax = contour[V[u]].GetX();
			Ay = contour[V[u]].GetY();
			Bx = contour[V[v]].GetX();
			By = contour[V[v]].GetY();
			Cx = contour[V[w]].GetX();
			Cy = contour[V[w]].GetY();

			if ( EPSILON > ( ( ( Bx-Ax) * (Cy-Ay) ) - ( (By-Ay) * (Cx-Ax) ) ) )
			{
				//std::cout << " EPSILON GREATER THAN " << std::endl;
				return false;
			}
			for (p = 0 ; p < n ; p++)
			{
				Px = contour[V[p]].GetX();
				Py = contour[V[p]].GetY();
				// std::cout << " p " << p << std::endl;
				if( (p == u) || (p == v) || (p == w) )
				{
					//std::cout << " continue " << std::endl;
					continue;
				}	 		
				
				if ((abs(Px - Ax) < 10E-6) && (abs(Py - Ay) < 10E-6))
				{
					//std::cout << " EGALITE A "<<  p << "( "<< Px <<  ": " << Py << ") = (" << Ax << " : " << Ay << ")" <<  std::endl;
					continue;
				}
				
				if ((abs(Px - Bx) < 10E-6) && (abs(Py-By) < 10E-6))
				{
					//std::cout << " EGALITE B "<<  p << "( "<< Px <<  ": " << Py << ") = (" << Bx << " : " << By << ")" <<  std::endl;
					continue;
				}
								
				if ((abs(Px-Cx) < 10E-6) && (abs(Py-Cy) < 10E-6))
				{
					//std::cout << " EGALITE C "<<  p << "( "<< Px <<  ": " << Py << ") = (" << Cx << " : " << Cy << ")" <<  std::endl;
					
					continue;
				}
														
				if (InsideTriangle(Ax, Ay, Bx, By, Cx, Cy, Px, Py)) 
				{
					//std::cout << " Ax " << Ax << " Ay " << Ay << " Bx " << Bx << " By " << By << " Cx " << Cx << "Cy " << Cy << std::endl;
					//std::cout << " INSIDE p " << V[p] << " => " << Px << " : " << Py << std::endl;
					return false;
				}

			}
			//std::cout << " OUTSIDE " << std::endl;
			return true;
		}
		
		bool Process(const Vector3dVector &contour, VectorInt &result)
		{
			/* allocate and initialize list of Vertices in polygon */
			//std::cout << "contour " << contour.size() << std::endl;
			int n = contour.size();
			int nbTriangle = 0;
			
			if ( n < 3 ) 
				return false;
			int *V = new int[n];
			/* we want a counter-clockwise polygon in V */
			if ( 0.0f < Area(contour) )
			for (int v = 0; v < n; v++) 
			{
				//std::cout << "clockwise " << std::endl;
				V[v] = v;
				
			}

			else
			{
				//std::cout << "anti clockwise " << std::endl;
				for(int v = 0; v < n; v++) 
				V[v] = (n-1)-v;
			}

			int nv = n;
			
			/*  remove nv-2 Vertices, creating 1 triangle every time */
			int count = 2 * nv;   /* error detection */
			for(int m = 0, v = nv - 1; nv > 2; )
			{
				/* if we loop, it is probably a non-simple polygon */
				if (0 >= (count--))
				{
					//** Triangulate: ERROR - probable bad polygon!
					return false;
				}
				/* three consecutive vertices in current polygon, <u,v,w> */
					
				int u = v  ; 
							
				if (nv <= u)
				{
					u = 0;     /* previous */
				} 
	
				v = u + 1;
				
 
				if (nv <= v)
				{
					v = 0;     /* new v    */
				} 
					
				int w = v + 1; 
								
				if (nv <= w) 
				{
					w = 0;     /* next     */
				}
										
				if ( Snip(contour,u,v,w,nv,V) )
				{
					nbTriangle++;
					int a,b,c,s,t;
					  /* true names of the vertices */
					  a = V[u]; 
					  b = V[v]; 
					  c = V[w];
					 // std::cout << nbTriangle << " =>  a " << a << " b " << b << " c " << c << std::endl;
					  /* output Triangle */
					  //~ result.push_back( contour[a] );
					  //~ result.push_back( contour[b] );
					  //~ result.push_back( contour[c] );
					  result.push_back( a );
					  result.push_back( b );
					  result.push_back( c );
					  //??
					  //m++;
					  //std::cout << " Point vire " << V[v] << std::endl;
					  /* remove v from remaining polygon */
					  for(s = v,t = v + 1 ; t < nv ; s++, t++) 
						V[s] = V[t];
						
						/*for (int i = 0 ; i < nv ; i ++)
						{
							std::cout << V[i] << " "  ;
						}*/
						
						//std::cout << " " << std::endl;
						 
					  nv--;
					  /* resest error detection counter */
					  count = 2 * nv;
				}
			}
			delete V;
			return true;
		}
				
		/**
		 * 
		 * Handler Region
		 * 
		 * */
		
		void handleShape(std::ifstream& f, std::string shapes)
		{								
			//Init
			std::vector<Vec3f> positions;
			std::vector<Vec3i> triangles;
			std::vector<Vec3f> normals;
			
			Vec3f ** tabNormal;
			
			int IndIndice = 0;
			int ** tabIndices;
			int tabnbShapes[3];
			std::string strArgument;
			std::string  tab[3];	

			float ** tabargs;
			Vector * pXYZ;
			Vector * pNormal;
			Vector * tabXYZ;
			ITRIANGLE * pITriangle;
			
			//Creation d'un mesh triangle
			Ref<Device::RTShape> mesh = g_device->rtNewShape("trianglemesh");
			
			positions.clear();
			triangles.clear();
			normals.clear();
			ind = 0;
			oldind= 0;		
			tabargs = new float*[3];
			tabIndices = new int*[3];
			tabNormal = new Vec3f*[3];
			
			//Create of the header of the shapes (name,material)
			for (int i = 0 ; i < 3 ; i++)
			{
				searchSpace = shapes.find(" ", ind);
				ind = (int) searchSpace;
				strArgument = shapes.substr(oldind,ind - oldind);
				oldind = ind;
				ind++;
				tab[i] = strArgument;
			}
							
			for (int i = 0 ; i < 3 ; i++)
			{
				int nbShapes;
				int nbCarac;
				int nbval = 0;
				ind = 0;
				oldind = 0;
								
				getline(f,shapes);
				
				searchSpace = shapes.find(" ",ind);
				
				ind = (int) searchSpace;
				
				strArgument = shapes.substr(oldind, ind - oldind);
				
				nbShapes = atoi(strArgument.c_str());
				
				oldind = ind;
				ind ++;
								
				tabnbShapes[i] = nbShapes;
				tabIndices[i] = new int[nbShapes / 3];
				tabNormal[i] = new Vec3f[nbShapes / 3];
				for (int j = 0 ; j < nbShapes / 3 ; j++)
				{
					tabNormal[i][j] = Vec3f(0.0f, 0.0f, 0.0f);
				}
				tabargs[i] = new float[nbShapes];
													
				if(nbShapes != 0)
				{
					//std::cout << "nbShapes " << nbShapes << std::endl;
					//Extract the arguments of each parameters line (string,int,float)
					for (int j = 0 ; j < (nbShapes / 3) ; j++)
					{
						getline(f, shapes);
																			
						ind = 1 ;
						oldind = 0;
						nbCarac = 0;						
												
						tabIndices[i][IndIndice] = IndIndice;
						IndIndice++;
						
						while (nbCarac < 3)
						{
							searchSpace = shapes.find(" ", ind);
							ind = (int) searchSpace;
							strArgument = shapes.substr(oldind, ind - oldind);
							if (strArgument.compare(" ") != 0)
							{
								float v = atof(strArgument.c_str());
								//std::cout << " v " << v << std::endl;
								tabargs[i][nbval++] = v;
								nbCarac ++;
							}
							
							oldind = ind;
							ind++;
						}
					}
					
					pXYZ = new Vector[nbShapes / 3];
					pITriangle = new ITRIANGLE [(nbShapes/3) * 3];
					tabXYZ = new Vector[nbShapes / 3];
					int ind = 0;
					
					for (int i = 0 ; i < (nbShapes/3) * 3; i +=3)
					{
						pXYZ[ind].x = tabXYZ[ind].x =  tabargs[2][i];
						pXYZ[ind].y = tabXYZ[ind].y = tabargs[2][i + 1];
						pXYZ[ind].z = tabXYZ[ind].z = tabargs[2][i + 2];
						ind++;
					}
					
					PolygoneTriangulation(nbShapes / 3, tabXYZ, pXYZ, pITriangle, &nbTri);
											
					for (int i = 0 ; i < nbShapes / 3 ; i++)
					{
						//std::cout << pXYZ[i].x << " : " << pXYZ[i].y << " : " << pXYZ[i].z << std::endl;
						positions.push_back(Vec3f(pXYZ[i].x, pXYZ[i].y, pXYZ[i].z));
					}
					
					for(int i = 0; i < nbTri; i++)
					{				
						//std::cout << " Triangle n" << i + 1 << std::endl;
						/*std::cout << " v[" << i <<"].p1 " << pITriangle[i].p1 << " v[" << i <<"].p2 " << pITriangle[i].p2 << " v[" << i <<"].p3 " << pITriangle[i].p3 << std::endl;
						std::cout<< pXYZ[pITriangle[i].p1].x<<" "<<  pXYZ[pITriangle[i].p1].y << " " << pXYZ[pITriangle[i].p1].z <<  "\n";
						std::cout<< pXYZ[pITriangle[i].p2].x<<" "<<  pXYZ[pITriangle[i].p2].y << " " << pXYZ[pITriangle[i].p2].z << "\n";
						std::cout<< pXYZ[pITriangle[i].p3].x<<" "<<  pXYZ[pITriangle[i].p3].y << " " << pXYZ[pITriangle[i].p2].z <<"\n";*/
									
						triangles.push_back(Vec3i(pITriangle[i].p1, pITriangle[i].p2 , pITriangle[i].p3));
					}
					
					/*for (int i = 0 ; i < nbTri ; i++)
					{
						Vec3i a = triangles[i];
						Vec3f v1;
						Vec3f v2;
						int index1 = a.x;
						int index2 = a.y;
						int index3 = a.z;
						std::cout << " point " << i << " => " << a.x << " : " << a.y << " : " << a.z << std::endl;
						std::cout << " a " << pXYZ[a.x].x << " : " << pXYZ[a.x].y << " : " << pXYZ[a.x].z << std::endl;
						std::cout << " b " << pXYZ[a.y].x << " : " << pXYZ[a.y].y << " : " << pXYZ[a.y].z << std::endl;
						std::cout << " c " << pXYZ[a.z].x << " : " << pXYZ[a.z].y << " : " << pXYZ[a.z].z << std::endl;	
					}*/
																	
				}
			}
																					
			mesh->rtSetArray("positions", "float3", &positions[0], positions.size(), sizeof(Vec3f));
			mesh->rtSetArray("indices"  , "int3", &triangles[0], triangles.size(), sizeof(Vec3i));

			mesh->rtCommit();
			
			//std::cout << "material size " << materials.size() << std::endl;
												
			*model += new ShapeNode(mesh, materials[0]);
									
			return;
							
		}
		
		void handleGlass(std::ifstream& f ,std::string material, std::string name)
		{
			Ref<Device::RTMaterial> cur = null;
			int tabnbMaterial[3];
			int nbcarac = 0;
			float ** tabargs;
			
			cur = g_device->rtNewMaterial("glass");
			materials.push_back(cur);
							
			cur->rtSetString("name" ,name.c_str());
			
			ind = 0;
			oldind= 0;
			size_t found;	
			tabargs = new float*[3];

			for (int i = 0 ; i < 3 ; i ++)
			{
				int nbmaterial;
				bool find;
				
				ind = 0;
				oldind = 0;
				getline(f, material);
							
				found = material.find(" ", ind);
				
				ind = (int) found;
				
				strArgument = material.substr(oldind, ind - oldind);
				
				nbmaterial = atoi(strArgument.c_str());
				find = false;
				
				oldind = ind;
				ind++;
				
				tabnbMaterial[i] = nbmaterial;
				tabargs[i] = new float[nbmaterial];
				
				bool checkAlias = false;
				
				if (nbmaterial > 0)
				{
					checkAlias = true;
				}
				
				//Exctract the informations of the material(string,int,float)				
				for (int j = 0 ; j < nbmaterial ; j++)
				{
					find = false;
					while(!find)
					{
						found = material.find(" " , ind);
						ind = (int) found;
						strArgument = material.substr(oldind, ind - oldind);
						if(strArgument.compare(" ") != 0)
						{
							float v = atof(strArgument.c_str());
							tabargs[i][j] = v;
							nbcarac++;
							find = true;
						}
						oldind = ind;
						ind++;
					}
				}
				
				if (checkAlias)
				{
					getline(f,material);
					bool endAlias = false;
					Alias a;
					while(!endAlias)
					{
						getline(f,material);
						//std::cout << material << std::endl;
						size_t foundalias = material.find("alias");
						a.id = idMaterial;
						if (searchVoid != std::string::npos &&  foundalias != std::string::npos)
						{
							//std::cout << " ALIAS " << std::endl;
							int ind = 0;
							int oldind= 0;
							std::string tabHeader[4];	
							for (int i = 0 ; i < 4 ; i++)
							{
								searchSpace = material.find(" ", ind);
								ind = (int) searchSpace;
								strArgument = material.substr(oldind, ind - oldind);
								oldind = ind;
								ind++;
								if( i > 0)
								{
									strArgument.erase(0, 1);
								}
								tabHeader[i] = strArgument;
							}
							
							//Show Alias arguments
														
							a.aliasName.push_back(tabHeader[2]);
						}
						else
						{
							endAlias = true;
						}
					}
					
					idMaterial++;	
					
					listMaterial.push_back(a);
								
				}
			}
			
			//std::cout << "tabargs[2][0] " << tabargs[2][0] << " : " << tabargs[2][1] << " : " << tabargs[2][2] << std::endl;
			
			cur->rtSetFloat3("reflectance", Vec3f(tabargs[2][0], tabargs[2][1], tabargs[2][2]));
			
			cur->rtCommit();
		}
		
		void handleGlow(std::ifstream& f ,std::string material, std::string name)
		{			
			Ref<Device::RTMaterial> cur = null;
			size_t found;	
			int tabnbMaterial[3];
			int nbcarac = 0;
			float ** tabargs;
			
			cur = g_device->rtNewMaterial("Mirror");
			
			cur->rtSetString("name" ,name.c_str());
			
			ind = 0;
			oldind= 0;

			tabargs = new float*[3];

			for (int i = 0 ; i < 3 ; i ++)
			{
				int nbmaterial;
				bool find;
				
				ind = 0;
				oldind = 0;
				getline(f, material);
							
				std::cout << material << std::endl;			
							
				found = material.find(" ", ind);
				
				ind = (int) found;
				
				strArgument = material.substr(oldind, ind - oldind);
				
				nbmaterial = atoi(strArgument.c_str());
				find = false;
				
				oldind = ind;
				ind++;
				
				tabnbMaterial[i] = nbmaterial;
				tabargs[i] = new float[nbmaterial];
								
				for (int j = 0 ; j < nbmaterial ; j++)
				{
					find = false;
					while(!find)
					{
						found = material.find(" " , ind);
						ind = (int) found;
						strArgument = material.substr(oldind, ind - oldind);
						if(strArgument.compare(" ") != 0)
						{
							float v = atof(strArgument.c_str());
							tabargs[i][j] = v;
							nbcarac++;
							find = true;
						}
						oldind = ind;
						ind++;
					}
				}
			}
			
			//std::cout << "tabargs[2][0] " << tabargs[2][0] << " : " << tabargs[2][1] << " : " << tabargs[2][2] << std::endl;
			
			cur->rtSetFloat3("reflectance", Vec3f(tabargs[2][0], tabargs[2][1], tabargs[2][2]));
			
			cur->rtCommit();
		}
		
		void handlePlastic(std::ifstream& f ,std::string material, std::string name)
		{
			int tabnbMaterial[3];
			int nbcarac = 0;
			float ** tabargs;
			Ref<Device::RTMaterial> cur = null;
			
			cur = g_device->rtNewMaterial("Plastic");
			cur->rtSetString("name" ,name.c_str());			
			ind = 0;
			oldind= 0;
			
			tabargs = new float*[3];

			for (int i = 0 ; i < 3 ; i ++)
			{
				int nbmaterial;
				bool find;
				
				ind = 0;
				oldind = 0;
				getline(f,material);
				
				//std::cout << "material " << material << std::endl;
							
				searchSpace = material.find(" ",ind);
				
				ind = (int) searchSpace;
				
				strArgument = material.substr(oldind, ind - oldind);
				
				nbmaterial = atoi(strArgument.c_str());
				find = false;
				
				oldind = ind;
				ind++;
				
				tabnbMaterial[i] = nbmaterial;
				tabargs[i] = new float[nbmaterial];
									
				bool checkAlias = false;
				
				if (nbmaterial > 0)
				{
					//If there is no material, we don't need to check it theirs have alias
					checkAlias = true;
				}
								
				for (int j = 0 ; j < nbmaterial ; j++)
				{
					find = false;
					while(!find)
					{
						searchSpace = material.find(" " , ind);
						ind = (int) searchSpace;
						strArgument = material.substr(oldind, ind - oldind);
						if(strArgument.compare(" ") != 0)
						{
							float v = 	atof(strArgument.c_str());
							//std::cout << " v " << v << std::endl;
							tabargs[i][j] = v;
							nbcarac++;
							find = true;
						}
						oldind = ind;
						ind++;
					}
				}
				
				if (checkAlias)
				{
					getline(f,material);
					bool endAlias = false;
					Alias a;
					while(!endAlias)
					{
						getline(f,material);
						size_t searchAlias = material.find("alias");
						a.id = idMaterial;

						//We suppose there is no a empty line between each alias
						if (searchVoid != std::string::npos &&  searchAlias != std::string::npos)
						{
							int ind = 0;
							int oldind= 0;
							std::string tabHeader[4];	
							for (int i = 0 ; i < 4 ; i++)
							{
								searchSpace = material.find(" ", ind);
								ind = (int) searchSpace;
								strArgument = material.substr(oldind, ind - oldind);
								oldind = ind;
								ind++;
								if( i > 0)
								{
									strArgument.erase(0, 1);
								}
								tabHeader[i] = strArgument;
							}
							
							//Show Alias arguments
							/*for (int i = 0 ; i < 4 ; i++)
							{
								std::cout << " arg " << i <<  " =>|" << tabHeader[i] << "|" << std::endl;
							}*/
							
							a.aliasName.push_back(tabHeader[2]);
							

						}
						else
						{
							//std::cout <<  " plus d'alias" << std::endl;
							endAlias = true;
						}
					}
					idMaterial++;	
					
					listMaterial.push_back(a);
								
				}				
			}
						
			cur->rtSetFloat3("pigmentColor" , Vec3f(tabargs[2][0], tabargs[2][1], tabargs[2][2]));
			//cur->rtSetFloat3("pigmentColor" , Vec3f(tabargs[2][0], tabargs[2][1], tabargs[2][2]));
			cur->rtSetFloat1("eta", tabargs[2][3]);
			cur->rtSetFloat1("roughness", tabargs[2][4]);
			cur->rtCommit();
			
			materials.push_back(cur);
		}
		
		void handleMaterials(std::ifstream& f ,std::string material)
		{
			ind = 0;
			oldind= 0;
			std::string tabHeader[3];	
			for (int i = 0 ; i < 3 ; i++)
			{
				searchSpace = material.find(" ", ind);
				ind = (int) searchSpace;
				strArgument = material.substr(oldind, ind - oldind);
				oldind = ind;
				ind++;
				if( i > 0)
				{
					strArgument.erase(0, 1);
				}
				tabHeader[i] = strArgument;
			}
				
			//Second arguments of tabheader is the material
			if (tabHeader[1].compare("plastic") == 0)
			{
				//std::cout << "plastic" << std::endl;
				handlePlastic(f, material, tabHeader[2]);
			}
			else if (tabHeader[1].compare("glass") == 0)
			{
				//std::cout << "glass" << std::endl;
				handleGlass(f, material, tabHeader[2]);	
			}
			else if (tabHeader[1].compare("glow") ==0)
			{
				//std::cout << "glow" << std::endl;
				handleGlow(f, material, tabHeader[3]);
			}
		}
	};
	
	Ref<Scene> loadRif(const FileName& fileName)
	{
		std::string s(fileName);
		const char * c = s.c_str();	
		std::ifstream f(c, std::ios::in);				
		return RadImporter(fileName, f).model.cast<Scene>(); 
	}
	
	

}


