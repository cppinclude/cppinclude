#pragma once

#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace compilation_db
{
class Loader;

//------------------------------------------------------------------------------

class Accessor
{
public:
	using LoaderPtr = stdfwd::unique_ptr< Loader >;

	virtual ~Accessor() = default;

	virtual LoaderPtr createLoader() = 0;
};

//------------------------------------------------------------------------------

}
