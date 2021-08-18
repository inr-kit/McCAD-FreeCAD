import FreeCAD, FreeCADGui, os

def modulePath():
    """returns the current McCad design module path
    @return Module path"""
    path1 = FreeCAD.ConfigGet("AppHomePath") + "Mod/McCad"
    path2 = FreeCAD.ConfigGet("UserAppData") + "Mod/McCad"
    if os.path.exists(path2):
        return path2
    else:
        return path1

def iconsPath():
    """returns the current Ship design module icons path
    @return Icons path"""
    path = modulePath() + "/Resources/icons"
    return path

def getPathFromFile(fileName):
    """ Gets the directory path from a file name
    @param fileName Name of the file
    @return Directory path.
    """
    if not fileName:
        return ''
    i = 1
    try:
        while 1:
            i = fileName.index("/", i+1)
    except ValueError:
        pass
    return fileName[0:i+1]
