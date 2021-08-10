#pragma once

#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace project
{
class Project;

//------------------------------------------------------------------------------

class ProjectAccessor
{
public:
	virtual ~ProjectAccessor() = default;

	using ProjectPtr = stdfwd::unique_ptr< Project >;

	virtual ProjectPtr createProject() = 0;
};

//------------------------------------------------------------------------------

}
