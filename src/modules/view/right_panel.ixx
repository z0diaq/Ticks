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
export module view.right_panel;

export import model.item;
import model.active_item;
export import view.config_dialog;
import <vector>;
import <memory>;
import <chrono>;

import <wx/defs.h>;

// Forward declarations for wxWidgets classes
import <wx/wx.h>;
import <wx/panel.h>;
import <wx/listctrl.h>;
import <wx/timer.h>;

//enum wxDragResult;
import <wx/dnd.h>;

/**
 * @brief Right panel containing active items with timers
 */
export class RightPanel
{
public:
	// Constructor
	explicit RightPanel( wxWindow* parent );

	// Get the wxPanel
	wxPanel* getPanel( ) const;

	// Add an active item
	bool addItem( const Item& item );

	// Update all timers
	void updateTimers( );

private:
	// Timer ID for updating active items
	static constexpr int TIMER_ID = 1001;

	void createControls( );
	void bindEvents( );
	void updateList( );

	// Event handlers
	void onDragEnter( wxDragResult& result );
	void onDragOver( wxCoord x, wxCoord y, wxDragResult& result );
	void onDrop( wxCoord x, wxCoord y, const Item& item );
	void onTimer( wxTimerEvent& event );
	void onItemActivated( wxListEvent& event );

	// Format time point to string
	static wxString formatTimePoint( const std::chrono::system_clock::time_point& timePoint );

	// UI controls
	wxPanel* panel_ = nullptr;
	wxListCtrl* listCtrl_ = nullptr;
	wxTimer* timer_ = nullptr;

	// Data
	std::vector<ActiveItem> activeItems_;
};

// Implementation will be in separate file due to wxWidgets dependencies
