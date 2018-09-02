#pragma once

/*
 * Copyright (c) 2017, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <memory>
#include <type_traits>

PYBIND11_DECLARE_HOLDER_TYPE(T, std::unique_ptr<T>);
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

#ifdef __GNUG__
#  define SPACETIME_PYTHON_WRAPPER_VISIBILITY __attribute__((visibility("hidden")))
#else
#  define SPACETIME_PYTHON_WRAPPER_VISIBILITY
#endif

namespace spacetime
{

namespace python
{

/**
 * Helper template for pybind11 class wrappers.
 */
template< class Wrapper, class Wrapped, class Holder = std::unique_ptr<Wrapped>, class WrappedBase = Wrapped >
class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapBase {

public:

    using wrapper_type = Wrapper;
    using wrapped_type = Wrapped;
    using wrapped_base_type = WrappedBase;
    using holder_type = Holder;
    using base_type = WrapBase< wrapper_type, wrapped_type, holder_type, wrapped_base_type >;
    using class_ = typename std::conditional<
        std::is_same< Wrapped, WrappedBase >::value
      , pybind11::class_< wrapped_type, holder_type >
      , pybind11::class_< wrapped_type, wrapped_base_type, holder_type >
    >::type;

    static wrapper_type & commit(pybind11::module & mod, const char * pyname, const char * clsdoc) {
        static wrapper_type derived(mod, pyname, clsdoc);
        return derived;
    }

    WrapBase() = delete;
    WrapBase(WrapBase const & ) = default;
    WrapBase(WrapBase       &&) = delete;
    WrapBase & operator=(WrapBase const & ) = default;
    WrapBase & operator=(WrapBase       &&) = delete;

#define DECL_PYBIND_CLASS_METHOD(METHOD) \
    template< class... Args > \
    wrapper_type & METHOD(Args&&... args) { \
        m_cls.METHOD(std::forward<Args>(args)...); \
        return *static_cast<wrapper_type*>(this); \
    }

    DECL_PYBIND_CLASS_METHOD(def)
    DECL_PYBIND_CLASS_METHOD(def_readwrite)
    DECL_PYBIND_CLASS_METHOD(def_property)
    DECL_PYBIND_CLASS_METHOD(def_property_readonly)
    DECL_PYBIND_CLASS_METHOD(def_property_readonly_static)

#undef DECL_PYBIND_CLASS_METHOD

protected:

    WrapBase(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : m_cls(mod, pyname, clsdoc)
    {}

    class_ m_cls;

}; /* end class WrapBase */

class ModuleInitializer {

public:

    static ModuleInitializer & get_instance() {
        static ModuleInitializer inst;
        return inst;
    }

    void initialize(pybind11::module & topmod) {
        if (!m_initialized) {
            initialize_spacetime(topmod);
        }
        m_initialized = true;
    }

    bool is_initialized() const { return m_initialized; }

private:

    PyObject * initialize_spacetime(pybind11::module & topmod);

    ModuleInitializer() = default;
    ModuleInitializer(ModuleInitializer const & ) = delete;
    ModuleInitializer(ModuleInitializer       &&) = delete;
    ModuleInitializer & operator=(ModuleInitializer const & ) = delete;
    ModuleInitializer & operator=(ModuleInitializer       &&) = delete;

    bool m_initialized = false;

}; /* end class ModuleInitializer */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set ff=unix fenc=utf8 nobomb et sw=4 ts=4:
