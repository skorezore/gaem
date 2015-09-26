#!/usr/bin/env python3


import os


curseslib = ('pdcurses' if 'nt' in os.name else 'ncurses')


def options(opts):
	opts.load('compiler_cxx')
	opts.recurse('external/Cpponfiguration')

def configure(conf):
	conf.load('compiler_cxx')
	conf.recurse('external/Cpponfiguration')
	conf.check(features='cxx cxxprogram', cxxflags=['-std=c++14', '-Wall', '-Wextra', '-O3', '-pedantic', '-pipe'], uselib_store='M')

def build(buld):
	buld.recurse('external/Cpponfiguration', 'build')
	buld(features='cxx cxxprogram', source=buld.path.ant_glob('source/**/*.cpp'), target='gaem', use=['M', 'cpponfig'], cxxflags=['-I../external/Cpponfiguration/include', '-I../external/tinydir'], lib=[curseslib])
