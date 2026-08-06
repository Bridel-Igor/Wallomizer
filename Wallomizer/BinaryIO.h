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

private:
	std::ifstream m_file;
};
