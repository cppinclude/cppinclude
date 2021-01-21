#include "char_b_factory.hpp"

#include "char_b.hpp"

std::unique_ptr< BaseChar > CharBFactory::createObject()
{
	return std::unique_ptr< BaseChar >{ new CharB };
}

