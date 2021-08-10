#pragma once

#include "reporter/ih/rp_accessor.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

class ReporterAccessorImpl final : public ReporterAccessor
{
public:
	ReporterAccessorImpl();
	~ReporterAccessorImpl() override;

	Factory & getReporterFactory() override;

private:
	std::unique_ptr< Factory > m_factory;
};

//------------------------------------------------------------------------------

}
