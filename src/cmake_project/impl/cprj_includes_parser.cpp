#include "cmake_project/impl/cprj_includes_parser.hpp"

#include "cmake_project/impl/cprj_includes_parser_context.hpp"

#include <std_fs>
#include <string_view>
#include <vector>

//------------------------------------------------------------------------------

namespace cmake_project
{
//------------------------------------------------------------------------------

IncludesParser::Includes IncludesParser::parse( std::string_view _command )
{
	IncludeParserContext context{ _command };

	const IndexType size = context.getCommandSize();
	const std::string & command = context.getCommand();
	for( IndexType i = 0; i < size; ++i )
	{
		const char currentChar = command.at( i );
		switch( currentChar )
		{
			case '\\':
			{
				if( getIgnoreSpace( context, i ) )
				{
					const IndexType nextPostWithSpace =
						i + getSizeSpecialChar();
					i = getFinishIgnoreSpace( context, nextPostWithSpace );
				}
			}
			break;
			case ' ':
			{
				i = getNotSpacePos( context, i );
				i = parseArgument( context, i );
			}
			break;
			default:
				break;
		}
	}
	return context.getIncludes();
}

//------------------------------------------------------------------------------

IncludesParser::IndexType IncludesParser::parseArgument(
	IncludeParserContext & _context, IndexType startPos )
{
	const IndexType minusPos = startPos;
	const IndexType prefixPos = minusPos + 1;
	const IndexType size = _context.getCommandSize();
	const std::string & command = _context.getCommand();
	if( minusPos < size && prefixPos < size )
	{
		const char minusChar = command.at( minusPos );
		const char prefixChar = command.at( prefixPos );
		IndexType result;
		if( minusChar == '-' && prefixChar == 'I' )
		{
			result = parseInclude( _context, prefixPos + 1 );
		}
		else
		{
			result = prefixPos + 1;
		}
		return result;
	}

	return size;
}

//------------------------------------------------------------------------------

IncludesParser::IndexType IncludesParser::parseInclude(
	IncludeParserContext & _context, IndexType startPos )
{
	const IndexType size = _context.getCommandSize();
	const std::string & command = _context.getCommand();

	std::string includeStr;
	IndexType finishPos = size;
	bool stopLoop = false;
	for( IndexType i = startPos; i < size && !stopLoop; ++i )
	{
		const char currentChar = command.at( i );
		switch( currentChar )
		{
			case '\\':
			{
				if( getIgnoreSpace( _context, i ) )
				{
					const IndexType nextPostWithSpace =
						i + getSizeSpecialChar();
					i = getFinishIgnoreSpace( _context, nextPostWithSpace );
					if( i < size )
					{
						const IndexType sizeInclude =
							i - nextPostWithSpace + 1 - getSizeSpecialChar();
						const std::string include =
							command.substr( nextPostWithSpace, sizeInclude );
						includeStr += include;
					}
				}
			}
			break;
			case ' ':
			{
				--i;
				stopLoop = true;
			}
			break;
			default:
				includeStr += currentChar;
		}

		finishPos = i;
	}

	if( !includeStr.empty() )
	{
		_context.addInclude( includeStr );
	}

	return finishPos;
}

//------------------------------------------------------------------------------

bool IncludesParser::getIgnoreSpace(
	IncludeParserContext & _context, IndexType currentPos )
{
	const std::string & command = _context.getCommand();
	const IndexType size = _context.getCommandSize();
	const IndexType nextPos = currentPos + 1;
	if( nextPos >= size )
	{
		return false;
	}

	const char nextChar = command.at( nextPos );
	const bool result = nextChar == '"';

	return result;
}

//------------------------------------------------------------------------------

IncludesParser::IndexType IncludesParser::getFinishIgnoreSpace(
	IncludeParserContext & _context, IndexType startPos )
{
	const std::string & command = _context.getCommand();
	const IndexType size = _context.getCommandSize();

	for( IndexType i = startPos; i < size; ++i )
	{
		const char currentChar = command.at( i );
		if( currentChar == '\\' )
		{
			const IndexType nextPos = i + 1;
			if( nextPos < size )
			{
				const char nextChar = command.at( nextPos );
				if( nextChar == '"' )
				{
					return nextPos;
				}
			}
		}
	}

	return size;
}

//------------------------------------------------------------------------------

IncludesParser::IndexType IncludesParser::getSizeSpecialChar()
{
	return 2;
}

//------------------------------------------------------------------------------

IncludesParser::IndexType IncludesParser::getNotSpacePos(
	IncludeParserContext & _context, IndexType startPos )
{
	const std::string & command = _context.getCommand();
	const IndexType size = _context.getCommandSize();
	for( IndexType i = startPos; i < size; ++i )
	{
		const char currentChar = command.at( i );
		if( currentChar != ' ' )
		{
			return i;
		}
	}

	return size;
}

//------------------------------------------------------------------------------

}
