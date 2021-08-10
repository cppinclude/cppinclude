#pragma once

#include "cmake_project/ih/cprj_accessor.hpp"

//------------------------------------------------------------------------------

namespace cmake_project
{
//------------------------------------------------------------------------------

class AccessorImpl final : public Accessor
{
public:
	LoaderPtr createLoader() override;
};

//------------------------------------------------------------------------------

}
