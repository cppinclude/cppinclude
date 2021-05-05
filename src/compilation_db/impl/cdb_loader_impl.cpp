#include "compilation_db/impl/cdb_loader_impl.hpp"

#include "compilation_db/impl/cdb_database_impl.hpp"
#include "compilation_db/resources/cdb_resources.hpp"

#include "json/api/json_array.hpp"
#include "json/api/json_object.hpp"
#include "json/api/json_value.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <string_view>

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

LoaderImpl::DatabasePtr LoaderImpl::load( const json::JsonObject & _json )
{
	DatabasePtr result{ createEmptyDb() };

	auto valueObjectPtr = _json.asValue();
	if( !valueObjectPtr )
	{
		INTERNAL_CHECK_WARRING( false );
		return result;
	}

	auto arrayPtr = valueObjectPtr->asArray();
	if( !arrayPtr )
	{
		INTERNAL_CHECK_WARRING( false );
		return result;
	}

	const json::JsonArray & array = *arrayPtr;
	const json::JsonArray::ArrayIndex size = array.getSize();
	for( json::JsonArray::ArrayIndex i = 0; i < size; ++i )
	{
		auto itemPtr = array.at( i );
		INTERNAL_CHECK_WARRING( itemPtr );
		if( itemPtr )
		{
			loadItem( *itemPtr, *result );
		}
	}

	return result;
}

//------------------------------------------------------------------------------

LoaderImpl::DatabasePtr LoaderImpl::createEmptyDb()
{
	return DatabasePtr{ new DatabaseImpl };
}

//------------------------------------------------------------------------------

void LoaderImpl::loadItem( const json::JsonValue & _value, Database & _db )
{
	auto objectPtr = _value.asObject();
	if( !objectPtr )
	{
		INTERNAL_CHECK_WARRING( false );
		return;
	}

	auto directoryPtr = objectPtr->getAttributeValue( resources::Directory );
	if( !directoryPtr )
	{
		INTERNAL_CHECK_WARRING( false );
		return;
	}

	auto commandPtr = objectPtr->getAttributeValue( resources::Command );
	if( !commandPtr )
	{
		INTERNAL_CHECK_WARRING( false );
		return;
	}

	auto filePtr = objectPtr->getAttributeValue( resources::File );
	if( !filePtr )
	{
		INTERNAL_CHECK_WARRING( false );
		return;
	}

	const std::string directory = directoryPtr->asString();
	const std::string command = commandPtr->asString();
	const std::string file = filePtr->asString();

	_db.addCommand( directory, command, file );
}

//------------------------------------------------------------------------------

}
