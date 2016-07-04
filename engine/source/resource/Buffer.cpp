#include <engine/resource/Buffer.hpp>
#include <glad/glad.h>

Buffer::Buffer()
{ }

Buffer::~Buffer()
{ destroy(); }

void Buffer::create()
{
	destroy();
	glGenBuffers(1, &_id);
}

void Buffer::destroy()
{
	if(_id != 0)
	{
		glDeleteBuffers(1, &_id);
		_id = 0;
	}
}

void Buffer::bind(Target const& target, glm::uint id)
{
	glBindBuffer(static_cast<GLenum>(target), id);
}

void Buffer::unbind(Target const& target)
{
	glBindBuffer(static_cast<GLenum>(target), 0);
}

void Buffer::data(Target const& target, glm::int64 size, void const* ptr, Usage const& usage)
{
	glBufferData(static_cast<GLenum>(target), size, ptr, static_cast<GLenum>(usage));
}

void Buffer::subdata(Target const& target, glm::int64 offset, glm::int64 size, void const* ptr)
{
	glBufferSubData(static_cast<GLenum>(target), offset, size, ptr);
}
