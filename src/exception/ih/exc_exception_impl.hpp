#pragma once

#include "exception/api/exc_exception.hpp"

#include <string>

//------------------------------------------------------------------------------

namespace exception
{
//------------------------------------------------------------------------------

template< class _BaseException > class ExceptionImpl : public _BaseException
{
public:
	ExceptionImpl( std::string_view _module, std::string_view _code )
		: m_module{ _module }
		, m_code{ _code }
	{
	}

	std::string getModuleName() const noexcept override
	{
		return this->m_module;
	}

	std::string getCode() const noexcept override
	{
		return this->m_code;
	}

	const char * what() const noexcept override
	{
		if( m_msg.empty() )
		{
			m_msg = this->getModuleName() + '-' + this->getCode() + ": " +
					this->getMessage();
		}

		return m_msg.c_str();
	}

private:
	mutable std::string m_msg;

	const std::string m_module;
	const std::string m_code;
};

//------------------------------------------------------------------------------

}
