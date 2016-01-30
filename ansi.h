#ifndef ANSI_H
#define ANSI_H

#include <iostream>

namespace ansi
{

	template<typename T> using Stream = std::basic_ostream<char,T>;

	template<typename T>
	inline Stream<T>& sgr(Stream<T>& s,const char* c)
	{ s << "\x1b[" << c << "m"; return s; }

	template<typename T> inline Stream<T>& reset(Stream<T>& s)   { return sgr<T>(s,"0"); }

	template<typename T> inline Stream<T>& bold(Stream<T>& s)    { return sgr<T>(s,"1"); }
	template<typename T> inline Stream<T>& nobold(Stream<T>& s)  { return sgr<T>(s,"22"); }

	template<typename T> inline Stream<T>& black(Stream<T>& s)   { return sgr<T>(s,"30"); }
	template<typename T> inline Stream<T>& red(Stream<T>& s)     { return sgr<T>(s,"31"); }
	template<typename T> inline Stream<T>& green(Stream<T>& s)   { return sgr<T>(s,"32"); }
	template<typename T> inline Stream<T>& yellow(Stream<T>& s)  { return sgr<T>(s,"33"); }
	template<typename T> inline Stream<T>& blue(Stream<T>& s)    { return sgr<T>(s,"34"); }
	template<typename T> inline Stream<T>& magenta(Stream<T>& s) { return sgr<T>(s,"35"); }
	template<typename T> inline Stream<T>& cyan(Stream<T>& s)    { return sgr<T>(s,"36"); }
	template<typename T> inline Stream<T>& white(Stream<T>& s)   { return sgr<T>(s,"37"); }

	template<typename T> inline Stream<T>& black_br(Stream<T>& s)   { return sgr<T>(s,"90"); }
	template<typename T> inline Stream<T>& red_br(Stream<T>& s)     { return sgr<T>(s,"91"); }
	template<typename T> inline Stream<T>& green_br(Stream<T>& s)   { return sgr<T>(s,"92"); }
	template<typename T> inline Stream<T>& yellow_br(Stream<T>& s)  { return sgr<T>(s,"93"); }
	template<typename T> inline Stream<T>& blue_br(Stream<T>& s)    { return sgr<T>(s,"94"); }
	template<typename T> inline Stream<T>& magenta_br(Stream<T>& s) { return sgr<T>(s,"95"); }
	template<typename T> inline Stream<T>& cyan_br(Stream<T>& s)    { return sgr<T>(s,"96"); }
	template<typename T> inline Stream<T>& white_br(Stream<T>& s)   { return sgr<T>(s,"97"); }

}

#endif
