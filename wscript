#!/usr/bin/env python


import os
import shutil


bearlibterminaldeps = (['gdi32', 'winmm', 'opengl32'] if 'nt' in os.name else ['opengl'])
sources_freetype = list(map(lambda x: 'external/BearLibTerminal/Terminal/Dependencies/FreeType/Source/' + x + '.c',
	                          ['autofit/autofit', 'base/ftbase', 'base/ftbbox', 'base/ftdebug', 'base/ftfstype', 'base/ftgasp', 'base/ftglyph', 'base/ftinit',
	                          'base/ftlcdfil', 'base/ftmm', 'base/ftsystem', 'raster/raster', 'sfnt/sfnt', 'smooth/smooth', 'truetype/truetype']))
defines_freetype = list(map(lambda x: '-DFT_INTERNAL_' + x[0] + '_H="freetype/internal/' + x[1] + '.h"',
	                          [('OBJECTS', 'ftobjs'), ('MEMORY', 'ftmemory'), ('GLYPH_LOADER', 'ftgloadr'), ('DRIVER', 'ftdriver'), ('AUTOHINT', 'autohint'),
	                           ('SERVICE', 'ftserv'), ('PIC', 'ftpic'), ('DEBUG', 'ftdebug'), ('CALC', 'ftcalc'), ('VALIDATE', 'ftvalid'), ('RFORK', 'ftrfork'),
	                           ('SFNT', 'sfnt'), ('STREAM', 'ftstream'), ('TRUETYPE_TYPES', 'tttypes'), ('TYPE1_TYPES', 't1types'), ('POSTSCRIPT_HINTS', 'pshints')]))

def options(opts):
	opts.load('compiler_cxx')
	opts.load('compiler_c')

def configure(conf):
	conf.load('compiler_cxx')
	conf.load('compiler_c')
	conf.env.append_value('INCLUDES', ['.', '../external/property_tree', '../external/tinydir', '../external/BearLibTerminal/Terminal/Include/C'] +
		                                list(map(lambda x: '../external/BearLibTerminal/Terminal/Dependencies/' + x + '/Include', ['FreeType', 'NanoJPEG', 'PicoPNG'])))
	conf.check(features='c cstlib', cflags=['-std=c11', '-O3', '-pipe'] + defines_freetype, uselib_store='F')
	conf.check(features='cxx cxxprogram', cxxflags=['-std=c++14', '-Wall', '-Wextra', '-O3', '-pedantic', '-pipe'], uselib_store='M')
	conf.check(features='cxx cxxstlib', cxxflags=['-std=c++14', '-Wno-deprecated-register', '-Wno-uninitialized', '-Wno-potentially-evaluated-expression',
		                                            '-Wno-return-type', '-O3','-pipe'], uselib_store='T')
	for dep in bearlibterminaldeps:
		conf.check(features='cxx cxxshlib', lib=dep, uselib_store='T')
	conf.write_config_header('config.h')

def build(buld):
	buld(features='c cstlib', source=sources_freetype, target='freetype', use='F')
	buld(features='cxx cxxstlib', source=buld.path.ant_glob('external/BearLibTerminal/Terminal/Dependencies/PicoPNG/Source/**/*.cpp'), target='picopng', use='T')
	buld(features='cxx cxxshlib', source=buld.path.ant_glob('external/BearLibTerminal/Terminal/Source/**/*.cpp'), target='bearlibterminal', use=['T', 'freetype', 'picopng'])
	buld(features='cxx cxxprogram', source=buld.path.ant_glob('source/**/*.cpp'), target='gaem', use=['M', 'bearlibterminal'])
	buld(rule=copyassets, always=True)

def copyassets(self):
	inassets  = self.generator.bld.path.find_dir('assets').abspath()
	outassets = self.generator.bld.path.get_bld().abspath() + '/assets'
	shutil.rmtree(outassets, True)
	shutil.copytree(inassets, outassets)
