#include "compilation_db/ih/cdb_accessor_impl.hpp"

#include "compilation_db/impl/cdb_loader_impl.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

AccessorImpl::LoaderPtr AccessorImpl::createLoader()
{
	return LoaderPtr{ new LoaderImpl };
}

//------------------------------------------------------------------------------

}
