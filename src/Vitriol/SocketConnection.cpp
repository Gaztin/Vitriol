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

#include "Vitriol/SocketConnection.h"

#include "Vitriol/Enums.h"

#if defined( _WIN32 )
  #include <ws2ipdef.h>
  #include <WS2tcpip.h>
#endif // _WIN32

using namespace Vitriol;

SocketConnection::SocketConnection( SocketConnection&& other )
{
	*this = std::move( other );
}

SocketConnection::SocketConnection( native_socket_t socket, const sockaddr_storage& address, int address_size )
	: native_socket_( socket )
	, address_      ( address )
	, address_size_ ( address_size )
{
}

SocketConnection::~SocketConnection( void )
{
	// Close socket connection
	if( native_socket_ != invalid_native_socket_v )
		close( native_socket_ );
}

SocketConnection& SocketConnection::operator=( SocketConnection&& other )
{
	native_socket_ = other.native_socket_;
	memcpy( &address_, &other.address_, other.address_size_ );
	address_size_  = other.address_size_;

	other.native_socket_ = invalid_native_socket_v;
	memset( &other.address_, 0, other.address_size_ );
	other.address_size_  = 0;

	return *this;
}

size_t SocketConnection::Receive( char* buf, size_t buf_size ) const
{
	if( int result = recv( native_socket_, buf, buf_size, 0 ); result > 0 )
	{
		return static_cast< size_t >( result );
	}
	else if( result == 0 )
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

std::string SocketConnection::GetAddressString( void ) const
{
	std::string result;

	const AddressFamily address_family = static_cast< AddressFamily >( address_.ss_family );

	switch( address_family )
	{
		case AddressFamily::Inet:
		{
			constexpr size_t   buflen = INET_ADDRSTRLEN;
			char               buf[ buflen ] { };
			const sockaddr_in* addr   = reinterpret_cast< const sockaddr_in* >( &address_ );

			// Convert address into human-readable dotted-decimal notation
			if( inet_ntop( addr->sin_family, &addr->sin_addr, buf, buflen ) )
				result.assign( buf, strnlen( buf, buflen ) );

		} break;

		case AddressFamily::Inet6:
		{
			constexpr size_t    buflen = INET6_ADDRSTRLEN;
			char                buf[ buflen ] { };
			const sockaddr_in6* addr   = reinterpret_cast< const sockaddr_in6* >( &address_ );

			if( getnameinfo( reinterpret_cast< const sockaddr* >( addr ), sizeof( sockaddr_in6 ), buf, buflen, nullptr, 0, 0 ) == 0 )
				result.assign( buf, strnlen( buf, buflen ) );

		} break;

		default:
			break;
	}

	return result;
}
