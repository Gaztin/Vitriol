/*
 * Copyright (c) 2020 Sebastian Kylander https://gaztin.com/
 *
 * This software is provided 'as-is', without any express or implied warranty. In no event will
 * the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the
 *    original software. If you use this software in a product, an acknowledgment in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as
 *    being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "Vitriol/Http/HttpServer.h"

#include <iostream>

class Server final : public Vitriol::HttpServer
{
private:

	void OnRequest( Vitriol::HttpRequest request ) override
	{
		if( request.GetEndpoint() == "/test" )
		{
			std::string           body = "Hello, world!";
			Vitriol::HttpResponse response( 200, Vitriol::HttpVersion::V1_1, std::move( body ), "text/plain" );

			request.Respond( response );
		}
	}

};

int main( int /*argc*/, char* /*argv*/[] )
{
	Server server;

	// Start maximum amount of threads
	server.StartThreads( std::thread::hardware_concurrency() );

	while( server.IsRunning() )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

	return 0;
}

#if defined( _WIN32 )

#include <Windows.h>

int WINAPI WinMain( HINSTANCE /*instance*/, HINSTANCE /*prev_instance*/, LPSTR /*cmd_line*/, int /*cmd_show*/ )
{
	return main( __argc, __argv );
}

#endif // _WIN32
