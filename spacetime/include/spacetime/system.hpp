#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <sstream>

namespace spacetime
{

// Taken from https://stackoverflow.com/a/12262626
class Formatter
{

public:

    Formatter() = default; // NOLINT(fuchsia-default-arguments)
    Formatter(Formatter const & ) = delete;
    Formatter(Formatter       &&) = delete;
    Formatter & operator = (Formatter const & ) = delete;
    Formatter & operator = (Formatter       &&) = delete;
    ~Formatter() = default;

    template <typename T>
    Formatter & operator << (T const & value) // NOLINT(fuchsia-overloaded-operator)
    {
        m_stream << value;
        return *this;
    }

    std::string str() const       { return m_stream.str(); }
    // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
    operator std::string () const { return m_stream.str(); }

    enum ConvertToString
    {
        to_str
    };

    // NOLINTNEXTLINE(fuchsia-overloaded-operator)
    std::string operator >> (ConvertToString const &) { return m_stream.str(); }

private:

    std::ostringstream m_stream;

}; /* end class Formatter */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
