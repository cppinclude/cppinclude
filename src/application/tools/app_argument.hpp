#pragma once

#include <string>
#include <optional>
#include <any>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class Argument
{
public:

	using ValueOpt = std::optional< std::any >;

	Argument(
		const std::string & _fullName,
		const std::string &  _description,
		ValueOpt _defaultValue = std::nullopt
	);

	template< class _Value >
	void setValue( const _Value & _value )
	{
		m_value = _value;
	}

	const std::string & getFullName() const;
	const std::string & getDescription() const;

	template< class _Value >
	std::optional< _Value > getValue() const
	{
		return getValue< _Value >( m_value );
	}

	template< class _Value >
	std::optional< _Value > getDefaultValue() const
	{
		return getValue< _Value >( m_defaultValue );
	}

private:

	template< class _Value >
	std::optional< _Value > getValue( const ValueOpt & _valueOpt ) const
	{
		if( _valueOpt )
		{
			return std::any_cast< _Value >( *_valueOpt );
		}
		return std::nullopt;
	}

private:

	std::string m_fullName;
	std::string m_description;
	ValueOpt m_defaultValue;
	ValueOpt m_value;

};

//------------------------------------------------------------------------------


}
