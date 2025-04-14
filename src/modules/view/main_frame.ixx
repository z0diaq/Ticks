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
export module view.main_frame;

import model.config;
import view.left_panel;
import view.right_panel;
import <memory>;
import <filesystem>;

import <wx/frame.h>;
import <wx/string.h>;
import <wx/gdicmn.h>; //wxSize, wxPoint

// Forward declarations for wxWidgets classes
//class wxFrame;
//class wxString;
//class wxPoint;
//class wxSize;
class wxSplitterWindow;
class wxCommandEvent;

/**
 * @brief Main application frame containing split panels
 */
export class MainFrame
{
public:
	// Constructor
	MainFrame( const wxString& title, const wxPoint& pos, const wxSize& size );

	// Get the wxFrame
	wxFrame* getFrame( ) const;

	// Initialize with config
	void initialize( const Config& config );

private:
	void createControls( );
	void bindEvents( );

	// Event handlers
	void onAbout( wxCommandEvent& event );
	void onExit( wxCommandEvent& event );
	void onOpenConfig( wxCommandEvent& event );
	void onSaveConfig( wxCommandEvent& event );

	// UI Controls
	wxFrame* frame_ = nullptr;
	wxSplitterWindow* splitter_ = nullptr;
	std::unique_ptr<LeftPanel> leftPanel_ = nullptr;
	std::unique_ptr<RightPanel> rightPanel_ = nullptr;

	// Config file path
	std::filesystem::path configPath_ = "config/default_config.yaml";
};

// Implementation will be in separate file due to wxWidgets dependencies
