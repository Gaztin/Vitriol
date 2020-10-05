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

namespace Vitriol
{
	class HttpResponse
	{
	public:

		HttpResponse( const HttpResponse& other ) = delete;
		HttpResponse( HttpResponse&& );
		HttpResponse( int code, std::string body = std::string(), std::string content_type = "text/plain" );

		HttpResponse& operator=( const HttpResponse& ) = delete;
		HttpResponse& operator=( HttpResponse&& other );

	public:

		std::string      GenerateData   ( HttpVersion version ) const;
		std::string_view GetHeaderField ( std::string_view key ) const;
		void             SetHeaderField ( std::string_view key, std::string value );

	public:

		std::string_view GetBody( void ) const { return body_; }

	private:

		std::string  body_;
		std::string  content_type_;

		HttpFieldMap header_fields_;

		int          code_;

	};
}
