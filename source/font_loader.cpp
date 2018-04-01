// font_loader.cpp

#include <ft2build.h>
#include FT_FREETYPE_H

struct dt_FontInfo_s;

dt_FontInfo_s* load_font(
	char const* const font
) {
	FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		return nullptr;
	}

	FT_Face font_face;
	error = FT_New_Face(library, font, 0, &font_face);
	if (error)
	{
		return nullptr;
	}

	//error = FT_Set_Char_Size(font_face, 0, )

	return nullptr;
}
