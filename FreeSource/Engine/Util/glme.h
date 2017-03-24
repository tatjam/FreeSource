#pragma once
/*	
		Tajamsoft GLM Extension Library

	This library is Open Domain, so feel
	free to do wathever you want with it.
	Giving credit is recommended but not 
	required!

	GLMEL extends GML with new classes for handling
	geometric shapes and their interactions in 2D/3D 
	space

	Tries to follow GLM style the best it can
	and so most functions are global instead
	of per class

*/


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glm
{


	// Represents and axis aligned bounding
	// box from one corner and diagonal 
	// distance
	class box3
	{
	public:

		glm::vec3 p1;
		glm::vec3 p2;

		box3() {}
		box3(glm::vec3 p1, glm::vec3 p2)
		{
			this->p1 = p1; this->p2 = p2;
		}
		
		void fromSize(glm::vec3 p1, glm::vec3 size)
		{
			this->p1 = p1;
			this->p2 = p1 + size;
		}


		~box3()	{ }
	};

	class sph3
	{
	public:

		glm::vec3 loc;
		float radius;

		sph3() {}
		sph3(glm::vec3 loc, float radius) { this->loc = loc; this->radius = radius; }
		~sph3() {}
	};


	// Checks is sph3D "a" contains point "p"
	bool contains(sph3 a, glm::vec3 p)
	{
		return (glm::distance(a.loc, p) < a.radius);
	}

	// Checks if box3D "a" contains point "p"
	bool contains(box3 a, glm::vec3 p)
	{
		return (p.x > a.p1.x && p.x < a.p2.x) &&
			(p.y > a.p1.y && p.y < a.p2.y) &&
			(p.z > a.p1.z && p.z < a.p2.z);

	}

	// Checks if sph3 "a" intersects sph3 "b"
	bool intersects(sph3 a, sph3 b)
	{
		return (glm::distance(a.loc, b.loc) < a.radius + b.radius);
	}

	// Checks if sph3 "a" intersetcs box3 "b"
	bool intersects(sph3 a, box3 b)
	{
		glm::vec3 testPoint = glm::vec3();
		testPoint.x = max(b.p1.x, min(a.loc.x, b.p2.x));
		testPoint.y = max(b.p1.y, min(a.loc.y, b.p2.y));
		testPoint.z = max(b.p1.z, min(a.loc.z, b.p2.z));

		return glm::distance(testPoint, a.loc) < a.radius;
	}

	// Checks if box3D "a" intersects box3D "b"
	bool intersects(box3 a, box3 b)
	{
		return (a.p1.x < a.p2.x && a.p2.x > b.p1.x) &&
			(a.p1.y < a.p2.y && a.p2.y > b.p1.y) &&
			(a.p1.z < a.p2.z && a.p2.z > b.p1.z);
	}


}