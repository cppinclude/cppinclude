#pragma once

#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace project
{
class Project;
}

namespace cmake_project
{
class Project;
}

//------------------------------------------------------------------------------

namespace model_includes
{
class Model;

//------------------------------------------------------------------------------

class Analyzer
{
public:
	using ModelPtr = stdfwd::unique_ptr< Model >;

	virtual ~Analyzer() = default;

	virtual ModelPtr analyze( const project::Project & _project ) const = 0;
	virtual ModelPtr analyze(
		const project::Project & _project,
		const cmake_project::Project & _cmakeProject ) const = 0;
};

//------------------------------------------------------------------------------

}
