#pragma once

/*
 * Copyright (c) 2020, Yung-Yu Chen <yyc@solvcon.net>
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
class Buffer
  : public std::enable_shared_from_this<Buffer>
{

private:

    struct ctor_passkey {};

public:

    static std::shared_ptr<Buffer> construct(size_t nbytes)
    {
        return std::make_shared<Buffer>(nbytes, ctor_passkey());
    }

    static std::shared_ptr<Buffer> construct() { return construct(0); }

    std::shared_ptr<Buffer> clone() const
    {
        std::shared_ptr<Buffer> ret = construct(nbytes());
        std::copy_n(data(), size(), (*ret).data());
        return ret;
    }

    /**
     * \param[in] length Memory buffer length.
     */
    Buffer(size_t nbytes, const ctor_passkey &)
      : m_nbytes(nbytes)
      , m_data(allocate(nbytes))
    {}

    ~Buffer() = default;

    Buffer() = delete;
    Buffer(Buffer const & ) = delete;
    Buffer(Buffer       &&) = delete;
    Buffer & operator=(Buffer const & ) = delete;
    Buffer & operator=(Buffer       &&) = delete;

    explicit operator bool() const { return bool(m_data); }

    size_t nbytes() const { return m_nbytes; }
    size_t size() const { return nbytes(); }

    /* Backdoor */
    char const * data() const { return data<char>(); }
    char       * data()       { return data<char>(); }

    template< typename T >
    T const * data() const
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return reinterpret_cast<T const *>(m_data.get());
    }

    template< typename T >
    T * data()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return reinterpret_cast<T*>(m_data.get());
    }

private:

    // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    using unique_ptr_type = std::unique_ptr<char, std::default_delete<char[]>>;
    static_assert(sizeof(size_t) == sizeof(unique_ptr_type), "sizeof(Buffer::m_data) must be a word");

    static unique_ptr_type allocate(size_t nbytes)
    {
        unique_ptr_type ret;
        if (0 != nbytes)
        {
            ret = unique_ptr_type(new char[nbytes]);
        }
        else
        {
            ret = unique_ptr_type();
        }
        return ret;
    }

    size_t m_nbytes;
    unique_ptr_type m_data;

}; /* end class Buffer */

template < size_t D, typename S, typename Arg, typename ... Args>
size_t buffer_offset_impl(S const & strides, Arg arg, Args ... args)
{
    return arg * strides[D] + buffer_offset_impl<D+1>(strides, args...);
}

template < typename S, typename ... Args>
size_t buffer_offset(S const & strides, Args ... args)
{
    return buffer_offset_impl<0>(strides, args...);
}

template < typename T >
class Array
{

public:

    using value_type = T;
    using shape_type = std::vector<size_t>;

    constexpr static size_t ITEMSIZE = sizeof(value_type);

    constexpr static size_t itemsize() { return ITEMSIZE; }

    explicit Array(size_t length)
      : m_mb(Buffer::construct(length * ITEMSIZE))
      , m_shape{length}
      , m_stride{1}
    {}

    explicit Array(std::vector<size_t> const & shape)
      : m_shape(shape)
    {
        m_stride.resize(shape.size());
        for (size_t it=shape.size()-1; it>=0; --it)
        {
            size_t offset = 1;
            for (size_t jt=it+1; jt<shape.size(); ++jt)
            {
                offset *= shape[jt];
            }
            m_stride[it] = offset;
        }
        size_t nelem = 1;
        for (size_t it=0; it<shape.size(); ++it)
        {
            nelem *= shape[it];
        }
        m_mb = Buffer::construct(nelem * ITEMSIZE);
    }

    ~Array() = default;

    Array() = delete;
    Array(Array const & ) = delete;
    Array(Array       &&) = delete;
    Array & operator=(Array const & ) = delete;
    Array & operator=(Array       &&) = delete;

    size_t length() const { return m_mb->nbytes() / ITEMSIZE; }
    size_t size() const { return length(); }

    value_type const & operator[](size_t it) const { return mb().template data<value_type>()[it]; }
    value_type       & operator[](size_t it)       { return mb().template data<value_type>()[it]; }

    value_type const & at(size_t it) const { validate_range(it); return (*this)[it]; }
    value_type       & at(size_t it)       { validate_range(it); return (*this)[it]; }

    size_t ndim() const { return m_shape.size(); }
    std::vector<size_t> shape() const { return m_shape; }
    size_t shape(size_t it) const { return m_shape[it]; }

    template<class ... Args>
    value_type const & operator()(Args ... args) const
    {
        return data()[buffer_offset(m_stride, args...)];
    }

    template<class ... Args>
    value_type & operator()(Args ... args)
    {
        return data()[buffer_offset(m_stride, args...)];
    }

    /* Backdoor */
    value_type const * data() const { return mb().data(); }
    value_type       * data()       { return mb().data(); }

protected:

    Buffer const & mb() const { return *m_mb; }
    Buffer       & mb()       { return *m_mb; }

private:

    void validate_range(size_t it) const
    {
        if (it >= size())
        {
            throw std::out_of_range("Buffer: index out of range");
        }
    }

    std::shared_ptr<Buffer> m_mb;
    shape_type m_shape;
    shape_type m_stride;

}; /* end class Array */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
