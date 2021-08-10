#include "parser/impl/pr_parser_impl.hpp"

#include "parser/api/pr_include_file.hpp"
#include "parser/impl/pr_parser_context.hpp"

#include "fs/api/fs_file.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <optional>
#include <vector>

//------------------------------------------------------------------------------

namespace parser
{
//------------------------------------------------------------------------------

Parser::IncludeFiles ParserImpl::parseFile( const fs::File & _file ) const
{
	IncludeFiles result;
	ParserContext context;

	while( !_file.eof() )
	{
		const std::string line = _file.getLine();
		context.setCurrentLine( line );
		auto includeFileOpt = parseLine( context );
		if( includeFileOpt )
		{
			result.push_back( *includeFileOpt );
		}
		context.increaseLineNumber();
	}

	return result;
}

//------------------------------------------------------------------------------

ParserImpl::IncludeFileOpt ParserImpl::parseLine( ParserContext & _context )
{
	constexpr size_t minimumSize = std::string_view{ "/*" }.size() - 1;

	IncludeFileOpt resutl;

	const std::string & line = _context.getCurrentLine();
	const size_t size = line.size();
	if( size <= minimumSize )
	{
		return resutl;
	}

	const size_t startPos = getStartPos( _context );

	for( size_t i = startPos; i < size; ++i )
	{
		const char currentChar = line[i];
		switch( currentChar )
		{
			case '/':
			{
				i = findComentEnd( _context, i );
				break;
			}

			case '"':
			{
				i = findEndOfString( _context, i );
				break;
			}

			case 'R':
			{
				i = findEndOfRawString( _context, i );
				break;
			}

			case '#':
			{
				if( auto indexOpt = findInclude( line, i ); indexOpt )
				{
					return parseInclude( _context, *indexOpt );
				}

				// if found # and it's not include it doesn't need analyze
				return resutl;

				break;
			}
			default:
				break;
		}
	}

	return resutl;
}

//------------------------------------------------------------------------------

std::size_t ParserImpl::getStartPos( ParserContext & _context ) noexcept
{
	const std::size_t index = 0;

	if( _context.isEnableMultilineComment() )
	{
		return findComentEnd( _context, index );
	}

	if( _context.isEnableMultilineString() )
	{
		return findEndOfString( _context, index );
	}

	if( _context.isEnableRawString() )
	{
		return findEndOfRawString( _context, index );
	}

	return 0;
}

//------------------------------------------------------------------------------

std::size_t ParserImpl::getStartPosInInclude(
	const ParserContext & _conext, std::size_t _index, char _char )
{
	const std::string & line = _conext.getCurrentLine();
	const std::size_t count = line.size();
	for( std::size_t i = _index; i < count; ++i )
	{
		const char currentChar = line[i];
		if( currentChar == _char )
		{
			return i;
		}

		if( currentChar == '/' )
		{
			const std::size_t nextPos = i + 1;
			if( nextPos >= count )
			{
				continue;
			}

			const char nextChar = line[nextPos];
			if( nextChar == '/' )
			{
				return std::string::npos;
			}

			if( nextChar == '*' )
			{
				const auto endPos = line.find( "*/", nextPos );
				if( endPos == std::string::npos )
				{
					return std::string::npos;
				}
				i = endPos + 1;
			}
		}
	}
	return std::string::npos;
}

//------------------------------------------------------------------------------

std::size_t ParserImpl::findComentEnd(
	ParserContext & _context, std::size_t _index ) noexcept
{
	const size_t nextIndex =
		_context.isEnableMultilineComment() ? 0 : _index + 1;
	const std::string & line = _context.getCurrentLine();
	const size_t size = line.size();
	if( nextIndex >= size )
	{
		return size;
	}

	const char nextChar = line[nextIndex];

	if( !_context.isEnableMultilineComment() )
	{
		if( nextChar == '/' )
		{
			return size;
		}

		if( nextChar != '*' )
		{
			return _index;
		}

		assert( nextChar == '*' );
	}

	const auto pos = line.find( "*/", nextIndex );
	_context.setMultilineComment( pos == std::string::npos );

	return _context.isEnableMultilineComment() ? size : pos + 1;
}

//------------------------------------------------------------------------------

std::size_t ParserImpl::findEndOfString(
	ParserContext & _context, std::size_t _index ) noexcept
{
	const std::string & line = _context.getCurrentLine();
	const size_t size = line.size();
	for( size_t i = _index + 1; i < size; ++i )
	{
		const char currentChar = line[i];
		switch( currentChar )
		{
			case '\"':
			{
				_context.setMultilineString( false );
				return i;
			}

			case '\\':
			{
				if( i == size - 1 )
				{
					_context.setMultilineString( true );
					return size;
				}

				const size_t nextPost = i + 1;
				assert( nextPost < size );
				if( nextPost < size && line[nextPost] == '\"' )
				{
					i = nextPost;
				}

				break;
			}
			default:
				break;
		}
	}

	return size;
}

//------------------------------------------------------------------------------

std::size_t ParserImpl::findEndOfRawString(
	ParserContext & _context, std::size_t _index ) noexcept
{
	const std::string & line = _context.getCurrentLine();

	if( !_context.isEnableRawString() )
	{
		const auto pos = line.find( "\"(", _index + 1 );
		if( pos == std::string::npos )
		{
			return _index;
		}

		_index += 3;
		_context.seEnableRawString( true );
	}

	const auto pos = line.find( ")\"", _index );
	if( pos != std::string::npos )
	{
		_context.seEnableRawString( false );
		return pos + 2;
	}

	return line.size();
}

//------------------------------------------------------------------------------

std::optional< std::size_t >
ParserImpl::findInclude( std::string_view _line, std::size_t _index )
{
	static const std::string includePhase{ "include" };
	static const size_t includePhaseSize = includePhase.size();

	const size_t size = _line.size();
	bool isStartedCheckPhase = false;
	size_t indexPhase = 0;

	for( size_t i = _index + 1; i < size; ++i )
	{
		const char currentChar = _line[i];
		if( currentChar == ' ' || currentChar == '\t' )
		{
			if( isStartedCheckPhase )
			{
				return std::nullopt;
			}
		}
		else
		{
			isStartedCheckPhase = true;
			const char includeChar = includePhase[indexPhase];
			if( currentChar == includeChar )
			{
				++indexPhase;
				if( indexPhase >= includePhaseSize )
				{
					return i;
				}
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

ParserImpl::IncludeFileOpt
ParserImpl::parseInclude( const ParserContext & _context, std::size_t _index )
{
	const std::string & line = _context.getCurrentLine();
	const size_t startPosSystem = getStartPosInInclude( _context, _index, '<' );
	const size_t startPosUser = getStartPosInInclude( _context, _index, '"' );

	if( startPosSystem == std::string::npos &&
		startPosUser == std::string::npos )
	{
		// it's strange that after #include don't exist < or "
		return std::nullopt;
	}

	const size_t startPos = std::min( startPosSystem, startPosUser );

	const bool isSystem =
		startPosSystem != std::string::npos && startPosSystem == startPos;
	const size_t startPosName =
		( isSystem ? startPosSystem : startPosUser ) + 1;

	const char endChar = isSystem ? '>' : '"';

	size_t endPosName = line.find( endChar, startPosName );
	if( endPosName == std::string::npos )
	{
		// it's strange, include isn't closed
		return std::nullopt;
	}

	const std::string name =
		line.substr( startPosName, endPosName - startPosName );

	IncludeFileLocation location{
		_context.getLineNumber(), startPosName + 1, endPosName + 1 };
	return IncludeFile{ location, name, isSystem };
}

//------------------------------------------------------------------------------

}
