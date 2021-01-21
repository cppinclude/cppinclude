#include "reporter/impl/different_type_reporter/rp_different_type_reporter_details.hpp"

#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

const DifferentTypeReporterDetails::Includes &
DifferentTypeReporterDetails::getUserIncludes() const
{
	return m_userIncludes;
}

//------------------------------------------------------------------------------

const DifferentTypeReporterDetails::Includes &
DifferentTypeReporterDetails::getSystemIncludes() const
{
	return m_systemIncludes;
}

//------------------------------------------------------------------------------

void DifferentTypeReporterDetails::insertInclude( const Include & _include )
{
	using namespace model_includes;

	static_assert( static_cast< int >( IncludeType::Count ) == 2 );
	const auto type = _include.getType();
	switch( type )
	{
		case IncludeType::User :
			m_userIncludes.insert( _include );
		break;
		case IncludeType::System :
			m_systemIncludes.insert( _include );
		break;

		default:
			INTERNAL_CHECK_WARRING( false );
	}
}

//------------------------------------------------------------------------------

}
