#include "fs/impl/physical/fs_physical_file.hpp"

#include <string>
#include <string_view>

//------------------------------------------------------------------------------

namespace fs::physical {

//------------------------------------------------------------------------------

PhysicalFile::PhysicalFile( std::fstream && _file )
	:	m_file{ std::move( _file ) }
{
	m_file.unsetf( std::ios_base::skipws );
}

//------------------------------------------------------------------------------

bool PhysicalFile::eof() const
{
	return m_file.eof();
}

//------------------------------------------------------------------------------

std::string PhysicalFile::getLine() const
{
	std::string line;
    std::getline( m_file, line );
	return line;
}

//------------------------------------------------------------------------------

std::istream & PhysicalFile::toInputStream()
{
	return m_file;
}

//------------------------------------------------------------------------------

File & PhysicalFile::operator<<( std::string_view _str )
{
	m_file << _str;
	return *this;
}

//------------------------------------------------------------------------------

}
