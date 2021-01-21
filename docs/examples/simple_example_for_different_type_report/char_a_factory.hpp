#pragma once

#include <base_char_factory.hpp>

class CharAFactory : public BaseCharFactory
{
public:

	std::unique_ptr< BaseChar > createObject() override;
};
