#include "json/impl/json_object_impl.hpp"

#include "json/impl/json_value_impl.hpp"

#include <string>

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

JsonObjectImpl::JsonObjectPtr JsonObjectImpl::createJson(
	std::istream & _stream
)
{
	JsonImpl json;
	_stream >> json;

	return JsonObjectPtr{ new JsonObjectImpl{ std::move( json ) } };
}

//------------------------------------------------------------------------------

JsonObjectImpl::JsonObjectImpl( JsonImpl && _jsonImpl )
	:	m_jsonImpl( std::move( _jsonImpl ))
{
}

//------------------------------------------------------------------------------

JsonObjectImpl::JsonValuePtr JsonObjectImpl::getAttributeValue(
	std::string_view _name
) const
{
	if( m_jsonImpl.contains( _name ) )
	{
		JsonImpl json = m_jsonImpl.at( _name.data() );
		return JsonValuePtr{ new JsonValueImpl{ std::move( json ) } };
	}
	else
	{
		return nullptr;
	}
}

//------------------------------------------------------------------------------

}
