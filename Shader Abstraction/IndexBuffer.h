#pragma once

class IndexBuffer {
private:
	unsigned m_RendererID;
	unsigned m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void unBind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
