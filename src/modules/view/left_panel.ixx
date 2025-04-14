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
export module view.left_panel;

import model.item;
import model.config;
import <vector>;
import <memory>;

// Forward declarations for wxWidgets classes
class wxWindow;
class wxPanel;
class wxListCtrl;
class wxListEvent;
class wxCommandEvent;

// Custom event for drag start
// TODO: fixme
//export wxDECLARE_EVENT( EVT_ITEM_DRAG_BEGIN, wxCommandEvent );

/**
 * @brief Left panel containing draggable items from configuration
 */
export class LeftPanel
{
public:
	// Constructor
	explicit LeftPanel( wxWindow* parent );

	// Get the wxPanel
	wxPanel* getPanel( ) const;

	// Load items from configuration
	void loadItems( const Config& config );

	// Get items
	[[nodiscard]] const std::vector<Item>& getItems( ) const;

private:
	void createControls( );
	void bindEvents( );
	void updateList( );

	// Custom event handlers
	void onListItemBeginDrag( wxListEvent& event );
	void onListItemSelected( wxListEvent& event );

	// UI controls
	wxPanel* panel_ = nullptr;
	wxListCtrl* listCtrl_ = nullptr;

	// Items
	std::vector<Item> items_;
};

// Implementation will be in separate file due to wxWidgets dependencies