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

#include "Vitriol/Enums.h"

#include <string>
#include <map>

namespace Vitriol
{
	class HttpRequest
	{
	public:

		using FieldMap = std::map< std::string, std::string >;

	public:

		HttpRequest( HttpRequest&& other );
		HttpRequest( const HttpRequest& other ) = default;
		HttpRequest( HttpMethod method, HttpVersion version, std::string endpoint );

		HttpRequest& operator=( const HttpRequest& other ) = default;
		HttpRequest& operator=( HttpRequest&& other );

	public:

		void SetPayload     ( std::string payload );
		void AddHeaderField ( std::string key, std::string value );

	public:

		std::string_view GetEndpoint( void ) const { return endpoint_; }

	private:

		HttpMethod  method_;
		HttpVersion version_;
		std::string endpoint_;
		std::string payload_;
		FieldMap    header_fields_;

	};
}
