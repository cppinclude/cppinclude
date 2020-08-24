#pragma once

enum class CharKind;

class BaseChar
{
public:

	virtual ~BaseChar() = default;

	virtual CharKind getKind() const noexcept = 0;
};
