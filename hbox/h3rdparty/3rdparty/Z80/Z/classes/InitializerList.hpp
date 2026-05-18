/* Zeta API - h3rdparty/3rdparty/Z80/Z/classes/InitializerList.hpp
 ______  ______________  ___
|__   / |  ___|___  ___|/   \
  /  /__|  __|   |  |  /  -  \
 /______|_____|  |__| /__/ \__\
Copyright (C) 2006-2025 Manuel Sainz de Baranda y Goñi.
Released under the terms of the GNU Lesser General Public License v3. */

#ifndef Z_classes_InitializerList_HPP
#define Z_classes_InitializerList_HPP

#include <h3rdparty/3rdparty/Z80/Z/inspection/language.h>

#if Z_DIALECT_HAS(CPP11, INITIALIZER_LIST)
#	include <h3rdparty/3rdparty/Z80/Z/constants/pointer.h>
#	include <h3rdparty/3rdparty/Z80/Z/macros/language.hpp>
#	include <h3rdparty/3rdparty/Z80/Z/types/integral.hpp>

#	define Z_HAS_InitializerList 1

	namespace Zeta {template <class t> class InitializerList {
		public:
		typedef t	 value_type;
		typedef const t& reference;
		typedef const t& const_reference;
		typedef USize	 size_type;
		typedef const t* iterator;
		typedef const t* const_iterator;

		private:
		const t* _data;
		USize	 _size;

		public:
		Z_CT(CPP11) InitializerList() Z_NOTHROW
		: _data(Z_NULL), _size(0) {}

		Z_CT(CPP11) USize    size () const Z_NOTHROW {return _size;}
		Z_CT(CPP11) const t* begin() const Z_NOTHROW {return _data;}
		Z_CT(CPP11) const t* end  () const Z_NOTHROW {return _data + _size;}
	};}
#endif

#endif // Z_classes_InitializerList_HPP
