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

#include "Vitriol/Socket/Socket.h"

#include <iostream>
#include <string>

#if defined( _WIN32 )
  #include <WS2tcpip.h>
  #include <WSPiApi.h>
#endif // _WIN32

using namespace Vitriol;

Socket::Socket( Socket&& other )
: address_family_( other.address_family_ )
, socket_type_   ( other.socket_type_ )
, protocol_      ( other.protocol_ )
, native_handle_ ( other.native_handle_ )
, address_info_  ( other.address_info_ )
{
	other.address_family_ = AddressFamily::None;
	other.socket_type_    = SocketType::None;
	other.protocol_       = Protocol::None;
	other.native_handle_  = invalid_native_socket_v;
	other.address_info_   = nullptr;
}

Socket::Socket( AddressFamily address_family, SocketType socket_type, Protocol protocol )
	: address_family_( address_family )
	, socket_type_   ( socket_type )
	, protocol_      ( protocol )
{

#if defined( _WIN32 )
	WSADATA wsa;
	WSAStartup( MAKEWORD( 2, 2 ), &wsa );
#endif // _WIN32

	// Create socket
	if( native_handle_ = socket( +address_family, +socket_type, +protocol ); native_handle_ == invalid_native_socket_v )
	{
		std::cerr << "socket failed (code " << GetLastSocketError() << ")\n";
		return;
	}
}

Socket::~Socket( void )
{
	// Clean up address info
	if( address_info_ )
		freeaddrinfo( address_info_ );

	// Close socket
	if( native_handle_ != invalid_native_socket_v )
		close( native_handle_ );

#if defined( _WIN32 )
	WSACleanup();
#endif // _WIN32

}

Socket& Socket::operator=( Socket&& other )
{
	address_family_ = other.address_family_;
	socket_type_    = other.socket_type_;
	protocol_       = other.protocol_;
	native_handle_  = other.native_handle_;
	address_info_   = other.address_info_;

	other.address_family_ = AddressFamily::None;
	other.socket_type_    = SocketType::None;
	other.protocol_       = Protocol::None;
	other.native_handle_  = invalid_native_socket_v;
	other.address_info_   = nullptr;

	return *this;
}

bool Socket::Bind( uint16_t port )
{
	const std::string port_string = std::to_string( port );

	addrinfo hints { };
	hints.ai_family   = +address_family_;
	hints.ai_socktype = +socket_type_;
	hints.ai_protocol = +protocol_;

	// Get local address info
	if( int result = getaddrinfo( "127.0.0.1", port_string.c_str(), &hints, &address_info_ ); result != 0 )
	{
		std::cerr << "getaddrinfo failed (code " << result << ")\n";
		return false;
	}

	// Bind socket to local address and desired port
	if( bind( native_handle_, address_info_->ai_addr, address_info_->ai_addrlen ) == native_socket_error_v )
	{
		std::cerr << "bind failed (code " << GetLastSocketError() << ")\n";
		return false;
	}

	return true;
}

bool Socket::Listen( void )
{
	// Listen to socket
	if( listen( native_handle_, SOMAXCONN ) == native_socket_error_v )
	{
		std::cerr << "listen failed (code " << GetLastSocketError() << ")\n";
		return false;
	}

	return true;
}

std::optional< SocketConnection > Socket::Accept( void )
{
	sockaddr_storage addr { };
	addr.ss_family = static_cast< uint16_t >( address_family_ );

	int addr_size = sizeof( sockaddr_storage );

	if( native_socket_t socket = accept( native_handle_, reinterpret_cast< sockaddr* >( &addr ), &addr_size ); socket == invalid_native_socket_v )
	{
		std::cerr << "accept failed (code " << GetLastSocketError() << ")\n";
		return std::nullopt;
	}
	else
	{
		return std::make_optional< SocketConnection >( socket, addr, addr_size );
	}
}
