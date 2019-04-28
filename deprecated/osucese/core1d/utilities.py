def format_float(d):
    """
    Format of float's exponent is not consistent on Windows and Linux. 
    This function is designed to give consistent formatting for float 
    value, mainly for $\delta t$.
    """
    s = ("%e"%d).split('e')
    return "%.1fe%+d" % (float(s[0]), int(s[1]))
