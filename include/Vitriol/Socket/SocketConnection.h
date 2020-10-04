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

#include "Vitriol/Vitriol.h"

#include <string>

namespace Vitriol
{
	class SocketConnection
	{
	public:

		 SocketConnection( const SocketConnection& ) = delete;
		 SocketConnection( SocketConnection&& other );
		 SocketConnection( native_socket_t socket, const sockaddr_storage& address, int address_size );
		~SocketConnection( void );

		SocketConnection& operator=( const SocketConnection& ) = delete;
		SocketConnection& operator=( SocketConnection&& other );

	public:

		void        SetBlocking      ( bool blocking );
		size_t      Receive          ( char* buf, size_t buf_size ) const;
		bool        Send             ( const char* buf, size_t buf_size ) const;
		std::string GetAddressString ( void ) const;

	private:

		native_socket_t  native_socket_;
		sockaddr_storage address_;
		int              address_size_;

	};
}
