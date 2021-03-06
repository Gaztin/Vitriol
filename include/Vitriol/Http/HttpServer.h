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

#include "Vitriol/Http/HttpRequest.h"
#include "Vitriol/Http/HttpResponse.h"
#include "Vitriol/Socket/Socket.h"

#include <list>
#include <mutex>
#include <thread>
#include <vector>

namespace Vitriol
{
	class HttpServer
	{
	public:

		         HttpServer( const HttpServer& ) = delete;
		         HttpServer( HttpServer&& )      = default;
		         HttpServer( uint16_t port = 80 );
		virtual ~HttpServer( void )              = default;

		HttpServer& operator=( const HttpServer& ) = delete;
		HttpServer& operator=( HttpServer&& )      = default;

	public:

		void   StartThreads      ( size_t thread_count );
		bool   IsRunning         ( void ) const;
		size_t GetNumConnections ( void );

	protected:

		virtual void OnRequest( HttpRequest request ) = 0;

	private:

		void        ThreadEntry  ( void );
		HttpRequest ParseRequest ( std::reference_wrapper< SocketConnection > connection, std::string_view data ) const;

	private:

		Socket                        socket_;
		std::vector< std::thread >    threads_;
		std::list< SocketConnection > all_connections_;
		std::mutex                    connections_mutex_;

	};
}
