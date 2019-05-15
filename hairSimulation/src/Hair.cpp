#include "Hair.h"

#pragma once
glm::vec3& operator*= (glm::vec3 v, const float &a) { v.x *= a;   v.y *= a; v.z *= a;  return v; }
glm::vec3 operator* (const glm::vec3 &v, const float &a) { return v *= a; }
glm::vec3 operator* (const float &a, const glm::vec3 &v) { return v *= a; }

glm::vec3& operator+= (glm::vec3 v, const glm::vec3 &v1) { v.x += v1.x;   v.y += v1.y; v.z += v1.z;  return v; }
glm::vec3 operator+ (const glm::vec3 &v, const glm::vec3 &v1) { return v += v1; }

std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << "point(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}
Hair::Hair()
{

}

Hair::Hair(vector<vec3> _all_root_ps)
{
	bool res = this->Init(_all_root_ps);
	if (!res)
	{
		cout << "ERROR:: fail to initialize hair  " << "\n";
	}
}

Hair::~Hair()
{

}

bool Hair::Init(vector<vec3> _all_root_positions){
	int range_max = _all_root_positions.size();
	for (int i = 0; i < min(range_max, GUIDEHAIRNUM_JUST); i++){
		int id = rand();
		id %= range_max;
		vec3 postion = _all_root_positions[id];
		Hairline hair(postion);
		this->m_hairlines.push_back(hair);
	}
	return true;
}

void Hair::Draw(){
	for (int i = 0; i < this->m_hairlines.size(); i++){
		this->m_hairlines[i].Draw();
	}
}

void Hair::Update(){
	for (int i = 0; i < this->m_hairlines.size(); i++){
		this->m_hairlines[i].Update();
	}
}
void Hair::addHairLine(Hairline h) {
	m_hairlines.push_back(h);
}
Hairline::Hairline()
{

}

Hairline::Hairline(vec3 root_position){
	this->m_root_position = root_position;

	this->m_Springs = new vector<Spring*>;
	vec3 last, cur, next;
//	vec3 normal = normalize(root_position);
	vec3 normal = vec3(1, 0, 0);
	vector<vec3*> velocitys;
	for (int i = 0; i < SPRING_NUM_JUST; i++){
		cur = root_position + i * SPRING_LENGTH_JUST * normal;
		cout << cur << endl;
		velocitys.push_back(vec3(0,0,0));
		this->m_positions.push_back(cur);
	}
	velocitys.push_back(vec3(0, 0, 0));
	this->m_positions.push_back(cur + SPRING_LENGTH_JUST * normal);

	Spring * spring;
	for (int i = 0; i < SPRING_NUM_JUST; i++)
	{
		if (i == 0){
			spring = new Spring(NULL, &this->m_positions[i], &this->m_positions[i + 1], root_position,NULL,velocitys[i],velocitys[i+1]);
		}
		else{
			spring = new Spring(&this->m_positions[i - 1], &this->m_positions[i], &this->m_positions[i + 1], root_position,velocitys[i - 1],velocitys[i],velocitys[i + 1]);
		}
		this->m_Springs->push_back(spring);
	}
	this->m_Springs->push_back(new Spring(&this->m_positions[SPRING_NUM_JUST - 1], &this->m_positions[SPRING_NUM_JUST], NULL, root_position));
	this->m_Springs->push_back(new Spring(velocitys[SPRING_NUM_JUST - 1], velocitys[SPRING_NUM_JUST], NULL, root_position));
	this->_initBufferData();
	this->UpdateRenderData();
}

Hairline::~Hairline()
{

}
void Hairline::Draw(){
	this->_bindBufferData();
	//for (int i = 0; i < render_positions.size(); i+=2) {
	//	cout << render_positions[i] << " " << render_positions[i + 1] << endl;
	//}
	GLCall(glDrawArrays(GL_LINES, 0, render_positions.size()));
}

void Hairline::Update(){
	for (int i = 0; i < this->m_Springs->size(); i++){
		Spring *spring = this->m_Springs->at(i);
		spring->Update();
		this->m_positions[i] = *spring->m_x;
	}
	this->UpdateRenderData();
	this->_bindBufferData();
}

void Hairline::UpdateRenderData(){
	this->render_positions.clear();
	for (int i = 0; i < m_positions.size() - 2; i++){
		this->render_positions.push_back(m_positions[i]);
		this->render_positions.push_back(m_positions[i + 1]);
	}
	_bindBufferData();
}
void Hairline::_initBufferData() {
	this->VAO = new VertexArray();
	this->VBO = new VertextBuffer(this->render_positions.data(), 3 * sizeof(float) * this->render_positions.size());
	this->layout = new VertextBufferLayout();
	this->layout->push<float>(3);
	this->VAO->BindBuffer(* this->VBO, *this->layout);
}
void Hairline::_bindBufferData(){
	this->VBO->Bind(render_positions.data(), sizeof(float) * render_positions.size() * 3);
	this->VAO->BindBuffer(*this->VBO, *this->layout);
}



// --------- Spring -------------//

Spring::Spring()
{

}

Spring::Spring(vec3 *_last, vec3 *_x, vec3 *_next, vec3 _root){
    this->m_last = _last;
    this->m_x = _x;
    this->m_next = _next;

    this->m_velocity = vec3(0, 0, 0);
// dt (we can compute it)
	this->m_delt = 1.0f/FPS;

    this->mass = PARTICLE_MASS_JUST;
    this->k = SPRING_K_JUST;
    this->m_root_postion = _root;
}

Spring::~Spring(){

}


void Spring::Update(){

    if(this->m_last == NULL)
    {
        vec3 p = this->m_root_postion;
        *this->m_x = p;
        return;
    }

	vec3 force_SpringLast(0,0,0), force_SpringNext(0,0,0), force_damping(0,0,0), force_gravity(0,0,0);
	vec3 delta, direction;
    float scale  = 0;
    float length = 0;
    float deltal = 0;
	vec3 force(0,0,0),accelration(0,0,0);
	vec3 v, p;
    p = * this->m_x;
    v =   this->m_velocity;

    // force of last Spring
    delta       = * this->m_last - * this->m_x;
    length      = glm::length(delta);
    direction   = normalize(delta);
    deltal		= length - SPRING_LENGTH_JUST;
    scale		= this->k * deltal;


	force_SpringLast = direction * scale;

//  force of next Spring
    if(this->m_next != NULL)
    {
		delta = *this->m_next - *this->m_x;
		length = glm::length(delta);
		direction = normalize(delta);
		deltal = length - SPRING_LENGTH_JUST ;
		scale = this->k * deltal;
        force_SpringNext = direction * scale;
    }
    // force of damping
    force_damping = -1 * ((this->m_velocity) * DAMPING_K_JUST);

	// force of gravity
    force_gravity = this->mass * this->gravity * vec3(0,-1,0);

    force = force_SpringLast + force_SpringNext + force_damping+ force_gravity;

	accelration = force / this->mass;

    // compute velocity
    v = this->m_velocity;
    v = accelration * this->m_delt + v;

    // update position
    p = v * this->m_delt + p;
    // check length constrain
    p = this->checkSpringLength(p);




    this->m_velocity = v;
    * this->m_x = p;
}
vec3 Spring::checkSpringLength(vec3 p)
{
	vec3 delta = (p - *(this->m_last));
	float deltal = length(delta);

	if (deltal < MINLEN * SPRING_LENGTH_JUST) // too short
	{
		p = normalize(delta) * MINLEN* SPRING_LENGTH_JUST + *this->m_last;
	}
	else if (deltal > MAXLEN * SPRING_LENGTH_JUST)// too long
	{
		p = normalize(delta) * MAXLEN * SPRING_LENGTH_JUST + *this->m_last;
	}
	return p;
}
