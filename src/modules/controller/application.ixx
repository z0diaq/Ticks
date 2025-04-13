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
export module controller.application;

import view.main_frame;
import model.config;

import <memory>;
import <optional>;
import <filesystem>;

// Forward declaration for wxWidgets classes
class wxApp;
class wxString;

/**
 * @brief Main wxWidgets application class
 */
export class Application {
public:
	// Constructor
	Application( );

	// Initialize the application
	bool initialize( int& argc, char** argv );

	// Run the application
	int run( );

private:
	// Load configuration
	std::optional<Config> loadConfiguration( const std::filesystem::path& path );

	// wxApp instance
	wxApp* app_ = nullptr;

	// Main frame
	std::unique_ptr<MainFrame> mainFrame_;
};

// Implementation will be in separate file due to wxWidgets dependencies