#pragma once

#include "fs/api/fs_file.hpp"

#include <fstream>

//------------------------------------------------------------------------------

namespace fs::physical {

//------------------------------------------------------------------------------

class PhysicalFile final : public File
{
public:
	explicit PhysicalFile( std::fstream && _file );

	bool eof() const override;
	std::string getLine() const override;
	std::istream & toInputStream() override;
	File & operator<<( std::string_view _str ) override;

private:
	mutable std::fstream m_file;
};

//------------------------------------------------------------------------------

}
