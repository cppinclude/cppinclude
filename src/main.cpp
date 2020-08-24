#include "application/app_con.hpp"

#include <exception>
#include <iostream>

//------------------------------------------------------------------------------

int main(int _argc, char * _argv[])
{
	try
	{
		application::ConcoleApplication app;
		return app.run( _argc, _argv );
	}
	catch( const std::exception & _exc )
	{
		std::cout << _exc.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch( ... )
	{
		std::cout << "Something wrong happens" << std::endl;
		return EXIT_FAILURE;
	}

}

//------------------------------------------------------------------------------

