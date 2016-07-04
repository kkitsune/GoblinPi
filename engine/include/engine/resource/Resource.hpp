#pragma once

#include <engine/config.h>
#include <glm/glm.hpp>

class ENGINE_API Resource
{
public:
	inline Resource() : _id(0)
	{ }

	virtual inline ~Resource()
	{ }

	Resource(Resource const& other) = delete;

	Resource(Resource&& other) = default;

	Resource& operator=(Resource const& other) = delete;

	Resource& operator=(Resource&& other) = default;

	inline glm::uint id() const
	{ return _id; }

	virtual void create() = 0;

	virtual void destroy() = 0;

protected:
	glm::uint _id;
};
