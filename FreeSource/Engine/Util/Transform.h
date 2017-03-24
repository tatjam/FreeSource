#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VECTOR_UP glm::vec3(0, 1, 0)
#define VECTOR_RIGHT glm::vec3(1, 0, 0)
#define VECTOR_FORWARD glm::vec3(0, 0, 1)

// Transformations for Models (and other similar 3D objects)
// Functions do not automatically update the modelMatrix!
// Call buildMatrix to generate it
class Transform
{
public:

	Transform* parent;

	// Position relative to parent (if any)
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	// Rotation relative to self (GLOBAL)
	glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	// Scale relative to self (GLOBAL)
	glm::vec3 scaling = glm::vec3(1.0f, 1.0f, 1.0f);

	// The last generated modelMatrix
	glm::mat4 modelMatrix;


	// Rotate the transform around point using up as axis
	// and radians for distance rotated
	// If lookToPath is true the transform will adjust to look forward 
	// along the rotation
	void rotateAround(glm::vec3 point, glm::vec3 up, float radians, bool lookToPath = false)
	{
		//position = 	
	}

	// Change our parent
	// If update is set to false our position
	// in world will change due to new parent's position
	void setParent(Transform* t, bool update = true)
	{
		parent = t;
		if (update)
		{
			position -= parent->position;
		}
	}

	void lookAt(glm::vec3 point, glm::vec3 up)
	{
		//glm::vec3 dirVector = 
	}


	glm::mat4 getRealMatrix()
	{
		if (parent == NULL)
		{
			return getMatrix();
		}
		else
		{
			// This should go over all parents of parents, etc...
			return parent->getRealMatrix() * getMatrix();
		}
	}

	glm::mat4 getMatrix()
	{
		glm::mat4 identity = glm::mat4(1.0f);
		glm::mat4 n;

		n = glm::translate(n, position);
		n *= glm::mat4_cast(rotation);
		n = glm::scale(n, scaling);

		return n;
	}

	void buildMatrix()
	{
		modelMatrix = getRealMatrix();
	}

	// Set rotation quaternion using euler angles
	void setEulerAngles(glm::vec3 angles)
	{
		angles.x = glm::radians(angles.x);
		angles.y = glm::radians(angles.y);
		angles.z = glm::radians(angles.z);

		glm::quat aroundX, aroundY, aroundZ;

		// Create quaternions for each axis

		aroundX = glm::angleAxis(angles.x, VECTOR_RIGHT);
		aroundY = glm::angleAxis(angles.y, VECTOR_UP);
		aroundZ = glm::angleAxis(angles.z, VECTOR_FORWARD);

		// Join quaternions
		rotation = aroundX * aroundY * aroundZ;
	}

	void rotateEuler(glm::vec3 angles)
	{
		angles.x = glm::radians(angles.x);
		angles.y = glm::radians(angles.y);
		angles.z = glm::radians(angles.z);

		glm::quat aroundX, aroundY, aroundZ;

		// Create quaternions for each axis

		aroundX = glm::angleAxis(angles.x, VECTOR_RIGHT);
		aroundY = glm::angleAxis(angles.y, VECTOR_UP);
		aroundZ = glm::angleAxis(angles.z, VECTOR_FORWARD);

		// Add the quaternions
		rotation *= aroundX * aroundY * aroundZ;
	}

	// Alternative versions with direct floats:

	void setEulerAngles(float x, float y, float z)
	{
		setEulerAngles(glm::vec3(x, y, z));
	}
	void rotateEuler(float x, float y, float z)
	{
		rotateEuler(glm::vec3(x, y, z));
	}
};