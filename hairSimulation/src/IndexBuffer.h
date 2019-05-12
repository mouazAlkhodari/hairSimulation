#pragma once
class IndexBuffer
{
	unsigned int m_RendererID;
	unsigned int m_Count;
public:

	IndexBuffer(const float *data, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();
};

