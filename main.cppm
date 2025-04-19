/*
 * Ticks - Timers, Events and GUI
 * Copyright (C) 2025 Piotr Tkaczyk
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <iostream>

import controller.application;

int main( int argc, char* argv[ ] )
{
	try
	{
		// Create and initialize the application
		Application app;
		if( !app.initialize( argc, argv ) )
		{
			std::cerr << "Failed to initialize application" << std::endl;
			return 1;
		}

		// Run the application
		return app.run( );
	}
	catch( const std::exception& e )
	{
		std::cerr << "Exception: " << e.what( ) << std::endl;
		return 1;
	}
	catch( ... )
	{
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
}
