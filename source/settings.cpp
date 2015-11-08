// The MIT License (MIT)

// Copyright (c) 2015 Skorezore

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


#include "settings.hpp"
#include "util/quickscope_wrapper.hpp"
#include "boost/property_tree/info_parser.hpp"
#include <fstream>


using namespace std;
using namespace std::chrono;

namespace pt = boost::property_tree;


const gaem_settings & settings() {
	static gaem_settings setts = [&]() {
		pt::ptree tree;
		ifstream ifs("assets/gaem.cfg");  // Force-create
		pt::read_info(ifs, tree);

		gaem_settings tmp;

		tmp.graphics.between_frames    = milliseconds(tree.get("graphics.time_between_frames", 75));
		tmp.credits.time_between_lines = tree.get("credits.time_between_lines", 400);

		return tmp;
	}();
	static quickscope_wrapper saviour{[&]() {
		pt::ptree tree;

		tree.put("graphics.time_between_frames", setts.graphics.between_frames.count());
		tree.put("graphics.time_between_frames^", "In milliseconds"s);
		tree.put("credits.time_between_lines", setts.credits.time_between_lines);
		tree.put("credits.time_between_lines^", "In milliseconds"s);

		pt::write_info("assets/gaem.cfg", tree);
	}};

	return setts;
}
