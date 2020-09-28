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

#include "Vitriol/Socket.h"

using namespace Vitriol;

Socket::Socket( AddressFamily address_family, SocketType socket_type )
{

#if defined( _WIN32 )
	WSADATA wsa;
	WSAStartup( MAKEWORD( 2, 2 ), &wsa );
#endif // _WIN32

	// Create socket
	native_handle_ = socket( static_cast< int >( address_family )
	                       , static_cast< int >( socket_type )
	                       , 0
	);
}

Socket::~Socket( void )
{
	// Close socket
	if( native_handle_ )
		close( native_handle_ );

#if defined( _WIN32 )
	WSACleanup();
#endif // _WIN32

}
