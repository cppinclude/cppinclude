#include "model_includes/test/fixtures/wrappers/mi_test_include_wrapper.hpp"

#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"

#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>

//------------------------------------------------------------------------------

namespace model_includes::test {

//------------------------------------------------------------------------------

IncludeWrapper::IncludeWrapper( const Include & _include )
	:	m_include{ &_include }
{
}

//------------------------------------------------------------------------------

IncludeWrapper::IncludeWrapper()
	:	m_include{ nullptr }
{
}

//------------------------------------------------------------------------------

std::string IncludeWrapper::dump() const
{
	std::stringstream result;
	const Include & include = getInclude();
	result
		<< "source: " << include.getSourceFile().getPath().c_str()
		<< " destination: " << include.getDestinationFile().getPath().c_str()
		<< " type: " << toString( include.getType() )
		<< " status: " << toString( include.getStatus() )
	;
	return result.str();
}

//------------------------------------------------------------------------------

bool IncludeWrapper::isAvaliable() const
{
	return m_include != nullptr;
}

//------------------------------------------------------------------------------

BoostPredicate IncludeWrapper::checkSource( std::string_view _path ) const
{
	return checkFile( getInclude().getSourceFile(), _path );
}

//------------------------------------------------------------------------------

BoostPredicate IncludeWrapper::checkDestination( std::string_view _path ) const
{
	return checkFile( getInclude().getDestinationFile(), _path );
}

//------------------------------------------------------------------------------

BoostPredicate IncludeWrapper::checkType( IncludeType _exceptType ) const
{
	const IncludeType type = getInclude().getType();
	if( type == _exceptType )
	{
		return true;
	}

	std::stringstream stream;
	stream
		<< "current type: "
		<< toString( type )
		<< " but except: "
		<< toString( _exceptType )
		<< "\n"
	;
	return stream.str();
}

//------------------------------------------------------------------------------

BoostPredicate IncludeWrapper::checkStatus( IncludeStatus _exceptStatus ) const
{
	const IncludeStatus status = getInclude().getStatus();
	if( status == _exceptStatus )
	{
		return true;
	}

	std::stringstream stream;
	stream
		<< "current status: "
		<< toString( status )
		<< " but except: "
		<< toString( _exceptStatus )
		<< "\n"
	;
	return stream.str();
}

//------------------------------------------------------------------------------

BoostPredicate IncludeWrapper::checkFile(
	const File & _file,
	std::string_view _path
) const
{
	const Path & filePath = _file.getPath();
	Path excpectedPath{ std::string{ _path } };
	excpectedPath = toLexicallyNormal( excpectedPath );
	if( filePath == excpectedPath )
	{
		return true;
	}

	std::stringstream stream;
	stream << "current file: " << filePath << " but except :" << excpectedPath;
	return stream.str();
}

//------------------------------------------------------------------------------

const Include & IncludeWrapper::getInclude() const
{
	INTERNAL_CHECK_ERROR( m_include );
	return *m_include;
}

//------------------------------------------------------------------------------

std::string IncludeWrapper::toString( IncludeType _type ) const
{
	static_assert( static_cast< int >( IncludeType::Count ) == 2 );
	switch( _type )
	{
		case IncludeType::User:
			return "user";
		case IncludeType::System:
			return "system";
		case IncludeType::Count:
			return "<count>";
	}

	INTERNAL_CHECK_WARRING( false );
	return "";
}

//------------------------------------------------------------------------------

std::string IncludeWrapper::toString( IncludeStatus _status ) const
{
	static_assert( static_cast< int >( IncludeStatus::Count ) == 2 );
	switch( _status )
	{
		case IncludeStatus::Resolved:
			return "resolved";
		case IncludeStatus::Unresolved:
			return "unresolved";
		case IncludeStatus::Count:
			return "<count>";
	}

	INTERNAL_CHECK_WARRING( false );
	return "";
}

//------------------------------------------------------------------------------

IncludeWrapper::Path IncludeWrapper::toLexicallyNormal( const Path & _path )
{
	return stdfs::lexically_normal( _path );
}

//------------------------------------------------------------------------------

}
