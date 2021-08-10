#pragma once

#include "compilation_db/ih/cdb_accessor.hpp"

//------------------------------------------------------------------------------

namespace compilation_db
{
//------------------------------------------------------------------------------

class AccessorImpl final : public Accessor
{
public:
	LoaderPtr createLoader() override;
};

//------------------------------------------------------------------------------

}
