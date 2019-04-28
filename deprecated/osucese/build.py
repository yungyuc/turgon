"""
This is the build script.
"""

if __name__ == '__main__':
    import sys
    if len(sys.argv) == 1:
        sys.argv.append('build')
    from core1d.distutils import build
    build()
