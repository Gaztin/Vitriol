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

int main( int /*argc*/, char* /*argv*/[] )
{
	Vitriol::Socket socket( Vitriol::AddressFamily::Inet, Vitriol::SocketType::Stream, Vitriol::Protocol::TCP );

	// Bind and listen on socket
	if( socket.Bind( 9182 ) && socket.Listen() )
	{
		std::cout << "Waiting for connections..\n";

		// Wait for an incoming connection
		if( auto incoming = socket.Accept() )
		{
			const std::string address_string = incoming->GetAddressString();

			std::cout << "Incoming: " << address_string << "\n";

			constexpr size_t buf_size       = 512;
			char             buf[ buf_size ];
			size_t           bytes_received = 0;

			while( ( bytes_received = incoming->Receive( buf, buf_size ) ) > 0 )
			{
				const std::string buf_string( buf, bytes_received );

				std::cout << buf_string;
			}

			std::cout << "Closed " << address_string << "\n";
		}
	}

	// Pause before exit
	std::cout << "Press any key to exit.. ";
	std::cin.get();

	return 0;
}

#if defined( _WIN32 )

#include <Windows.h>

int WINAPI WinMain( HINSTANCE /*instance*/, HINSTANCE /*prev_instance*/, LPSTR /*cmd_line*/, int /*cmd_show*/ )
{
	return main( __argc, __argv );
}

#endif // _WIN32
