#pragma once

#include "base_char.hpp"

class CharA : public BaseChar
{
public:
	CharKind getKind() const noexcept override;
};
