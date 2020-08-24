#include "json/impl/json_array_impl.hpp"

#include "json/impl/json_value_impl.hpp"

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

JsonArrayImpl::JsonArrayImpl( JsonImpl && _jsonImpl )
	:	m_jsonImpl( std::move( _jsonImpl ) )
{
}

//------------------------------------------------------------------------------

JsonArrayImpl::ArrayIndex JsonArrayImpl::getSize() const
{
	return m_jsonImpl.size();
}

//------------------------------------------------------------------------------

bool JsonArrayImpl::empty() const
{
	return m_jsonImpl.empty();
}

//------------------------------------------------------------------------------

JsonArrayImpl::JsonValuePtr JsonArrayImpl::at( ArrayIndex _index ) const
{
	JsonImpl json = m_jsonImpl[ _index ];
	return JsonValuePtr{ new JsonValueImpl{ std::move( json ) } };
}

//------------------------------------------------------------------------------

}
