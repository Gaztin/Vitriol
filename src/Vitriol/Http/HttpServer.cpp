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

#include <cassert>
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

void HttpServer::OnRequest( HttpRequest request )
{
	std::cout << "Request for endpoint: " << request.GetEndpoint() << "\n";
}

void HttpServer::ThreadEntry( void )
{
	std::vector< std::reference_wrapper< SocketConnection > > thread_connections;

	while( socket_.IsValid() )
	{
		while( auto connection = socket_.Accept() )
		{
			std::scoped_lock lock( connections_mutex_ );

			auto& connection_ref = all_connections_.emplace_back( std::move( *connection ) );
			connection_ref.SetBlocking( false );

			// Keep track of connections accepted by this thread
			thread_connections.push_back( std::ref( connection_ref ) );
		}

		for( auto it = thread_connections.begin(); it != thread_connections.end(); )
		{
			constexpr size_t  buf_size         = 512;
			SocketConnection& connection       = *it;
			char              buf[ buf_size ]  = { };
			bool              connection_reset = false;
			std::string       request_string;
			size_t            bytes_read;

			while( ( bytes_read = connection.Receive( buf, buf_size, &connection_reset ) ) > 0 )
			{
				request_string.append( buf, bytes_read );
			}

			if( !request_string.empty() )
			{
				HttpRequest request = ParseRequest( std::ref( connection ), request_string );

				OnRequest( std::move( request ) );
			}

			if( connection_reset )
			{
				std::scoped_lock lock( connections_mutex_);
				auto             all_it = std::find( all_connections_.begin(), all_connections_.end(), connection );

				assert( all_it != all_connections_.end() );

				// Untrack connection
				all_connections_.erase( all_it );
				it = thread_connections.erase( it );
			}
			else
			{
				++it;
			}
		}

		// Keep CPU usage low
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}
}

HttpRequest HttpServer::ParseRequest( std::reference_wrapper< SocketConnection > connection, std::string_view data ) const
{
	size_t           newline_offset = data.find_first_of( '\n' );
	std::string_view line           = data.substr( 0, newline_offset );

	// Handle windows-style line endings
	if( line.back() == '\r' )
		line.remove_suffix( 1 );

	// First line contains method, endpoint and protocol version
	const size_t      first_space  = line.find_first_of( ' ' );
	const size_t      second_space = line.find_first_of( ' ', first_space + 1 );
	const HttpMethod  method       = HttpMethodFromString( line.substr( 0, first_space ) );
	std::string       endpoint     = std::string( line.substr( first_space + 1, second_space - ( first_space + 1 ) ) );
	const HttpVersion version      = HttpVersionFromString( line.substr( second_space + 1 ) );
	HttpRequest       request      = HttpRequest( std::move( connection ), method, version, std::move( endpoint ) );

	// Remaining lines contain metadata such as user agent and content type
	while( !data.empty() )
	{
		data.remove_prefix( newline_offset + 1 );

		newline_offset = data.find_first_of( '\n' );
		line           = data.substr( 0, newline_offset );

		// Handle windows-style line endings
		if( line.back() == '\r' )
			line.remove_suffix( 1 );

		// Empty line means end of metadata
		if( line.empty() )
		{
			data.remove_prefix( newline_offset + 1 );
			break;
		}

//////////////////////////////////////////////////////////////////////////

		// Header fields
		const size_t colon_offset       = line.find_first_of( ':' );
		std::string  key                = std::string( line.substr( 0, colon_offset ) );
		const size_t value_start_offset = line.find_first_not_of( ' ', colon_offset + 1 );
		std::string  value              = std::string( line.substr( value_start_offset ) );

		// Set header field
		request.AddHeaderField( std::move( key ), std::move( value ) );
	}

	// If there is still data left, that means we have a payload as well
	if( !data.empty() )
	{
		std::string payload( data );

		request.SetPayload( std::move( payload ) );
	}

	return request;
}
