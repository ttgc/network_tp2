#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <cassert>

#include <gsl/gsl>

#include "utils.hpp"

class MemoryStream
{
public:
    MemoryStream() : m_cursor(0)
    {
        m_buffer.reserve(64);
    }

    void Write(gsl::not_null<void*> data, size_t length);
    void Write(gsl::span<const char> data);
    void Write(gsl::span<std::byte> data);
    void WriteStr(const std::string& data);

    template<typename T>
    void Write(T& data)
    {
        static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "A generic write only support primitive data type");

        if(ms_streamEndianess != DetectEndianness())
        {
            T temp = data;
            auto val = SwapEndian(temp);
            Write((void*)&val, sizeof(val));
            return;
        }

        Write((void*)&data, sizeof(data));
    }

    size_t Size() { return m_buffer.size(); }
    size_t RemainingSize() const { return m_buffer.size() - m_cursor; }

    gsl::span<std::byte> Read(size_t length)
    {
        assert(length <= RemainingSize());
        m_cursor += length;
        return gsl::make_span(m_buffer.data() + (m_cursor - length), length);
    }

    template<typename T>
    T Read()
    {
        static_assert(std::is_copy_constructible<T>::value && 
                    (std::is_arithmetic<T>::value || std::is_enum<T>::value), "A generic write only support primitive data type");
        auto data = Read(sizeof(T));
        T* outData = reinterpret_cast<T*>(data.data());
        
        if(ms_streamEndianess != DetectEndianness())
        {
            *outData = SwapEndian(*outData);
        }

        return T(*outData);
    }

    std::string ReadStr();
    
    void Flush()
    {
        m_cursor = 0;
        m_buffer.clear();
    }

    gsl::span<std::byte> Data() { return m_buffer; }
protected:
    std::vector<std::byte> m_buffer;
    size_t m_cursor = 0;

    static constexpr PlatformEndianness ms_streamEndianess = PlatformEndianness::LittleEndian;
};

class InputStream : public MemoryStream
{
public:
    InputStream(gsl::span<char> data)
    {
        Reinit(data);
    }

    InputStream(gsl::span<std::byte> data)
    {
        Reinit(data);
    }

    void Reinit(gsl::span<char> data)
    {
        Flush();
        m_buffer.reserve(data.size_bytes());
        m_buffer.insert(std::begin(m_buffer), 
                        reinterpret_cast<std::byte*>(data.data()), 
                        reinterpret_cast<std::byte*>(data.data()) + data.size_bytes());
    }

    void Reinit(gsl::span<std::byte> data)
    {
        Flush();
        m_buffer.reserve(data.size_bytes());
        m_buffer.insert(std::begin(m_buffer), 
                        data.data(), 
                        data.data() + data.size_bytes());
    }
};

class OutputStream : public MemoryStream
{
};