// dt_build_private.h
#pragma once

#ifdef _MSC_VER
/*
* "formal parameter different from declaration"
* MSVC generates this for adding 'const' to value-type parameters when defining a function.
* This has no effect on the caller, and just adds noise when reading headers. This is a poorly designed warning.
*/
#	pragma warning(disable: 4028)
#endif
