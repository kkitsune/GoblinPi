#pragma once

#include <vector>
#include <array>

#include "Resource.hpp"

class ENGINE_API Buffer : public Resource
{
public:
	enum class Target
	{
		Array = 0x8892,
		ElementArray = 0x8893
	};

	enum class Usage
	{
		StaticDraw = 0x88E4,
		StreamDraw = 0x88E0,
		DynamicDraw = 0x88E8
	};

	Buffer();

	virtual ~Buffer();

	Buffer(Buffer const& other) = delete;

	Buffer(Buffer&& other) = default;

	Buffer& operator=(Buffer const& other) = delete;

	Buffer& operator=(Buffer&& other) = default;

	virtual void create() override;

	virtual void destroy() override;

	static void bind(Target const& target, glm::uint id);

	static void unbind(Target const& target);

	static void data(Target const& target, glm::int64 size, void const* ptr, Usage const& usage);

	template<typename T>
	static void data(Target const& target, std::vector<T>& vector, Usage const& usage)
	{
		data(target, (glm::int64) vector.size() * (glm::int64) sizeof(T), &vector[0], usage);
	}

	template<typename T, size_t Size>
	static void data(Target const& target, std::array<T, Size>& array, Usage const& usage)
	{
		data(target, (glm::int64) Size * (glm::int64) sizeof(T), &array[0], usage);
	}

	static void subdata(Target const& target, glm::int64 offset, glm::int64 size, void const* ptr);

	template<typename T>
	static void subdata(Target const& target, glm::int64 offset, std::vector<T>& vector)
	{
		subdata(target, offset, (glm::int64) vector.size() * (glm::int64) sizeof(T), &vector[0]);
	}

	template<typename T, size_t Size>
	static void subdata(Target const& target, glm::int64 offset, std::array<T, Size>& array)
	{
		subdata(target, offset, (glm::int64) Size * (glm::int64) sizeof(T), &array[0]);
	}
};
