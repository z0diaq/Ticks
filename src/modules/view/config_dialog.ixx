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
export module view.config_dialog;

import model.item;
import <memory>;

import <wx/wx.h>;
import <wx/dialog.h>;
import <wx/spinctrl.h>;

/**
 * @brief Dialog for configuring an item
 */
export class ConfigDialog
{
public:
	// Constructor
	ConfigDialog( wxWindow* parent, const Item& item );

	// Show the dialog and get result
	bool showDialog( );

	// Get the modified item
	[[nodiscard]] Item getModifiedItem( ) const;

private:
	void createControls( );
	void bindEvents( );

	// UI Controls
	wxDialog* dialog_ = nullptr;
	wxTextCtrl* nameCtrl_ = nullptr;
	wxTextCtrl* typeCtrl_ = nullptr;
	wxTextCtrl* actionCtrl_ = nullptr;
	wxSpinCtrl* timeoutCtrl_ = nullptr;

	// Original and modified item
	Item originalItem_;
	bool dialogResult_ = false;
};

// Implementation will be in separate file due to wxWidgets dependencies
