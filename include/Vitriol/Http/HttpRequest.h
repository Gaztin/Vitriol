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

#pragma once

#include "Vitriol/Socket/SocketConnection.h"
#include "Vitriol/Enums.h"

#include <string>
#include <map>

namespace Vitriol
{
	class HttpResponse;

	class HttpRequest
	{
	public:

		using FieldMap = std::map< std::string, std::string >;

	public:

		HttpRequest( HttpRequest&& other );
		HttpRequest( const HttpRequest& other ) = default;
		HttpRequest( std::reference_wrapper< SocketConnection > sender_connection, HttpMethod method, HttpVersion version, std::string uri );

		HttpRequest& operator=( const HttpRequest& other ) = default;
		HttpRequest& operator=( HttpRequest&& other );

	public:

		void Respond        ( const HttpResponse& response );
		void SetPayload     ( std::string payload );
		void AddHeaderField ( std::string key, std::string value );

	public:

		std::string_view GetURI   ( void ) const { return uri_; }
		std::string_view GetPath  ( void ) const;
		std::string_view GetQuery ( std::string_view key ) const;

	private:

		std::reference_wrapper< SocketConnection > sender_connection_;
		HttpMethod                                 method_;
		HttpVersion                                version_;
		std::string                                uri_;
		std::string                                payload_;
		FieldMap                                   header_fields_;

	};
}
