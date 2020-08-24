#include "char_a_factory.hpp"
#include "char_b_factory.hpp"
#include "char_kind.hpp"
#include "base_char.hpp"

#include <iostream>
#include <cassert>

std::unique_ptr< BaseChar > createChar( BaseCharFactory & _factory );
std::string enumToString( CharKind _kind );

int main()
{
	CharAFactory factoryA;
	CharBFactory factoryB;

	char c;
	std::cin >> c;

	std::unique_ptr< BaseChar > currentObject;
	if( c == 'a' )
		currentObject = createChar( factoryA );
	else
		currentObject = createChar( factoryB );
	assert( currentObject );

	std::cout << "Enum : " << enumToString( currentObject->getKind() ) << '\n';

	return 0;
}

std::unique_ptr< BaseChar > createChar( BaseCharFactory & _factory )
{
	return _factory.createObject();
}


std::string enumToString( CharKind _kind )
{
	static_assert( static_cast< int >( CharKind::Count )  == 2, "");
	switch( _kind )
	{
		case CharKind::A : return "CharKind::A";
		case CharKind::B : return "CharKind::B";

		case CharKind::Count : return "CharKind::Count";
	}

	assert( false );
	return "";
}
