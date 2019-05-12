
#include <vector>

#include "renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;


#pragma once
class VertextBuffer
{
	unsigned int m_RendererID;
public:	
	template<typename T>
	VertextBuffer(T *data, unsigned int size) {
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}
	template<typename T>
	VertextBuffer(vector<T> data, unsigned int size) {
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW));
	}
	void Bind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}
	template<typename T>
	void Bind(T *data, unsigned int size) const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}
	template<typename T>
	void Bind(vector<T> data, unsigned int size) const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW));
	}
	void UnBind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
};