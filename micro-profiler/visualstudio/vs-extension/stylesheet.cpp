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

#include "stylesheet.h"

#include <agge.text/text_engine.h>
#include <atlbase.h>
#include <vsshell80.h>
#include <vsshell100.h>

using namespace agge;
using namespace std;
using namespace wpl;

namespace micro_profiler
{
	namespace integration
	{
		namespace
		{
			const GUID c_environment_category = { 0x1F987C00, 0xE7C4, 0x4869, { 0x8A, 0x17, 0x23, 0xFD, 0x60, 0x22, 0x68, 0xB0 } };

			color get_syscolor(IVsUIShell &vsshell, VSSYSCOLOREX color_index)
			{
				DWORD c;

				if (CComQIPtr<IVsUIShell2> vsshell2 = &vsshell)
				{
					return vsshell2->GetVSSysColorEx(color_index, &c) == S_OK
						? color::make(GetRValue(c), GetGValue(c), GetBValue(c)) : color::make(0, 0, 0, 0);
				}
				else
				{
					return vsshell.GetVSSysColor((VSSYSCOLOR)color_index, &c) == S_OK
						? color::make(GetRValue(c), GetGValue(c), GetBValue(c)) : color::make(0, 0, 0, 0);
				}
			}

			color get_syscolor(int color_kind)
			{
				auto c = ::GetSysColor(color_kind);
				return color::make(GetRValue(c), GetGValue(c), GetBValue(c));
			}

			color invert(color original)
			{	return color::make(~original.r, ~original.g, ~original.b, original.a);	}

			color semi(color original, double opacity)
			{	return color::make(original.r, original.g, original.b, static_cast<agge::uint8_t>(opacity * 255));	}

			color interpolate(color a, color b, double k)
			{
				return color::make(
					static_cast<agge::uint8_t>((1 - k) * a.r + k * b.r),
					static_cast<agge::uint8_t>((1 - k) * a.g + k * b.g),
					static_cast<agge::uint8_t>((1 - k) * a.b + k * b.b),
					static_cast<agge::uint8_t>((1 - k) * a.a + k * b.a)
				);
			}
		}

		vsstylesheet::vsstylesheet(signal<void ()> &update, gcontext::text_engine_type &text_engine, IVsUIShell &shell,
			IVsFontAndColorStorage &font_and_color)
		{
			_update_conn = update += [this, &text_engine, &shell, &font_and_color] {
				synchronize(text_engine, shell, font_and_color);
			};
			synchronize(text_engine, shell, font_and_color);
		}

		void vsstylesheet::synchronize(gcontext::text_engine_type &text_engine, IVsUIShell &vsshell,
			IVsFontAndColorStorage &fonts_and_colors)
		{
			// control background: VSCOLOR_DESIGNER_BACKGROUND, -45;
			// background: VSCOLOR_ENVIRONMENT_BACKGROUND, -53;
			// border: VSCOLOR_ACCENT_BORDER, -5;
			// text: VSCOLOR_PANEL_TEXT, -106;

			set_color("background", get_syscolor(vsshell, VSCOLOR_ENVIRONMENT_BACKGROUND));
			set_color("background.selected", get_syscolor(vsshell, VSCOLOR_HIGHLIGHT));
			set_color("background.header", get_syscolor(vsshell, VSCOLOR_LIGHT));
			set_color("background.listview", get_syscolor(vsshell, VSCOLOR_LIGHT));
			set_color("background.listview.odd", interpolate(get_syscolor(vsshell, VSCOLOR_LIGHT),
				get_syscolor(vsshell, VSCOLOR_PANEL_TEXT), 0.03));
			set_color("border", get_syscolor(vsshell, VSCOLOR_DARK));
			set_color("text", get_syscolor(vsshell, VSCOLOR_PANEL_TEXT));
			set_color("text.selected", get_syscolor(COLOR_HIGHLIGHTTEXT));

			set_value("border", 1.0f);
			set_value("padding", 3.0f);

			LOGFONTW lf = {};
			FontInfo fi = {};

			fonts_and_colors.OpenCategory(c_environment_category, FCSF_READONLY);
			fonts_and_colors.GetFont(&lf, &fi);
			fonts_and_colors.CloseCategory();

			font::key fk(lf.lfFaceName[0] ? lf.lfFaceName : L"Segoe UI", abs(lf.lfHeight), lf.lfWeight > FW_NORMAL,
				!!lf.lfItalic, agge::font::key::gf_strong);

			set_font("text", text_engine.create_font(fk.typeface.c_str(), fk.height, fk.bold, fk.italic, fk.grid_fit_));
			set_font("text.header", text_engine.create_font(fk.typeface.c_str(), fk.height, true, fk.italic, fk.grid_fit_));

			changed();
		}
	}
}
