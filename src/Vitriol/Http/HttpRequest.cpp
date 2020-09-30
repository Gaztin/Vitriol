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

using namespace Vitriol;

HttpRequest::HttpRequest( HttpRequest&& other )
{
	*this = std::move( other );
}

HttpRequest::HttpRequest( HttpMethod method, HttpVersion version, std::string endpoint )
	: method_  ( method )
	, version_ ( version )
	, endpoint_( std::move( endpoint ) )
{
}

HttpRequest& HttpRequest::operator=( HttpRequest&& other )
{
	method_        = other.method_;
	version_       = other.version_;
	endpoint_      = std::move( other.endpoint_ );
	header_fields_ = std::move( other.header_fields_ );

	other.method_  = HttpMethod::None;
	other.version_ = HttpVersion::None;

	return *this;
}

void HttpRequest::SetPayload( std::string payload )
{
	payload_ = std::move( payload );
}

void HttpRequest::AddHeaderField( std::string key, std::string value )
{
	header_fields_.try_emplace( std::move( key ), std::move( value ) );
}
