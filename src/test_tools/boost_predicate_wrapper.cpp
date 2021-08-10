#include "test_tools/boost_predicate_wrapper.hpp"

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

BoostPredicateWrapper::BoostPredicateWrapper( bool _result )
	: m_message{ _result ? "" : "error" }
{
}

//------------------------------------------------------------------------------

BoostPredicateWrapper::BoostPredicateWrapper( std::string_view _message )
	: m_message{ _message }
{
}

//------------------------------------------------------------------------------

BoostPredicateWrapper::BoostPredicateWrapper( std::string _message )
	: m_message{ std::move( _message ) }
{
}

//------------------------------------------------------------------------------

BoostPredicateWrapper::operator ::boost::test_tools::assertion_result() const
{
	boost::test_tools::predicate_result result{ m_message.empty() };
	result.message() << m_message;
	return result;
}

//------------------------------------------------------------------------------

}
