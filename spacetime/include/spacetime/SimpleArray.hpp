#pragma once

/*
 * Copyright (c) 2020, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see LICENSE.txt
 */

#include "spacetime/ConcreteBuffer.hpp"

#include <stdexcept>

namespace spacetime
{

namespace detail
{

template < size_t D, typename S >
size_t buffer_offset_impl(S const &)
{
    return 0;
}

template < size_t D, typename S, typename Arg, typename ... Args >
size_t buffer_offset_impl(S const & strides, Arg arg, Args ... args)
{
    return arg * strides[D] + buffer_offset_impl<D+1>(strides, args...);
}

} /* end namespace detail */

template < typename S, typename ... Args >
size_t buffer_offset(S const & strides, Args ... args)
{
    return detail::buffer_offset_impl<0>(strides, args...);
}

template < typename T >
class SimpleArray
{

public:

    using value_type = T;
    using shape_type = small_vector<size_t>;

    static constexpr size_t ITEMSIZE = sizeof(value_type);

    static constexpr size_t itemsize() { return ITEMSIZE; }

    explicit SimpleArray(size_t length)
      : m_buffer(ConcreteBuffer::construct(length * ITEMSIZE))
      , m_shape{length}
      , m_stride{1}
    {}

    explicit SimpleArray(std::vector<size_t> const & shape)
      : m_shape(shape)
      , m_stride(calc_stride(m_shape))
    {
        if (!m_shape.empty())
        {
            m_buffer = ConcreteBuffer::construct(m_shape[0] * m_stride[0] * ITEMSIZE);
        }
    }

    static shape_type calc_stride(shape_type const & shape)
    {
        shape_type stride(shape.size());
        if (!shape.empty())
        {
            stride[shape.size()-1] = 1;
            for (size_t it=shape.size()-1; it>0; --it)
            {
                stride[it-1] = stride[it] * shape[it];
            }
        }
        return stride;
    }

    SimpleArray(std::initializer_list<T> init)
      : SimpleArray(init.size())
    {
        std::copy_n(init.begin(), init.size(), data());
    }

    SimpleArray() = default;

    SimpleArray(SimpleArray const & other)
      : m_buffer(other.m_buffer->clone())
      , m_shape(other.m_shape)
      , m_stride(other.m_stride)
    {}

    SimpleArray(SimpleArray && other) noexcept
      : m_buffer(std::move(other.m_buffer))
      , m_shape(std::move(other.m_shape))
      , m_stride(std::move(other.m_stride))
    {}

    SimpleArray & operator=(SimpleArray const & other)
    {
        if (this != &other)
        {
            *m_buffer = *(other.m_buffer); // Size is checked inside.
        }
        return *this;
    }

    SimpleArray & operator=(SimpleArray && other) noexcept
    {
        if (this != &other)
        {
            m_buffer = std::move(other.m_buffer);
            m_shape = std::move(other.m_shape);
            m_stride = std::move(other.m_stride);
        }
        return *this;
    }

    ~SimpleArray() = default;

    explicit operator bool() const { return bool(m_buffer); }

    size_t nbytes() const { return m_buffer ? m_buffer->nbytes() : 0; }
    size_t size() const { return nbytes() / ITEMSIZE; }

    value_type const & operator[](size_t it) const { return data(it); }
    value_type       & operator[](size_t it)       { return data(it); }

    value_type const & at(size_t it) const { validate_range(it); return data(it); }
    value_type       & at(size_t it)       { validate_range(it); return data(it); }

    size_t ndim() const { return m_shape.size(); }
    shape_type shape() const { return m_shape; }
    size_t shape(size_t it) const { return m_shape[it]; }

    template<class ... Args>
    value_type const & operator()(Args ... args) const { return data(buffer_offset(m_stride, args...)); }

    template<class ... Args>
    value_type       & operator()(Args ... args)       { return data(buffer_offset(m_stride, args...)); }

    /* Backdoor */
    value_type const * data() const { return buffer().template data<value_type>(); }
    value_type       * data()       { return buffer().template data<value_type>(); }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    value_type const & data(size_t it) const { return data()[it]; }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    value_type       & data(size_t it)       { return data()[it]; }

private:

    ConcreteBuffer const & buffer() const { return *m_buffer; }
    ConcreteBuffer       & buffer()       { return *m_buffer; }

    void validate_range(size_t it) const
    {
        if (it >= size())
        {
            throw std::out_of_range("Buffer: index out of range");
        }
    }

    std::shared_ptr<ConcreteBuffer> m_buffer;
    shape_type m_shape;
    shape_type m_stride;

}; /* end class SimpleArray */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
