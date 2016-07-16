#pragma once

/*
 * FileSystem.hpp -- Slightly modified version of Wenzel Jacob's path.h (https://github.com/wjakob/filesystem)
 *
 * Copyright (c) 2015 Wenzel Jakob <wenzel@inf.ethz.ch>
 * Copyright (c) 2016 Daniel Grondin <tm.kkitsune@gmail.com>
 */

#include <sys/stat.h>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cctype>
#include <cerrno>

#if defined(_WIN32)
# include <windows.h>
#else
# include <unistd.h>
#endif

#if defined(__linux)
# include <linux/limits.h>
#endif

namespace filesystem
{
	class Path
	{
	public:
		enum class Type
		{
			Windows = 0,
			Posix = 1,
#if defined(_WIN32)
			Native = static_cast<int>(Windows)
#else
			Native = static_cast<int>(Posix)
#endif
		};

		Path() : _type(Type::Native), _absolute(false)
		{ }

		Path(char const* str)
		{ set(str); }

		Path(std::string const& str)
		{ set(str); }

#if defined(_WIN32)
		Path(std::wstring const& wstring)
		{ set(wstring); }

		Path(wchar_t const* wstring)
		{ set(wstring); }
#endif

		Path(Path const&) = default;

		Path(Path&&) = default;

		Path& operator=(Path const&) = default;

		Path& operator=(Path&&) = default;

		size_t length() const { return _path.size(); }

		bool empty() const { return _path.empty(); }

		bool is_absolute() const { return _absolute; }

		Path make_absolute() const
		{
#if !defined(_WIN32)
			char temp[PATH_MAX];
			if (realpath(str().c_str(), temp) == NULL)
				throw std::runtime_error("Internal error in realpath(): " + std::string(strerror(errno)));
			return Path(temp);
#else
			std::wstring value = wstr(), out(MAX_PATH, '\0');
			DWORD length = GetFullPathNameW(value.c_str(), MAX_PATH, &out[0], NULL);
			if (length == 0)
				throw std::runtime_error("Internal error in realpath(): " + std::to_string(GetLastError()));
			return Path(out.substr(0, length));
#endif
		}

		bool exists() const
		{
#if defined(_WIN32)
			return GetFileAttributesW(wstr().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
			struct stat sb;
        return stat(str().c_str(), &sb) == 0;
#endif
		}

		size_t file_size() const
		{
#if defined(_WIN32)
			struct _stati64 sb;
			if (_wstati64(wstr().c_str(), &sb) != 0)
				throw std::runtime_error("path::file_size(): cannot stat file \"" + str() + "\"!");
#else
			struct stat sb;
			if (stat(str().c_str(), &sb) != 0)
				throw std::runtime_error("path::file_size(): cannot stat file \"" + str() + "\"!");
#endif
			return (size_t) sb.st_size;
		}

		bool is_directory() const
		{
#if defined(_WIN32)
			DWORD result = GetFileAttributesW(wstr().c_str());
			if (result == INVALID_FILE_ATTRIBUTES)
				return false;
			return (result & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
			struct stat sb;
			if (stat(str().c_str(), &sb))
				return false;
			return S_ISDIR(sb.st_mode);
#endif
		}

		bool is_file() const
		{
#if defined(_WIN32)
			DWORD attr = GetFileAttributesW(wstr().c_str());
			return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
			struct stat sb;
			if (stat(str().c_str(), &sb))
				return false;
			return S_ISREG(sb.st_mode);
#endif
		}

		std::string extension() const
		{
			const std::string &name = filename();
			size_t pos = name.find_last_of(".");
			if (pos == std::string::npos)
				return "";
			return name.substr(pos + 1);
		}

		std::string filename() const
		{
			if (empty())
				return "";
			const std::string &last = _path[_path.size() - 1];
			return last;
		}

		Path parent_path() const
		{
			Path result;
			result._absolute = _absolute;

			if (_path.empty())
			{
				if (!_absolute)
					result._path.push_back("..");
			}
			else
			{
				size_t until = _path.size() - 1;
				for (size_t i = 0; i < until; ++i)
					result._path.push_back(_path[i]);
			}
			return result;
		}

		Path operator/(Path const& other) const
		{
			if (other._absolute)
				throw std::runtime_error("path::operator/(): expected a relative path!");
			if (_type != other._type)
				throw std::runtime_error("path::operator/(): expected a path of the same type!");

			Path result(*this);

			for(auto p : other._path)
				result._path.push_back(p);

			return result;
		}

		std::string str(Type type = Type::Native) const
		{
			std::ostringstream oss;

			if (_type == Type::Posix && _absolute)
				oss << "/";

			for (size_t i = 0; i < _path.size(); ++i)
			{
				oss << _path[i];
				if (i + 1 < _path.size())
					oss << (type == Type::Posix ? '/' : '\\');
			}

			return oss.str();
		}

		void set(std::string const& str, Type type = Type::Native)
		{
			_type = type;
			if (type == Type::Windows)
			{
				_path = tokenize(str, "/\\");
				_absolute = str.size() >= 2 && std::isalpha(str[0]) && str[1] == ':';
			}
			else
			{
				_path = tokenize(str, "/");
				_absolute = !str.empty() && str[0] == '/';
			}
		}

		friend std::ostream& operator<<(std::ostream& os, Path const& path)
		{
			os << path.str();
			return os;
		}

		bool remove_file()
		{
#if !defined(_WIN32)
			return std::remove(str().c_str()) == 0;
#else
			return DeleteFileW(wstr().c_str()) != 0;
#endif
		}

		bool resize_file(size_t target_length)
		{
#if !defined(_WIN32)
			return ::truncate(str().c_str(), (off_t) target_length) == 0;
#else
			HANDLE handle = CreateFileW(wstr().c_str(), GENERIC_WRITE, 0, nullptr, 0, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (handle == INVALID_HANDLE_VALUE)
				return false;
			LARGE_INTEGER size;
			size.QuadPart = (LONGLONG) target_length;
			if (SetFilePointerEx(handle, size, NULL, FILE_BEGIN) == 0)
			{
				CloseHandle(handle);
				return false;
			}
			if (SetEndOfFile(handle) == 0)
			{
				CloseHandle(handle);
				return false;
			}
			CloseHandle(handle);
			return true;
#endif
		}

		static Path getcwd()
		{
#if !defined(_WIN32)
			char temp[PATH_MAX];
			if (::getcwd(temp, PATH_MAX) == NULL)
				throw std::runtime_error("Internal error in getcwd(): " + std::string(strerror(errno)));
			return Path(temp);
#else
			std::wstring temp(MAX_PATH, '\0');
			if (!_wgetcwd(&temp[0], MAX_PATH))
				throw std::runtime_error("Internal error in getcwd(): " + std::to_string(GetLastError()));
			return Path(temp.c_str());
#endif
		}

#if defined(_WIN32)
		std::wstring wstr(Type type = Type::Native) const
		{
			std::string temp = str(type);
			int size = MultiByteToWideChar(CP_UTF8, 0, &temp[0], (int)temp.size(), NULL, 0);
			std::wstring result(size, 0);
			MultiByteToWideChar(CP_UTF8, 0, &temp[0], (int)temp.size(), &result[0], size);
			return result;
		}

		void set(std::wstring const& wstring, Type type = Type::Native)
		{
			std::string string;
			if (!wstring.empty())
			{
				int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), NULL, 0, NULL, NULL);
				string.resize(size, 0);
				WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), &string[0], size, NULL, NULL);
			}
			set(string, type);
		}
#endif

		bool operator==(Path const& p) const { return p._path == _path; }
		bool operator!=(Path const& p) const { return p._path != _path; }

	private:
		static std::vector<std::string> tokenize(std::string const& string, std::string const& delim)
		{
			std::string::size_type lastPos = 0, pos = string.find_first_of(delim, lastPos);
			std::vector<std::string> tokens;

			while (lastPos != std::string::npos)
			{
				if (pos != lastPos)
					tokens.push_back(string.substr(lastPos, pos - lastPos));
				lastPos = pos;
				if (lastPos == std::string::npos || lastPos + 1 == string.length())
					break;
				pos = string.find_first_of(delim, ++lastPos);
			}

			return tokens;
		}


		Type _type;
		std::vector<std::string> _path;
		bool _absolute;
	};

	inline bool create_directory(Path const& p)
	{
#if defined(_WIN32)
		return CreateDirectoryW(p.wstr().c_str(), NULL) != 0;
#else
		return mkdir(p.str().c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0;
#endif
	}
}
