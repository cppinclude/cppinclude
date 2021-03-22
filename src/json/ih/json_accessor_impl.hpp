#include "json/ih/json_accessor.hpp"

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

class JsonAccesorImpl : public JsonAccessor
{
public:
	JsonAccesorImpl();
	~JsonAccesorImpl() override;

	JsonObjectPtr createJson( std::istream & _stream ) override;
};

//------------------------------------------------------------------------------

}
