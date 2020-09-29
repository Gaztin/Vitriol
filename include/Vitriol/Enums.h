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

namespace Vitriol
{
	enum class AddressFamily : int
	{
		None        = -1,
		Unspecified = AF_UNSPEC,
		Inet        = AF_INET,
		Inet6       = AF_INET6,
	};

	enum class SocketType : int
	{
		None                     = -1,
		Stream                   = SOCK_STREAM,
		Datagram                 = SOCK_DGRAM,
		Raw                      = SOCK_RAW,
		ReliablyDeliveredMessage = SOCK_RDM,
		SequencedPacketStream    = SOCK_SEQPACKET,
	};

	enum class Protocol : int
	{
		None = -1,
		TCP  = IPPROTO_TCP,
		UDP  = IPPROTO_UDP,
	};

	// For ease of use, unary plus (+) operator will perform a cast to underlying type
	constexpr int operator+( AddressFamily address_family ) { return static_cast< int >( address_family ); }
	constexpr int operator+( SocketType socket_type )       { return static_cast< int >( socket_type ); }
	constexpr int operator+( Protocol protocol )            { return static_cast< int >( protocol ); }
}
