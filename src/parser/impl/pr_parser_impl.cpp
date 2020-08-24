#include "parser/impl/pr_parser_impl.hpp"

#include "parser/api/pr_include_file.hpp"

#include "fs/api/fs_file.hpp"

#include <fstream>
#include <vector>
#include <optional>

//------------------------------------------------------------------------------

namespace parser {

//------------------------------------------------------------------------------

Parser::IncludeFiles ParserImpl::parseFile( const fs::File & _file ) const
{
	IncludeFiles result;
	std::size_t lineNumber = 1;

	while( !_file.eof() )
	{
		std::string line = _file.getLine();
		if( auto includeFileOpt = parseLine( line, lineNumber ); includeFileOpt )
		{
			result.push_back( *includeFileOpt );
		}
		++lineNumber;
	}
	return result;
}

//------------------------------------------------------------------------------

ParserImpl::IncludeFileOpt ParserImpl::parseLine(
	std::string_view _line,
	std::size_t _lineNumber
) const
{
	static constexpr size_t minimumSize = std::string_view{"#include<>"}.size();

	IncludeFileOpt resutl;

	const size_t size = _line.size();
	if( size <= minimumSize )
		return resutl;

	for( size_t i = 0; i < size; ++i )
	{
		const char str_char = _line[i];
		switch( str_char )
		{
			case '/' :
				i = findComentEnd( _line, i );
				break;

			case '\"':
				i = findEndOfString( _line, i );
				break;

			case '#' :
			if( auto indexOpt = findInclude( _line, i ); indexOpt )
				return parseInclude( _line, _lineNumber, *indexOpt );
			else // if found # and it's not include it doesn't need analyze
				return resutl;
		}
	}

	return resutl;
}

//------------------------------------------------------------------------------

std::size_t ParserImpl::findComentEnd(
	std::string_view _line,
	std::size_t _index
) noexcept
{
	const size_t nextIndex = _index + 1;
	const size_t size = _line.size();
	if( nextIndex >= size )
		return size;

	const char nextChar = _line[ nextIndex ];

	if( nextChar == '/' )
		return size;

	if( nextChar != '*' )
		return _index;

	const auto pos = _line.find( "*/", nextIndex );
	return ( pos == std::string::npos ? size : pos + 1 );
}

//------------------------------------------------------------------------------

std::size_t ParserImpl::findEndOfString(
	std::string_view _line,
	std::size_t _index
) noexcept
{
	const size_t size = _line.size();
	for( size_t i = _index + 1 ; i < size ; ++i )
	{
		if( _line[i] == '\"' )
			return  i;
	}

	return size;
}

//------------------------------------------------------------------------------

std::optional< std::size_t > ParserImpl::findInclude(
	std::string_view _line,
	std::size_t _index
)
{
	static const std::string includePhase{ "include" };
	static const size_t includePhaseSize = includePhase.size();

	const size_t size = _line.size();
	bool isStartedCheckPhase = false;
	size_t indexPhase = 0;

	for( size_t i = _index + 1; i < size; ++i )
	{
		const char str_char = _line[ i ];
		if( str_char == ' ' || str_char == '\t' )
		{
			if( isStartedCheckPhase )
				return std::nullopt;
		}
		else
		{
			isStartedCheckPhase = true;
			const char includeChar = includePhase[indexPhase];
			if( str_char == includeChar )
			{
				++indexPhase;
				if( indexPhase >= includePhaseSize )
					return i;
			}
			else
			{
				return std::nullopt;
			}
		}
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

ParserImpl::IncludeFileOpt ParserImpl::parseInclude(
	std::string_view _line,
	std::size_t _lineNumber,
	std::size_t _index
)
{
	const size_t startPosSystem = _line.find( '<', _index );
	const size_t startPosUser	= _line.find( '"', _index );

	if( startPosSystem == std::string::npos && startPosUser == std::string::npos )
	{
		// it's strange that after #include don't exist < or "
		return std::nullopt;
	}

	const bool isSystem = startPosSystem != std::string::npos;
	const size_t startPosName = ( isSystem ? startPosSystem : startPosUser ) + 1;

	const char endChar = isSystem ? '>' : '"';

	size_t endPosName = _line.find( endChar, startPosName );
	if( endPosName == std::string::npos )
	{
		// it's strange, include isn't closed
		return std::nullopt;
	}

	const std::string_view name = _line.substr(
		startPosName,
		endPosName - startPosName
	);

	IncludeFileLocation location{ _lineNumber, startPosName + 1, endPosName + 1 };
	return IncludeFile{ location, name, isSystem };
}

//------------------------------------------------------------------------------

}
