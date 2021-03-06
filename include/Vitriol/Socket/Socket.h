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
#include "Vitriol/Vitriol.h"

#include <optional>
#include <string_view>

namespace Vitriol
{
	class Socket
	{
	public:

		 Socket( const Socket& ) = delete;
		 Socket( Socket&& other );
		 Socket( AddressFamily address_family, SocketType socket_type, Protocol protocol );
		~Socket( void );

		Socket& operator=( const Socket& ) = delete;
		Socket& operator=( Socket&& other );

	public:

		bool                              Bind        ( uint16_t port );
		bool                              Listen      ( void );
		std::optional< SocketConnection > Accept      ( void );
		void                              SetBlocking ( bool blocking );

	public:

		bool IsValid( void ) const { return native_handle_ != invalid_native_socket_v; }

	private:

		AddressFamily   address_family_ = AddressFamily::None;
		SocketType      socket_type_    = SocketType::None;
		Protocol        protocol_       = Protocol::None;
		native_socket_t native_handle_  = invalid_native_socket_v;
		addrinfo*       address_info_   = nullptr;

	};
}
