#include "application/tools/app_configuration_file_loader.hpp"

#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"

#include "json/api/json_object.hpp"
#include "json/ih/json_accessor.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "fs/api/fs_file.hpp"
#include "fs/api/fs_file_system.hpp"

#include <memory>
#include <optional>
#include <std_fs>

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

ConfigurationFileLoader::ConfigurationFileLoader(
	JsonAccessor & _jsonAccessor, const fs::FileSystem & _fs )
	: m_jsonAccessor{ _jsonAccessor }
	, m_fs{ _fs }
{
}

//------------------------------------------------------------------------------

ConfigurationFileLoader::ConfigurationFilePtr
ConfigurationFileLoader::load( const ParserArgWrapper & _arguments )
{
	auto configurationPathOpt = _arguments.getConfigurationFile();
	if( !configurationPathOpt )
	{
		configurationPathOpt = _arguments.getDefaultConfigurationFile();
	}

	INTERNAL_CHECK_ERROR( configurationPathOpt );
	if( !m_fs.isExistFile( *configurationPathOpt ) )
	{
		return nullptr;
	}

	auto filePtr = m_fs.openFile( *configurationPathOpt );
	INTERNAL_CHECK_WARRING( filePtr )
	if( filePtr )
	{
		return load( *filePtr );
	}

	return nullptr;
}

//------------------------------------------------------------------------------

ConfigurationFileLoader::ConfigurationFilePtr
ConfigurationFileLoader::load( fs::File & _file )
{
	auto jsonObjectPtr = m_jsonAccessor.createJson( _file.toInputStream() );
	INTERNAL_CHECK_WARRING( jsonObjectPtr )
	if( jsonObjectPtr )
	{
		ConfigurationFilePtr resultPtr{ new ConfigurationFile };
		INTERNAL_CHECK_ERROR( resultPtr )
		resultPtr->loadFromJson( *jsonObjectPtr );
		return resultPtr;
	}

	return nullptr;
}

//------------------------------------------------------------------------------

}
