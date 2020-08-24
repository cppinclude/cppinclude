#pragma once

#include "test_tools/boost_predicate_wrapper.hpp"

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {
	class Include;
	class File;

	enum class IncludeType;
	enum class IncludeStatus;
}

//------------------------------------------------------------------------------

namespace model_includes::test {

//------------------------------------------------------------------------------

class IncludeWrapper
{
public:

	IncludeWrapper();
	IncludeWrapper( const Include & _include );

	std::string dump() const;
	bool isAvaliable() const;

	BoostPredicate checkSource( std::string_view  _path ) const;
	BoostPredicate checkDestination( std::string_view _path ) const;

	BoostPredicate checkType( IncludeType _exceptType ) const;
	BoostPredicate checkStatus( IncludeStatus _exceptStatus ) const;

private:

	const Include & getInclude() const;

	BoostPredicate checkFile( const File & _file, std::string_view _path ) const;

	std::string toString( IncludeType _type ) const;
	std::string toString( IncludeStatus _status ) const;

private:

	const Include * m_include;
};

//------------------------------------------------------------------------------

}
