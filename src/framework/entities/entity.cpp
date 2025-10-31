#include "entity.h"
#include "framework/camera.h"
#include "framework/pulse.h"
#include <algorithm>

void Entity::render(Camera* camera)
{
	for (int i = 0; i < children.size(); ++i) {
		children[i]->render(camera);
	}
}

void Entity::update(float delta_time)
{
	for (int i = 0; i < children.size(); ++i) {
		children[i]->update(delta_time);
	}
}

void Entity::addChild(Entity* child)
{
	if (child->parent) {
		std::cerr << "Child has already a parent, remove it first!" << std::endl;
		return;
	}

	// Checks if it's already a child
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end()) {
		std::cerr << "Entity is already one of the children!" << std::endl;
		return;
	}

	child->parent = this;
	children.push_back(child);
}

void Entity::removeChild(Entity* child)
{
	// Checks if it's a child
	auto it = std::find(children.begin(), children.end(), child);
	if (it == children.end()) {
		std::cerr << "Entity is not a child!!" << std::endl;
		return;
	}

	children.erase(it);
	child->parent = nullptr;
}

Matrix44 Entity::getGlobalMatrix()
{
	if (parent)
		return model * parent->getGlobalMatrix();
	return model;
}

float Entity::distance(Entity* e)
{
	return model.getTranslation().distance(e->model.getTranslation());
}
void EntityMesh::update(float elapsed_time) {
	// You can add custom update code here for EntityMesh
	Entity::update(elapsed_time); // Call base class update to update children
}
void EntityMesh::render(Camera* camera) {

	if(!mesh){
		Entity::render(camera);
		return;
	}

	const Matrix44& globalMat = getGlobalMatrix();
	float distance = 10.0f;
	BoundingBox box = transformBoundingBox(globalMat, mesh->box);

	//Distance CUlling
	if (camera->eye.distance(box.center) > distance + box.halfsize.length()) {
		Entity::render(camera);
		return;
	}

	//Frustum Culling
	if (camera->testSphereInFrustum(box.center, box.halfsize.length()) != CLIP_INSIDE) {
		Entity::render(camera);
		return;
	}

	if(!material->shader){
		material->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/pulse.fs");
	}



	Shader* shader = material->shader;
	// Set OpenGL flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Enable shader and pass uniforms 
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform3("u_color", vec3(1, 1, 1));
	shader->setUniform("u_pulse_color", pulse.color);
	shader->setUniform("u_pulse_width",pulse.width);
	shader->setUniform3("u_pulse_center", pulse.center);
	shader->setUniform("u_pulse_radius", pulse.radius);
	shader->setUniform("u_pulse_active", pulse.active);
	if(material->diffuse)shader->setTexture("u_texture", material->diffuse, 0);


	// Render the mesh using the active shader
	mesh->render(GL_TRIANGLES);

	// Render the mesh using the active shader
	mesh->render(GL_TRIANGLES);

	// Disable shader after finishing rendering
	shader->disable();
	Entity::render(camera);
};

