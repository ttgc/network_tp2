#include "streams.hpp"

void MemoryStream::Write(gsl::not_null<void*> data, size_t length)
{
    m_buffer.insert(std::end(m_buffer), 
                reinterpret_cast<std::byte*>(data.get()), 
                reinterpret_cast<std::byte*>(data.get()) + length);
}

void MemoryStream::WriteStr(const std::string& data)
{
    assert(data.length() < 0xffff);

    uint16_t strSize = static_cast<uint16_t>(data.size());
    Write<uint16_t>(strSize);

    m_buffer.insert(std::end(m_buffer), 
                reinterpret_cast<const std::byte*>(data.data()), 
                reinterpret_cast<const std::byte*>(data.data()) + data.length());
}

std::string MemoryStream::ReadStr()
{
    uint16_t strSize = Read<uint16_t>();
    auto str = Read(strSize);
	std::vector<char> buffer(strSize);

    std::transform(str.begin(), str.end(), buffer.begin(), [](std::byte b){ return (char)b; });
	std::string output(buffer.begin(), buffer.end());

    return output;
}

void MemoryStream::Write(gsl::span<const char> data)
{
    m_buffer.insert(std::end(m_buffer), 
                reinterpret_cast<const std::byte*>(data.data()), 
                reinterpret_cast<const std::byte*>(data.data()) + data.size_bytes());
}

void MemoryStream::Write(gsl::span<std::byte> data)
{
    m_buffer.insert(std::end(m_buffer), data.data(), data.data() + data.size_bytes());
}