#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace json {
	class JsonObject;
}

//------------------------------------------------------------------------------

namespace compilation_db {
	class Database;

//------------------------------------------------------------------------------

class Loader
{
public:
	virtual ~Loader() = default;

	using DatabasePtr = stdfwd::unique_ptr< Database >;

	virtual DatabasePtr load( const json::JsonObject & _json ) = 0;
	virtual DatabasePtr createEmptyDb() = 0;
};

//------------------------------------------------------------------------------

}
