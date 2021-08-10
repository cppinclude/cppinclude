#pragma once

#include <string>

#include <boost/test/tools/assertion_result.hpp>

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

class BoostPredicateWrapper
{
public:
	BoostPredicateWrapper( bool _result );
	BoostPredicateWrapper( std::string_view _message );
	BoostPredicateWrapper( std::string _message );

	operator ::boost::test_tools::assertion_result() const;

private:
	std::string m_message;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------

using BoostPredicate = tools::BoostPredicateWrapper;
