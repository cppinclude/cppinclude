#pragma once

#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace compilation_db {
	class Database;
}

//------------------------------------------------------------------------------

namespace cmake_project {
	class Project;

//------------------------------------------------------------------------------

class Loader
{
public:
	using ProjectPtr = stdfwd::unique_ptr< Project >;

	virtual ~Loader() = default;

	virtual ProjectPtr load( const compilation_db::Database & _db ) = 0;
	virtual ProjectPtr createEmptyProject() = 0;
};

//------------------------------------------------------------------------------

}
