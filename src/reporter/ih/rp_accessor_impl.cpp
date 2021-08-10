#include "reporter/ih/rp_accessor_impl.hpp"

#include "reporter/impl/rp_factpory_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

ReporterAccessorImpl::ReporterAccessorImpl()
	: m_factory{ new FactoryImpl }
{
}

//------------------------------------------------------------------------------

ReporterAccessorImpl::~ReporterAccessorImpl() = default;

//------------------------------------------------------------------------------

Factory & ReporterAccessorImpl::getReporterFactory()
{
	INTERNAL_CHECK_ERROR( m_factory );
	return *m_factory;
}

//------------------------------------------------------------------------------

}
