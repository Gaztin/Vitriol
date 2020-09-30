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

using namespace Vitriol;

HttpServer::HttpServer( uint16_t port )
	: socket_( AddressFamily::Inet, SocketType::Stream, Protocol::TCP )
{
	// Bind and listen on socket
	socket_.Bind( port );
	socket_.Listen();
}

void HttpServer::StartThreads( size_t thread_count )
{
	for( size_t i = 0; i < thread_count; ++i )
	{
		threads_.emplace_back( &HttpServer::ThreadEntry, this );
	}
}

bool HttpServer::IsRunning( void ) const
{
	for( const std::thread& thread : threads_ )
	{
		if( thread.joinable() )
			return true;
	}

	return false;
}

void HttpServer::ThreadEntry( void )
{
	while( auto connection = socket_.Accept() )
	{
		const std::string address_string = connection->GetAddressString();

		std::cout << "Connection: " << address_string << "\n";

//////////////////////////////////////////////////////////////////////////

		constexpr size_t buf_size = 512;
		char             buf[ buf_size ];
		size_t           bytes_read;

		while( ( bytes_read = connection->Receive( buf, buf_size ) ) > 0 )
		{
			const std::string buf_string( buf, bytes_read );

			std::cout << buf_string;
		}

		std::cout << "Closed: " << address_string << "\n";
	}

	std::cout << "Done\n";
}
