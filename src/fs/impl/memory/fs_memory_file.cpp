#include "fs/impl/memory/fs_memory_file.hpp"

#include <sstream>

//------------------------------------------------------------------------------

namespace fs::memory {

//------------------------------------------------------------------------------

MemoryFile::MemoryFile()
{
	m_buf.unsetf( std::ios_base::skipws );
}

//------------------------------------------------------------------------------

bool MemoryFile::eof() const
{
	return m_buf.eof();
}

//------------------------------------------------------------------------------

std::string MemoryFile::getLine() const
{
	std::string result;
	getline( m_buf, result);
	return result;
}

//------------------------------------------------------------------------------

std::istream & MemoryFile::toInputStream()
{
	return m_buf;
}

//------------------------------------------------------------------------------

File & MemoryFile::operator<<( std::string_view _str )
{
	m_buf << _str;
	return *this;
}

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
