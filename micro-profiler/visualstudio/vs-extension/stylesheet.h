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

#pragma once

#include <wpl/stylesheet.h>
#include <wpl/visual.h>

typedef struct IVsUIShell IVsUIShell;
typedef struct IVsFontAndColorStorage IVsFontAndColorStorage;

namespace micro_profiler
{
	namespace integration
	{
		class vsstylesheet : public wpl::stylesheet_db
		{
		public:
			vsstylesheet(wpl::signal<void ()> &update, wpl::gcontext::text_engine_type &text_engine, IVsUIShell &shell,
				IVsFontAndColorStorage &font_and_color);

		private:
			void synchronize(wpl::gcontext::text_engine_type &text_engine, IVsUIShell &vsshell,
				IVsFontAndColorStorage &fonts_and_colors);

		private:
			wpl::slot_connection _update_conn;
		};
	}
}