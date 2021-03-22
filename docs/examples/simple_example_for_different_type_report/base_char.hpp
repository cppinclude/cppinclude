#pragma once

#include "char_kind.hpp"

class BaseChar
{
public:
	virtual ~BaseChar() = default;

	virtual CharKind getKind() const noexcept = 0;
};
