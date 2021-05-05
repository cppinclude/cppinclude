#pragma once

#include <stdfwd/size_t>
#include <stdfwd/string_view>

//------------------------------------------------------------------------------

namespace compilation_db {
	class CommandObject;

//------------------------------------------------------------------------------

class Database
{
public:
	using IndexType = std::size_t;

	virtual ~Database() = default;

	virtual IndexType getCount() const = 0;
	virtual const CommandObject & getObject( IndexType _index ) const = 0;

	virtual void addCommand(
		std::string_view _directory,
		std::string_view _command,
		std::string_view _file
	) = 0;

};

//------------------------------------------------------------------------------

}
