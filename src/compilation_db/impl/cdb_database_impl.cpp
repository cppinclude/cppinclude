#include "compilation_db/impl/cdb_database_impl.hpp"

#include "compilation_db/impl/cdb_command_object_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <string_view>

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

DatabaseImpl::DatabaseImpl() = default;
DatabaseImpl::~DatabaseImpl() = default;

//------------------------------------------------------------------------------

DatabaseImpl::IndexType DatabaseImpl::getCount() const
{
	return m_objects.size();
}

//------------------------------------------------------------------------------

const CommandObject & DatabaseImpl::getObject( IndexType _index ) const
{
	const CommandObjectPtr & objectPtr = m_objects.at( _index );
	INTERNAL_CHECK_ERROR( objectPtr );
	return *objectPtr;
}

//------------------------------------------------------------------------------

void DatabaseImpl::addCommand(
	std::string_view _directory,
	std::string_view _command,
	std::string_view _file
)
{
	m_objects.emplace_back(
		new CommandObjectImpl{ _directory, _command, _file }
	);
}

//------------------------------------------------------------------------------

}
