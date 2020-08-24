#include "project/ih/prj_project_accesso_impl.hpp"

#include "project/impl/prj_project_impl.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace project {

//------------------------------------------------------------------------------

ProjectAccessorImpl::ProjectAccessorImpl() = default;
ProjectAccessorImpl::~ProjectAccessorImpl() = default;

//------------------------------------------------------------------------------

ProjectAccessorImpl::ProjectPtr ProjectAccessorImpl::createProject()
{
	return ProjectPtr{ new ProjectImpl{} };
}

//------------------------------------------------------------------------------

}
