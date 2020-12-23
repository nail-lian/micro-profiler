//	Copyright (c) 2011-2020 by Artem A. Gevorkyan (gevorkyan.org)
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#include <frontend/about_ui.h>

#include <common/string.h>

#include <wpl/controls.h>
#include <wpl/factory.h>
#include <wpl/types.h>

using namespace std;
using namespace std::placeholders;
using namespace wpl;

namespace micro_profiler
{
	namespace
	{
		function<int (int)> scale_x()
		{	return [] (int value) {	return value;	};	}

		function<int (int)> scale_y()
		{	return [] (int value) {	return value;	};	}
	}


	about_ui::about_ui(const wpl::factory &factory_)
		: stack(scale_y()(5), false), _close_button(static_pointer_cast<button>(factory_.create_control("button"))),
			close(_close_button->clicked)
	{
		function<int (int)> xx = scale_x();
		function<int (int)> yy = scale_y();
		shared_ptr<wpl::link> link;
		shared_ptr<stack> inner_stack;

		add(link = factory_.create_control<wpl::link>("link"), xx(40));
			link->set_text(L"Please, take any of these steps to support the development of MicroProfiler:");

		add(pad_control(inner_stack = make_shared<stack>(10, false), xx(10), 0), -100);
			inner_stack->add(link = factory_.create_control<wpl::link>("link"), yy(20), 1);
				link->set_text(L"1. Leave a review on <a href=\"https://marketplace.visualstudio.com/items?itemName=ArtemGevorkyan.MicroProfilerx64x86#review-details\">Visual Studio Marketplace</a>");
				_connections.push_back(link->clicked += bind(&about_ui::on_link, this, _2));

			inner_stack->add(link = factory_.create_control<wpl::link>("link"), yy(20), 2);
				link->set_text(L"2. Write <a href=\"https://github.com/tyoma/micro-profiler/issues\">an issue or a suggestion</a>");
				_connections.push_back(link->clicked += bind(&about_ui::on_link, this, _2));


			inner_stack->add(link = factory_.create_control<wpl::link>("link"), yy(20), 3);
				link->set_text(L"3. Star MicroProfiler on <a href=\"https://github.com/tyoma/micro-profiler\">github.com</a>");
				_connections.push_back(link->clicked += bind(&about_ui::on_link, this, _2));

		add(_close_button, yy(30), 4);
			_close_button->set_text(L"Thank You!");
	}

	void about_ui::on_link(const wstring &address)
	{	link(address);	}
}
