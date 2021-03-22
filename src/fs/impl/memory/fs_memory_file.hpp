#pragma once

#include "fs/api/fs_file.hpp"

#include <sstream>

//------------------------------------------------------------------------------

namespace fs::memory {

//------------------------------------------------------------------------------

class MemoryFile final : public File
{
public:
	MemoryFile();

	bool eof() const override;
	std::string getLine() const override;
	std::istream & toInputStream() override;
	File & operator<<( std::string_view _str ) override;

private:
	mutable std::stringstream m_buf;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
