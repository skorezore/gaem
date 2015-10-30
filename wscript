#!/usr/bin/env python


import os
import shutil


curseslib = ('pdcurses' if 'nt' in os.name else 'ncurses')


def options(opts):
	opts.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')
	conf.check(features='cxx cxxprogram', cxxflags=['-std=c++14', '-Wall', '-Wextra', '-O3', '-pedantic', '-pipe'], uselib_store='M')
	conf.check(features='cxx cxxprogram', lib=curseslib, uselib_store='M')
	conf.check(features='cxx cxxprogram', header_name='curses.h', mandatory=False)
	conf.check(features='cxx cxxprogram', header_name='ncurses.h', mandatory=False)
	conf.check(fragment    = '''
	                           #include <stdio.h>
	                           int main() {
	                             #if HAVE_CURSES_H
	                               printf("curses.h");
	                             #elif HAVE_NCURSES_H
	                               printf("ncurses.h");
	                             #else
	                               return 1;
	                             #endif
	                             return 0;
	                           }
	                        ''',
	           define_name = 'CURSES_LIB',
	           execute     = True,
	           define_ret  = True,
	           mandatory   = True)
	conf.write_config_header('config.h')

def build(buld):
	buld(features='cxx cxxprogram', source=buld.path.ant_glob('source/**/*.cpp'), target='gaem', use='M', includes='../external/property_tree ../external/tinydir')
	buld(rule=copyassets, always=True)

def copyassets(self):
	inassets  = self.generator.bld.path.find_dir('assets').abspath()
	outassets = self.generator.bld.path.get_bld().abspath() + '/assets'
	shutil.rmtree(outassets, True)
	shutil.copytree(inassets, outassets)
