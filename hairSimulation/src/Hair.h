#pragma once
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexArray.h"
#include "VertextBuffer.h"
#include "VertextBufferLayout.h"
using namespace std;
using namespace glm;
class Hair;
class Hairline;
class Spring;

// *********** config **************** //
const int SPRING_NUM_JUST = 10;
const float SPRING_K_JUST = 60.0f;
const float SPRING_LENGTH_JUST = 0.04f;
const float MINLEN = 0.2f;
const float MAXLEN = 4.5f;
const int GUIDEHAIRNUM_JUST = 200;

const float PARTICLE_MASS_JUST = 0.2f;
const float GRAVITY_JUST = 6.8f;
const float DAMPING_K_JUST = 1.4f;
//need to be in main 
const float FPS = 30;

class Hair {
public:
	Hair();
	Hair(vector<vec3> _all_root_ps);
	~Hair();

	bool Init(vector<vec3> _all_root_ps);
	void Draw();
	void Update();
	void addHairLine(Hairline h); 
	vector<Hairline> m_hairlines;

private:
	void _bindBufferData();
	void _initBufferData();
};

class Hairline
{
public:
	Hairline();
	Hairline(vec3 root_p);
	~Hairline();

	void Update();
	void UpdateRenderData();
	void Draw();

	VertexArray *VAO;
	VertextBuffer *VBO;
	VertextBufferLayout *layout;
		
	vector<Spring*> *m_Springs;
	vector<vec3> m_positions;
	vector<vec3> render_positions;

	vec3 m_root_position;
	
	void _bindBufferData();
	void _initBufferData();
private:
	// TODO : need to understand ^_^ 
//	vec3 _linearInterpolate(vec3 s, vec3 e, float t);
//	void _archimedeanspiral(vector<vec3> ps);
};

class Spring
{
public:
	Spring();
	Spring(vec3 *_last, vec3 *_x, vec3 *_next, vec3 _root);
	~Spring();

	void Update();
	vec3 checkSpringLength(vec3 p);

	vec3 *m_last;   // last point
	vec3 *m_x	;   // this point
	vec3 *m_next;   // next point
	vec3 *m_vNext;  // next velocity
	vec3 *m_vLast;  // last velocity

	vec3 m_velocity;
	float gravity = GRAVITY_JUST;
	vec3 m_root_postion;
	float mass;
	
// dt (we can compute it)
	float m_delt;

	float head_k = 2.3f * SPRING_K_JUST;
	float k = SPRING_K_JUST;

};