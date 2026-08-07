#pragma once

#include <fstream>
#include <filesystem>
#include <type_traits>

class BinaryWriter
{
public:
	explicit BinaryWriter(const std::filesystem::path& filePath) :
		m_file(filePath, std::ios::binary)
	{
	}

	BinaryWriter(const BinaryWriter&) = delete;
	BinaryWriter& operator=(const BinaryWriter&) = delete;

	bool isOpen() const noexcept
	{
		return m_file.is_open();
	}

	template<typename T>
	bool write(const T& value)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		return static_cast<bool>(m_file.write(reinterpret_cast<const char*>(&value), sizeof(T)));
	}

	bool write(const std::wstring& value)
	{
		if (value.size() > std::numeric_limits<std::uint16_t>::max())
			return false;

		std::uint16_t length = static_cast<std::uint16_t>(value.size());
		write(length);
		m_file.write(reinterpret_cast<const char*>(value.data()), length * sizeof(wchar_t));
		return static_cast<bool>(m_file);
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

	bool isOpen() const noexcept
	{
		return m_file.is_open();
	}

	template<typename T>
	bool read(T& value)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		return static_cast<bool>(m_file.read(reinterpret_cast<char*>(&value), sizeof(T)));
	}

	bool read(std::wstring& value, std::uint16_t maxLength = MAX_STRING_LENGTH_DEFAULT)
	{
		std::uint16_t length = 0;
		if (!read(length) || length > maxLength)
			return false;
		value.resize(length);

		m_file.read(reinterpret_cast<char*>(value.data()), length * sizeof(wchar_t));
		return static_cast<bool>(m_file);
	}

private:
	std::ifstream m_file;

	static constexpr std::uint16_t MAX_STRING_LENGTH_DEFAULT = 1024;
};
