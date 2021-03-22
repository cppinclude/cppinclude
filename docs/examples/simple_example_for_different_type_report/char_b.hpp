#pragma once

#include "base_char.hpp"

class CharB : public BaseChar
{
public:
	CharKind getKind() const noexcept override;
};
