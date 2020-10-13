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
		std::string_view path = request.GetPath();
		std::string      response_body;

		std::cout << "Request: '" << path << "'\n";

		response_body += "<body>";

		if( path == "/test" )
		{
			response_body += "<h1>Hello, world!</h1>";
			response_body += "<p>Count: " + std::to_string( count_++ ) + "</p>";
		}
		else if( path == "/button" )
		{
			response_body += R"(<form action="/form" method="get" id="button_form">)";
			response_body += R"(<label for="fname">First name:</label>)";
			response_body += R"(<input type="text" id="fname" name="fname"><br><br>)";
			response_body += R"(<label for="lname">Last name:</label>)";
			response_body += R"(<input type="text" id="lname" name="lname">)";
			response_body += R"(</form>)";
			response_body += R"(<button type="submit" form="button_form">Submit</button>)";
		}
		else if( path == "/form" )
		{
			response_body += "<p>";
			response_body += "Welcome, ";
			response_body += request.GetQuery( "fname" );
			response_body += ' ';
			response_body += request.GetQuery( "lname" );
			response_body += "!</p>";
		}
		else
		{
			response_body += "<p>Unhandled endpoint</p>";
		}

		response_body += "</body>";

		request.Respond( Vitriol::HttpResponse( 200, std::move( response_body ), "text/html" ) );
	}

private:

	int count_ = 0;

};

int main( int /*argc*/, char* /*argv*/[] )
{
	Server server;

	// Start maximum amount of threads
	server.StartThreads( std::thread::hardware_concurrency() );

	while( server.IsRunning() )
	{

	#if defined( _WIN32 )
		std::string console_title = "Number of connections: " + std::to_string( server.GetNumConnections() );
		SetConsoleTitleA( console_title.c_str() );
	#endif // _WIN32

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
