#pragma once

#include <glm/glm.hpp>

class VertexArray
{
	VertexArray()
	{ }

public:
	enum class Type
	{
		Byte = 0x1400,
		UnsignedByte = 0x1401,
		Short = 0x1402,
		UnsignedShort = 0x1403,
		Fixed = 0x140C,
		Float = 0x1406
	};

	enum class ElementType
	{
		UnsignedByte = 0x1401,
		UnsignedShort = 0x1403
	};

	enum class Mode
	{
		Points = 0x0000,
		LineStrip = 0x0003,
		LineLoop = 0x0002,
		Lines = 0x0001,
		TriangleStrip = 0x0005,
		TriangleFan = 0x0006,
		Triangles = 0x0004
	};

	static void enable_attribute_index(glm::uint index);

	static void disable_attribute_index(glm::uint index);

	static void attribute_pointer(glm::uint index, int size, Type const& type, bool normalized, int stride, void const* ptr);

	static void draw(Mode const& mode, int first, int count);

	static void draw_elements(Mode const& mode, int count, ElementType const& type, void const* indices);
};
