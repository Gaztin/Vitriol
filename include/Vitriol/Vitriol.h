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

#include <string>
#include <unordered_map>

#if defined( _WIN32 )
  #include <WinSock2.h>
  #define close closesocket
#endif // _WIN32

namespace Vitriol
{
	using HttpFieldMap = std::unordered_map< std::string, std::string >;

#if defined( _WIN32 )

	using native_socket_t = SOCKET;

	constexpr int             native_socket_error_v   = SOCKET_ERROR;
	constexpr native_socket_t invalid_native_socket_v = INVALID_SOCKET;
	constexpr int             error_would_block_v     = WSAEWOULDBLOCK;

	inline int GetLastSocketError( void ) { return WSAGetLastError(); }

#else // _WIN32

	using native_socket_t = int;

	constexpr int             native_socket_error_v   = -1;
	constexpr native_socket_t invalid_native_socket_v = -1;
	constexpr int             error_would_block_v     = EWOULDBLOCK;

	inline int GetLastSocketError( void ) { return errno; }

#endif // _WIN32

}
