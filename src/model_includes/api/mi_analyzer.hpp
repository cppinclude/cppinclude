#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace project {
	class Project;
}

//------------------------------------------------------------------------------

namespace model_includes {
	class Model;

//------------------------------------------------------------------------------

class Analyzer
{
public:

	using ModelPtr	= stdfwd::unique_ptr< Model >;
	using Project	= project::Project;

	virtual ~Analyzer() = default;

	virtual ModelPtr analyze( const Project & _project ) const = 0;
};

//------------------------------------------------------------------------------

}
