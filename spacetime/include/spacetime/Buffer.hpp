#pragma once

/*
 * Copyright (c) 2016, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see LICENSE.txt
 */

#include <type_traits>
#include <stdexcept>
#include <memory>

namespace spacetime
{

/**
 * Untyped and unresizeable memory buffer for data storage.
 */
class ByteBuffer
  : public std::enable_shared_from_this<ByteBuffer>
{

private:

    struct ctor_passkey {};

public:

    static std::shared_ptr<ByteBuffer> construct(size_t nbyte)
    {
        return std::make_shared<ByteBuffer>(nbyte, ctor_passkey());
    }

    /**
     * \param[in] length Memory buffer length.
     */
    ByteBuffer(size_t nbyte, const ctor_passkey &)
      : m_nbyte(nbyte)
      , m_data(allocate(nbyte))
    {}

    ~ByteBuffer() = default;

    ByteBuffer() = delete;
    ByteBuffer(ByteBuffer const & ) = delete;
    ByteBuffer(ByteBuffer       &&) = delete;

    ByteBuffer & operator=(ByteBuffer const & ) = delete;
    ByteBuffer & operator=(ByteBuffer       &&) = delete;

    explicit operator bool() const { return bool(m_data); }

    size_t nbyte() const { return m_nbyte; }

    size_t size() const { return nbyte(); }

    /* Backdoor */
    char * data() const { return data<char>(); }

    template< typename T >
    T * data() const
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return reinterpret_cast<T*>(m_data.get());
    }

private:

    // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    using unique_ptr_type = std::unique_ptr<char, std::default_delete<char[]>>;
    static_assert(sizeof(size_t) == sizeof(unique_ptr_type), "sizeof(Buffer::m_data) must be a word");

    static unique_ptr_type allocate(size_t nbyte)
    {
        unique_ptr_type ret;
        if (0 != nbyte)
        {
            ret = unique_ptr_type(new char[nbyte]);
        }
        else
        {
            ret = unique_ptr_type();
        }
        return ret;
    }

    size_t m_nbyte;
    unique_ptr_type m_data;

}; /* end class ByteBuffer */

template < typename T >
class Buffer
{

private:

    struct ctor_passkey {};

public:

    using value_type = T;

    constexpr static size_t ITEMSIZE = sizeof(value_type);

    constexpr static size_t itemsize() { return ITEMSIZE; }

    Buffer(size_t length, ctor_passkey const &)
      : m_byte(ByteBuffer::construct(length * ITEMSIZE))
    {}

    ~Buffer() = default;

    Buffer() = delete;
    Buffer(Buffer const & ) = delete;
    Buffer(Buffer       &&) = delete;
    Buffer & operator=(Buffer const & ) = delete;
    Buffer & operator=(Buffer       &&) = delete;

    size_t length() const { return m_byte->nbyte() / ITEMSIZE; }

    size_t size() const { return length(); }

    value_type const & operator[](size_t it) const { return (*m_byte).data()[it]; }
    value_type       & operator[](size_t it)       { return (*m_byte).data()[it]; }

    value_type const & at(size_t it) const { validate_range(it); return (*this)[it]; }
    value_type       & at(size_t it)       { validate_range(it); return (*this)[it]; }

private:

    void validate_range(size_t it) const
    {
        if (it >= size())
        {
            throw std::out_of_range("Buffer: index out of range");
        }
    }

    std::shared_ptr<ByteBuffer> m_byte;

}; /* end class Buffer */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
