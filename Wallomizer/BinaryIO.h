#pragma once

// BinaryWriter/BinaryReader
//
// Utility classes for writing and reading binary files.
//
// Supported:
//  - trivially copyable types
//  - std::wstring
//  - std::filesystem::path
//
// Strings and paths are stored as:
// [uint16_t length][UTF-16 characters without terminating '\0']

#include <cstdint>
#include <limits>
#include <type_traits>
#include <string>
#include <filesystem>
#include <fstream>

class BinaryWriter
{
public:
	explicit BinaryWriter(const std::filesystem::path& filePath) :
		m_file(filePath, std::ios::binary)
	{
	}

	BinaryWriter(const BinaryWriter&) = delete;
	BinaryWriter& operator=(const BinaryWriter&) = delete;

	[[nodiscard]] bool isOpen() const noexcept
	{
		return m_file.is_open();
	}

	template<typename T>
	[[nodiscard]] bool write(const T& value)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		return static_cast<bool>(m_file.write(reinterpret_cast<const char*>(&value), sizeof(T)));
	}

	[[nodiscard]] bool write(const std::wstring& value)
	{
		if (value.size() > std::numeric_limits<std::uint16_t>::max())
			return false;

		std::uint16_t length = static_cast<std::uint16_t>(value.size());
		if (!write(length))
			return false;

		m_file.write(reinterpret_cast<const char*>(value.data()), length * sizeof(wchar_t));
		return static_cast<bool>(m_file);
	}

	[[nodiscard]] bool write(const std::filesystem::path& value)
	{
		return write(value.wstring());
	}

private:
	std::ofstream m_file;
};

class BinaryReader
{
public:
	explicit BinaryReader(const std::filesystem::path& filePath) :
		m_file(filePath, std::ios::binary)
	{
	}

	BinaryReader(const BinaryReader&) = delete;
	BinaryReader& operator=(const BinaryReader&) = delete;

	[[nodiscard]] bool isOpen() const noexcept
	{
		return m_file.is_open();
	}

	template<typename T>
	[[nodiscard]] bool read(T& value)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		return static_cast<bool>(m_file.read(reinterpret_cast<char*>(&value), sizeof(T)));
	}

	[[nodiscard]] bool read(std::wstring& value, std::uint16_t maxLength = DEFAULT_MAX_STRING_LENGTH)
	{
		std::uint16_t length = 0;
		if (!read(length) || length > maxLength)
			return false;

		value.resize(length);
		m_file.read(reinterpret_cast<char*>(value.data()), length * sizeof(wchar_t));
		return static_cast<bool>(m_file);
	}

	[[nodiscard]] bool read(std::filesystem::path& path)
	{
		std::wstring temp;
		if (!read(temp))
			return false;

		path = temp;
		return true;
	}

private:
	std::ifstream m_file;

	static constexpr std::uint16_t DEFAULT_MAX_STRING_LENGTH = 1024;
};
