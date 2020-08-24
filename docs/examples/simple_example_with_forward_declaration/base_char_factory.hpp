#pragma once

#include <memory>

class BaseChar;

class BaseCharFactory
{
public:

	virtual ~BaseCharFactory() = default;

	virtual std::unique_ptr< BaseChar > createObject() = 0;
};
