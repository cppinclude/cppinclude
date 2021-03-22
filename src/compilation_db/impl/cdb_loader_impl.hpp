#pragma once

#include "compilation_db/api/cdb_loader.hpp"

//------------------------------------------------------------------------------

namespace json {
	class JsonValue;
}

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

class LoaderImpl final : public Loader
{
public:
	DatabasePtr load( const json::JsonObject & _json ) override;
	DatabasePtr createEmptyDb() override;

private:
	void loadItem( const json::JsonValue & _value, Database & _db );
};

//------------------------------------------------------------------------------

}
