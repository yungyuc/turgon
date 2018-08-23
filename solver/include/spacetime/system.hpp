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

    Formatter() {}
    Formatter(Formatter const & ) = delete;
    Formatter(Formatter       &&) = delete;
    Formatter & operator = (Formatter const & ) = delete;
    Formatter & operator = (Formatter       &&) = delete;

    template <typename T>
    Formatter & operator << (T const & value)
    {
        m_stream << value;
        return *this;
    }

    std::string str() const       { return m_stream.str(); }
    operator std::string () const { return m_stream.str(); }

    enum ConvertToString
    {
        to_str
    };
    std::string operator >> (ConvertToString) { return m_stream.str(); }

private:

    std::ostringstream m_stream;

}; /* end class Formatter */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
