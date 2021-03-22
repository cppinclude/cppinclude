#pragma once

#include "compilation_db/api/cdb_database.hpp"

#include <memory>
#include <vector>

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

class DatabaseImpl final : public Database
{
public:
	DatabaseImpl();
	~DatabaseImpl() override;

	IndexType getCount() const override;
	const CommandObject & getObject( IndexType _index ) const override;

	void addCommand(
		std::string_view _directory,
		std::string_view _command,
		std::string_view _file
	) override;

private:
	using CommandObjectPtr = std::unique_ptr< CommandObject >;

	std::vector< CommandObjectPtr > m_objects;
};

//------------------------------------------------------------------------------

}
