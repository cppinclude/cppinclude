#include "json/impl/json_value_impl.hpp"

#include "json/impl/json_array_impl.hpp"
#include "json/impl/json_object_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

JsonValueImpl::JsonValueImpl( JsonImpl && _jsonImpl )
	:	m_jsonImpl( std::move( _jsonImpl ) )
{
}

//------------------------------------------------------------------------------

std::string JsonValueImpl::asString() const
{
	INTERNAL_CHECK_WARRING( m_jsonImpl.is_string() );
	return m_jsonImpl.get< std::string >();
}

//------------------------------------------------------------------------------

int JsonValueImpl::asInt() const
{
	INTERNAL_CHECK_WARRING( m_jsonImpl.is_number() );
	return m_jsonImpl.get< int >();
}

//------------------------------------------------------------------------------

bool JsonValueImpl::asBool() const
{
	INTERNAL_CHECK_WARRING( m_jsonImpl.is_boolean() );
	return m_jsonImpl.get< bool >();
}

//------------------------------------------------------------------------------

JsonValueImpl::JsonObjectPtr JsonValueImpl::asObject() const
{
	INTERNAL_CHECK_WARRING( m_jsonImpl.is_object() );
	JsonImpl t( m_jsonImpl );
	INTERNAL_CHECK_WARRING( t.is_object() );

	return JsonObjectPtr{ new JsonObjectImpl{ std::move( t ) } };
}

//------------------------------------------------------------------------------

JsonValueImpl::JsonArrayPtr JsonValueImpl::asArray() const
{
	INTERNAL_CHECK_WARRING( m_jsonImpl.is_array() );
	JsonImpl t( m_jsonImpl );
	INTERNAL_CHECK_WARRING( t.is_array() );

	return JsonArrayPtr{ new JsonArrayImpl{ std::move( t ) } };
}

//------------------------------------------------------------------------------

}
