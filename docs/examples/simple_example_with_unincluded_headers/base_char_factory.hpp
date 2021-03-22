#pragma once

#include "base_char.hpp"

#include <memory>

class BaseCharFactory
{
public:
	virtual ~BaseCharFactory() = default;

	virtual std::unique_ptr< BaseChar > createObject() = 0;
};
