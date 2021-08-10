#pragma once

#include "compilation_db/api/cdb_command_object.hpp"

#include <string>

//------------------------------------------------------------------------------

namespace compilation_db
{
//------------------------------------------------------------------------------

class CommandObjectImpl final : public CommandObject
{
public:
	CommandObjectImpl(
		std::string_view _directory,
		std::string_view _command,
		std::string_view _file );

	const std::string & getDirectory() const override;
	const std::string & getCommand() const override;
	const std::string & getFile() const override;

private:
	const std::string m_directory;
	const std::string m_command;
	const std::string m_file;
};

//------------------------------------------------------------------------------

}
