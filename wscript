#!/usr/bin/env python3.5


import glob


CXX_FLAGS=['-std=c++14', '-Wall', '-Wextra', '-O3', '-pedantic', '-pipe']


def options(opts):
	opts.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')
	conf.check(features='cxx cxxprogram cxxstlib', cxxflags=CXX_FLAGS, uselib_store='M')

def build(buld):
	buld(features='cxx cxxstlib', source=glob.glob('external/Cpponfiguration/src/**/*.cpp', recursive=True), target='cpponfiguration', use='M')#, cxxflags=CXX_FLAGS)
	buld(features='cxx cxxprogram', source=glob.glob('source/**/*.cpp', recursive=True), target='gaem', use=['M', 'cpponfiguration'], cxxflags=['-I../external/Cpponfiguration/include', '-I../external/tinydir'], lib=['curses'])

