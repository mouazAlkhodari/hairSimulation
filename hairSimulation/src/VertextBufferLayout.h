#pragma once
#include <vector>
#include "renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
struct VertextBufferElement{
	unsigned int count;
	unsigned int type;
	unsigned int normal;
	unsigned int size;
};


class VertextBufferLayout {
private:
	vector<VertextBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertextBufferLayout() :m_Stride(0) {};
	
	template<class T>
	void push(unsigned int count) {
		ASSERT(false);
	}
	template<>
	void push<float>(unsigned int count) {
		m_Elements.push_back({ count, GL_FLOAT, false ,count * sizeof(float) });
		m_Stride += count * sizeof(float);
	}
	template<>
	void push<char>(unsigned int count) {
		m_Elements.push_back({ count, GL_BYTE, false ,count * sizeof(char) });
		m_Stride += count * sizeof(char);
	} 
	template<>
	void push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ count, GL_UNSIGNED_INT, false ,count * sizeof(unsigned int ) });
		m_Stride += count * sizeof(unsigned int);
	}
	/*template<>
	void push<glm::vec3 >(unsigned int count) {
		m_Elements.push_back({ count, GL_V3F, false ,count * sizeof(glm::vec3) });
		m_Stride += count * sizeof(glm::vec3);
	}*/
	inline const vector<VertextBufferElement> getElements()const { return m_Elements; }
	inline unsigned int getStride()const { return m_Stride; }
};