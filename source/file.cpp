// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.


#include "file.hpp"
#include "tinydir.h"
#include <functional>


using namespace std;


struct quickscope_wrapper {
	function<void()> func;

	~quickscope_wrapper() {
		if(func)
			func();
	}
};


vector<string> list_files(const string & directory, list_type type) {
	vector<string> result;
	tinydir_dir dir;
	tinydir_open_sorted(&dir, directory.c_str());
	quickscope_wrapper asdf{[&]() { tinydir_close(&dir); }};

	result.reserve(dir.n_files);
	for(unsigned int i = 0; i < dir.n_files; i++) {
		tinydir_file file;
		tinydir_readfile_n(&dir, &file, i);

		if(type == list_type::all || (file.is_dir && type == list_type::directories) || (!file.is_dir && type == list_type::files))
			result.emplace_back(file.name);
	}

	return result;
}
