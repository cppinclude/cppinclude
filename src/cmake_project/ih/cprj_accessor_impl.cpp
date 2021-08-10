#include "cmake_project/ih/cprj_accessor_impl.hpp"

#include "cmake_project/impl/cprj_loader_impl.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace cmake_project
{
//------------------------------------------------------------------------------

AccessorImpl::LoaderPtr AccessorImpl::createLoader()
{
	return LoaderPtr{ new LoaderImpl };
}

//------------------------------------------------------------------------------

}
