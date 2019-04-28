def getF90Extension(name, basepath=None, sources=None, 
        extra_objects=None, libraries=None):
    """
    Create Numpy extension object for F90 source files.

    @param name: Designated extension module name.
    @type name: str
    @keyword basename: Designated path for source files.  Default None will 
                       use module name as path.
    @type basename: str
    @param sources: Source files included.
    @type sources: list/str
    @param extra_objects: Extra object files.
    @type extra_objects: str
    @param libraries: Libraries to link.
    @type libraries: str
    """
    from glob import glob
    from numpy.distutils.core import Extension
    if basepath is None:
        basepath = "/".join(name.split(".")) + "_fsrc"
    if sources is None:
        sources = glob("/".join([basepath, "*.f90"]))
    elif isinstance(sources, list):
        sources = ["/".join([basepath, fn]) for fn in 
                   sources]
    else:
        sources = ["/".join([basepath, fn]) for fn in 
                   sources.strip().split()]
    if extra_objects:
        extra_objects = ["/".join([basepath, fn]) for fn in 
                         extra_objects.strip().split()]
    if libraries:
        libraries = libraries.strip().split()
    return Extension(name=name, sources=sources, 
        extra_objects=extra_objects, libraries=libraries)

def build(module_names=None):
    """
    Build assigned extension modules.

    @keyword module_names: Module names to be built.
    @type module_names: list of str
    """
    from numpy.distutils.core import setup
    def_module_names = ['core1d._core',]
    if module_names is None: module_names = []
    ext_modules = [getF90Extension(name=name) for name in 
        sorted(list(set(def_module_names + module_names)))]
    setup(ext_modules=ext_modules)
