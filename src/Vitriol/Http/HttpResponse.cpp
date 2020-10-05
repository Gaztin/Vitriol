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

#include "Vitriol/Http/HttpResponse.h"

#include <ctime>

using namespace Vitriol;

HttpResponse::HttpResponse( HttpResponse&& other )
	: body_         ( std::move( other.body_ ) )
	, content_type_ ( std::move( other.content_type_ ) )
	, header_fields_( std::move( other.header_fields_ ) )
	, code_         ( other.code_ )
{
	other.code_ = 0;
}

HttpResponse::HttpResponse( int code, std::string body, std::string content_type )
	: body_        ( std::move( body ) )
	, content_type_( std::move( content_type ) )
	, code_        ( code )
{
	// Date field
	{
		time_t now       = time( nullptr );
		tm*    t         = localtime( &now );
		char   buf[ 64 ] = { };

		strftime( buf, std::size( buf ), "%a, %d %b %Y %H:%M:%S %Z", t );

		header_fields_[ "Date" ] = std::string( buf, strnlen( buf, std::size( buf ) ) );
	}

	// Server field
	header_fields_[ "Server" ] = "Vitriol";

	if( !body_.empty() )
	{
		// Content-related fields
		header_fields_[ "Content-Length" ] = std::to_string( body_.size() );
		header_fields_[ "Content-Type" ]   = content_type;
	}
}

HttpResponse& HttpResponse::operator=( HttpResponse&& other )
{
	body_          = std::move( other.body_ );
	content_type_  = std::move( other.content_type_ );
	header_fields_ = std::move( other.header_fields_ );
	code_          = other.code_;

	other.code_ = 0;

	return *this;
}

std::string HttpResponse::GenerateData( HttpVersion version ) const
{
	std::string result;

	// Estimate how long the data is going to be
	result.reserve( 256 + body_.size() );

	// First line is "<version> <code> <status>"
	result += HttpVersionToString( version );
	result += ' ';
	result += std::to_string( code_ );
	result += '\n';

	// Remaining lines are header fields
	for( const HttpFieldMap::value_type& field : header_fields_ )
	{
		result += field.first;
		result += ": ";
		result += field.second;
		result += '\n';
	}

	// Append body if present
	if( !body_.empty() )
	{
		result += '\n';
		result += body_;
	}

	return result;
}

std::string_view HttpResponse::GetHeaderField( std::string_view key ) const
{
	auto it = std::find_if( header_fields_.begin(), header_fields_.end(), [ key ]( const HttpFieldMap::value_type& field ) { return field.first == key; } );
	if( it != header_fields_.end() )
		return it->second;

	return std::string_view();
}

void HttpResponse::SetHeaderField( std::string_view key, std::string value )
{
	// Avoid constructing std::string from key if possible
	auto it = std::find_if( header_fields_.begin(), header_fields_.end(), [ key ]( const HttpFieldMap::value_type& field ) { return field.first == key; } );
	if( it != header_fields_.end() ) it->second = std::move( value );
	else                             header_fields_.try_emplace( std::string( key ), std::move( value ) );
}
