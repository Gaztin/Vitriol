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

#include "Vitriol/Http/HttpRequest.h"

#include "Vitriol/Http/HttpResponse.h"

#include <iostream>

using namespace Vitriol;

HttpRequest::HttpRequest( HttpRequest&& other )
	: sender_connection_( std::move( other.sender_connection_ ) )
	, method_           ( other.method_ )
	, version_          ( other.version_ )
	, uri_              ( std::move( other.uri_ ) )
	, payload_          ( std::move( other.payload_ ) )
	, header_fields_    ( std::move( other.header_fields_ ) )
{
	other.method_  = HttpMethod::None;
	other.version_ = HttpVersion::None;
}

HttpRequest::HttpRequest( std::reference_wrapper< SocketConnection > sender_connection, HttpMethod method, HttpVersion version, std::string uri )
	: sender_connection_( std::move( sender_connection ) )
	, method_           ( method )
	, version_          ( version )
	, uri_              ( std::move( uri ) )
{
}

HttpRequest& HttpRequest::operator=( HttpRequest&& other )
{
	sender_connection_ = std::move( other.sender_connection_ );
	method_            = other.method_;
	version_           = other.version_;
	uri_               = std::move( other.uri_ );
	header_fields_     = std::move( other.header_fields_ );

	other.method_  = HttpMethod::None;
	other.version_ = HttpVersion::None;

	return *this;
}

void HttpRequest::Respond( const HttpResponse& response )
{
	const std::string data       = response.GenerateData( version_ );
	SocketConnection& connection = sender_connection_;

	connection.Send( data.data(), data.size() );
}

void HttpRequest::SetPayload( std::string payload )
{
	payload_ = std::move( payload );
}

void HttpRequest::AddHeaderField( std::string key, std::string value )
{
	header_fields_.try_emplace( std::move( key ), std::move( value ) );
}

std::string_view HttpRequest::GetPath( void ) const
{
	std::string_view uri_view = uri_;
	size_t           offset   = uri_view.find_first_of( "?#" );

	return uri_view.substr( 0, offset );
}

std::string_view HttpRequest::GetQuery( std::string_view wanted_key ) const
{
	std::string_view uri_view = uri_;

	if( const size_t query_offset = uri_view.find_first_of( '?' ); query_offset != std::string_view::npos )
	{
		for( size_t offset = query_offset; offset != std::string_view::npos; )
		{
			++offset;

			if( size_t equal_offset = uri_view.find_first_of( '=', offset ); equal_offset != std::string_view::npos )
			{
				const size_t           key_length = equal_offset - offset;
				const std::string_view key        = uri_view.substr( offset, key_length );
				offset                            = equal_offset + 1;
				const size_t           and_offset = uri_view.find_first_of( '&', offset );

				if( key == wanted_key )
				{
					const size_t value_offset = equal_offset + 1;

					if( and_offset == std::string_view::npos ) { return uri_view.substr( offset ); }
					else                                       { return uri_view.substr( offset, and_offset - offset ); }
				}

				offset = and_offset;
			}
		}
	}

	return std::string_view();
}
