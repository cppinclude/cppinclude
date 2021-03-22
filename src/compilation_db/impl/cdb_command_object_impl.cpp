#include "compilation_db/impl/cdb_command_object_impl.hpp"

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

CommandObjectImpl::CommandObjectImpl(
	std::string_view _directory,
	std::string_view _command,
	std::string_view _file
)
	:	m_directory{ _directory }
	,	m_command{ _command }
	,	m_file{ _file }
{
}

//------------------------------------------------------------------------------

const std::string & CommandObjectImpl::getDirectory() const
{
	return m_directory;
}

//------------------------------------------------------------------------------

const std::string & CommandObjectImpl::getCommand() const
{
	return m_command;
}

//------------------------------------------------------------------------------

const std::string & CommandObjectImpl::getFile() const
{
	return m_file;
}

//------------------------------------------------------------------------------

}
