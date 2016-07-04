#include <engine/resource/VertexArray.hpp>
#include <glad/glad.h>

void VertexArray::enable_attribute_index(glm::uint index)
{
	glEnableVertexAttribArray(index);
}

void VertexArray::disable_attribute_index(glm::uint index)
{
	glDisableVertexAttribArray(index);
}

void VertexArray::attribute_pointer(glm::uint index, int size, const VertexArray::Type& type, bool normalized, int stride, void const* ptr)
{
	glVertexAttribPointer(index, size, static_cast<GLenum>(type), (GLboolean) (normalized ? GL_TRUE : GL_FALSE), stride, ptr);
}

void VertexArray::draw(Mode const& mode, int first, int count)
{
	glDrawArrays(static_cast<GLenum>(mode), first, count);
}

void VertexArray::draw_elements(Mode const& mode, int count, ElementType const& type, void const* indices)
{
	glDrawElements(static_cast<GLenum>(mode), count, static_cast<GLenum>(type), indices);
}
