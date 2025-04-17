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
import view.config_dialog;

#include <wx/wx.h>
#include <wx/spinctrl.h>

ConfigDialog::ConfigDialog( wxWindow* parent, const Item& item )
	: originalItem_( item )
{

	dialog_ = new wxDialog( parent, wxID_ANY, "Configure Item", wxDefaultPosition, wxSize( 400, 300 ) );

	createControls( );
	bindEvents( );

	// Center the dialog
	dialog_->CenterOnParent( );
}

bool ConfigDialog::showDialog( )
{
	dialogResult_ = ( dialog_->ShowModal( ) == wxID_OK );
	return dialogResult_;
}

void ConfigDialog::createControls( )
{
	// Create a flexible grid sizer for form layout
	auto* mainSizer = new wxBoxSizer( wxVERTICAL );
	auto* formSizer = new wxFlexGridSizer( 4, 2, 10, 10 );
	formSizer->AddGrowableCol( 1 );

	// Name
	formSizer->Add( new wxStaticText( dialog_, wxID_ANY, "Name:" ), 0 );
	nameCtrl_ = new wxTextCtrl( dialog_, wxID_ANY, originalItem_.getName( ) );
	formSizer->Add( nameCtrl_, 1, wxEXPAND );

	// Type
	formSizer->Add( new wxStaticText( dialog_, wxID_ANY, "Type:" ), 0 );
	typeCtrl_ = new wxTextCtrl( dialog_, wxID_ANY, originalItem_.getType( ) );
	formSizer->Add( typeCtrl_, 1, wxEXPAND );

	// Action
	formSizer->Add( new wxStaticText( dialog_, wxID_ANY, "Action:" ), 0 );
	actionCtrl_ = new wxTextCtrl( dialog_, wxID_ANY, originalItem_.getAction( ) );
	formSizer->Add( actionCtrl_, 1, wxEXPAND );

	// Timeout
	formSizer->Add( new wxStaticText( dialog_, wxID_ANY, "Timeout (seconds):" ), 0 );
	timeoutCtrl_ = new wxSpinCtrl( dialog_, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 1, 86400, originalItem_.getTimeout( ) );
	formSizer->Add( timeoutCtrl_, 1, wxEXPAND );

	mainSizer->Add( formSizer, 1, wxEXPAND | wxALL, 10 );

	// Button sizer
	auto* buttonSizer = dialog_->CreateButtonSizer( wxOK | wxCANCEL );
	mainSizer->Add( buttonSizer, 0, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, 10 );

	dialog_->SetSizer( mainSizer );
}

void ConfigDialog::bindEvents( )
{
	// The default OK/Cancel buttons are already bound
}

Item ConfigDialog::getModifiedItem( ) const
{
	if( !dialogResult_ )
		return originalItem_; // Return original if dialog was canceled

	return originalItem_
		.withName( nameCtrl_->GetValue( ).ToStdString( ) )
		.withType( typeCtrl_->GetValue( ).ToStdString( ) )
		.withAction( actionCtrl_->GetValue( ).ToStdString( ) )
		.withTimeout( timeoutCtrl_->GetValue( ) );
}
