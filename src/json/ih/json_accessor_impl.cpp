#include "json/ih/json_accessor_impl.hpp"

#include "json/impl/json_object_impl.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

JsonAccesorImpl::JsonAccesorImpl() = default;
JsonAccesorImpl::~JsonAccesorImpl() = default;

//------------------------------------------------------------------------------

JsonAccesorImpl::JsonObjectPtr JsonAccesorImpl::createJson(
	std::istream & _stream
)
{
	return JsonObjectImpl::createJson( _stream );
}

//------------------------------------------------------------------------------

}
