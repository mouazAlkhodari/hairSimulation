#pragma once
#include "VertextBufferLayout.h"
#include "VertextBuffer.h"
#include "renderer.h"

class VertexArray
{
	unsigned int m_RenedererID;
public:
	VertexArray(){
		GLCall(glGenVertexArrays(1, &m_RenedererID));
	}
	~VertexArray(){
		GLCall(glDeleteVertexArrays(1, &m_RenedererID));
	}
	void BindBuffer(const VertextBuffer &vb, const VertextBufferLayout &layout) {
		vb.Bind();
		Bind();
		const auto &elements = layout.getElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto &element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normal, element.size, (const void *)offset));
			offset += element.size;
		}

	}

	void Bind() {
		GLCall(glBindVertexArray(m_RenedererID));
	}

	void UnBind() {
		GLCall(glBindVertexArray(0));
	}
};

