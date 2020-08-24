#pragma once

#include "project/ih/prj_project_accessor.hpp"

//------------------------------------------------------------------------------

namespace project {

//------------------------------------------------------------------------------

class ProjectAccessorImpl final : public ProjectAccessor
{
public:

	ProjectAccessorImpl();
	~ProjectAccessorImpl() override;

	ProjectPtr createProject() override;
};

//------------------------------------------------------------------------------

}
