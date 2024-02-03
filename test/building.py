#模拟环境
Env = None

from SCons.Script import *

def SetEnv(target_env):
    global Env
    Env = target_env

def GetCurrentDir():
    conscript = File('SConscript')
    fn = conscript.rfile()
    name = fn.name
    path = os.path.dirname(fn.abspath)
    return path

def _PretreatListParameters(target_list):
    while '' in target_list: # remove null strings
        target_list.remove('')
    while ' ' in target_list: # remove ' '
        target_list.remove(' ')

    if(len(target_list) == 0):
        return False # ignore this list, don't add this list to the parameter

    return True # permit to add this list to the parameter

def DefineGroup(name, src, depend, **parameters):
    global Env
    group = parameters
    group['name'] = name
    if type(src) == type([]):
        # remove duplicate elements from list
        src = list(set(src))
        group['src'] = File(src)
    else:
        group['src'] = src

    if 'CFLAGS' in group:
        target = group['CFLAGS']
        if len(target) > 0:
            Env.AppendUnique(CFLAGS = target)
    if 'CCFLAGS' in group:
        target = group['CCFLAGS']
        if len(target) > 0:
            Env.AppendUnique(CCFLAGS = target)
    if 'CXXFLAGS' in group:
        target = group['CXXFLAGS']
        if len(target) > 0:
            Env.AppendUnique(CXXFLAGS = target)
    if 'CPPPATH' in group:
        target = group['CPPPATH']
        if _PretreatListParameters(target) == True:
            paths = []
            for item in target:
                paths.append(os.path.abspath(item))
            target = paths
            Env.AppendUnique(CPPPATH = target)
    if 'CPPDEFINES' in group:
        target = group['CPPDEFINES']
        if _PretreatListParameters(target) == True:
            Env.AppendUnique(CPPDEFINES = target)

    if 'LIBS' in group:
        target = group['LIBS']
        if _PretreatListParameters(target) == True:
            Env.AppendUnique(LIBS = target)
    if 'LIBPATH' in group:
        target = group['LIBPATH']
        if _PretreatListParameters(target) == True:
            Env.AppendUnique(LIBPATH = target)

    # check whether to build group library
    if 'LIBRARY' in group:
        objs = Env.Library(name, group['src'])
    else:
        # only add source
        objs = group['src']

    return objs
