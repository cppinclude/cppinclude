#include "char_a_factory.hpp"

#include "char_a.hpp"

std::unique_ptr< BaseChar > CharAFactory::createObject()
{
	return std::unique_ptr< BaseChar >{ new CharA };
}
