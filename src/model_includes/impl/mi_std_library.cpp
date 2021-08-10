#include "model_includes/impl/mi_std_library.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace model_includes
{
//------------------------------------------------------------------------------

StdLibrary::StdLibrary() = default;

//------------------------------------------------------------------------------

const StdLibrary & StdLibrary::getInstance()
{
	static std::unique_ptr< StdLibrary > m_instance;
	if( !m_instance )
	{
		m_instance.reset( new StdLibrary );
		m_instance->init();
	}

	return *m_instance;
}

//------------------------------------------------------------------------------

bool StdLibrary::isExists( std::string_view _name ) const
{
	std::string str{ _name };
	return m_stdFiles.count( str ) > 0;
}

//------------------------------------------------------------------------------

void StdLibrary::init()
{
	initCppLibraties();
	initCLibraries();
}

//------------------------------------------------------------------------------

void StdLibrary::initCppLibraties()
{
	initConceptsLibrary();
	initCoroutinesLibrary();
	initUtilitiesLibrary();
	initDynamicMemoryLibrary();
	initNumberLimitLibrary();
	initErrorHandlingLibrary();
	initStringLibraryLibrary();
	initContainersLibrary();
	initIteratorsLibrary();
	initRangesLibrary();
	initAlgorithmsLibrary();
	initNumericsLibrary();
	initIOLibrary();
	initLocalizationLibrary();
	initRegexLibrary();
	initAtomicLibrary();
	initThreadLibrary();
	initFsLibrary();
	initCCompatibilityLibraries();
	initEmptyCHeaders();
	initMeaninglessCHeaders();
	initUnsupportedCHeaders();
}

//------------------------------------------------------------------------------

void StdLibrary::initConceptsLibrary()
{
	addHeaders( { "concepts" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initCoroutinesLibrary()
{
	addHeaders( { "coroutine" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initUtilitiesLibrary()
{
	addHeaders( {
		"cstdlib",		   "csignal",		   "csetjmp",	  "cstdarg",
		"typeinfo",		   "typeindex",		   "type_traits", "bitset",
		"functional",	   "utility",		   "ctime",		  "chrono",
		"cstddef",		   "initializer_list", "tuple",		  "any",
		"optional",		   "variant",		   "compare",	  "version",
		"source_location",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initDynamicMemoryLibrary()
{
	addHeaders( {
		"new",
		"memory",
		"scoped_allocator",
		"memory_resource",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initNumberLimitLibrary()
{
	addHeaders( {
		"climits",
		"cfloat",
		"cstdint",
		"cinttypes",
		"limits",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initErrorHandlingLibrary()
{
	addHeaders( {
		"exception",
		"stdexcept",
		"cassert",
		"system_error",
		"cerrno",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initStringLibraryLibrary()
{
	addHeaders( {
		"cctype",
		"cwctype",
		"cstring",
		"cwchar",
		"cuchar",
		"string",
		"string_view",
		"charconv",
		"format",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initContainersLibrary()
{
	addHeaders( {
		"array",
		"vector",
		"deque",
		"list",
		"forward_list",
		"set",
		"map",
		"unordered_set",
		"unordered_map",
		"stack",
		"queue",
		"span",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initIteratorsLibrary()
{
	addHeaders( { "iterator" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initRangesLibrary()
{
	addHeaders( { "ranges" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initAlgorithmsLibrary()
{
	addHeaders( { "algorithm", "execution" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initNumericsLibrary()
{
	addHeaders( {
		"cmath",
		"complex",
		"valarray",
		"random",
		"numeric",
		"ratio",
		"cfenv",
		"bit",
		"numbers",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initIOLibrary()
{
	addHeaders( {
		"iosfwd",
		"ios",
		"istream",
		"ostream",
		"iostream",
		"fstream",
		"sstream",
		"syncstream",
		"strstream",
		"iomanip",
		"streambuf",
		"cstdio",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initLocalizationLibrary()
{
	addHeaders( {
		"locale",
		"clocale",
		"codecvt",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initRegexLibrary()
{
	addHeaders( { "regex" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initAtomicLibrary()
{
	addHeaders( { "atomic" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initThreadLibrary()
{
	addHeaders( {
		"thread",
		"stop_token",
		"mutex",
		"shared_mutex",
		"future",
		"condition_variable",
		"semaphore",
		"latch",
		"barrier",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initFsLibrary()
{
	addHeaders( { "filesystem" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initCCompatibilityLibraries()
{
	addHeaders( { "assert.h",	"ctype.h",	"errno.h",	"fenv.h",	"float.h",
				  "inttypes.h", "limits.h", "locale.h", "math.h",	"setjmp.h",
				  "signal.h",	"stdarg.h", "stddef.h", "stdint.h", "stdio.h",
				  "stdlib.h",	"string.h", "time.h",	"uchar.h",	"wchar.h",
				  "wctype.h" } );
}

//------------------------------------------------------------------------------

void StdLibrary::initEmptyCHeaders()
{
	addHeaders( {
		"ccomplex",
		"complex.h",
		"ctgmath",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initMeaninglessCHeaders()
{
	addHeaders( {
		"ciso646",
		"iso646.h",
		"cstdalign",
		"cstdbool",
		"stdbool.h",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initUnsupportedCHeaders()
{
	addHeaders( {
		"stdatomic.h",
		"stdnoreturn.h",
		"threads.h",
	} );
}

//------------------------------------------------------------------------------

void StdLibrary::initCLibraries()
{
	addHeaders( { "stdalign.h", "tgmath.h" } );
}

//------------------------------------------------------------------------------

void StdLibrary::addHeaders( const Headers & _headers )
{
	for( const std::string & name: _headers )
	{
		if( auto pair = m_stdFiles.insert( name ); !pair.second )
		{
			INTERNAL_CHECK_WARRING( false );
		}
	}
}

//------------------------------------------------------------------------------

}
