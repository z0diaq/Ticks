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
module view.left_panel;

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/dnd.h>
#include <algorithm>
#include <ranges>

wxDEFINE_EVENT( EVT_ITEM_DRAG_BEGIN, wxCommandEvent );

// Custom data object for drag and drop
class ItemDataObject : public wxDataObject
{
public:
	explicit ItemDataObject( const Item& item = Item( ) )
		: item_( item )
	{
	}

	[[nodiscard]] const Item& getItem( ) const
	{
		return item_;
	}

	// wxDataObject interface
	wxDataFormat GetPreferredFormat( Direction ) const override
	{
		return wxDataFormat( "application/x-item" );
	}

	size_t GetDataSize( const wxDataFormat& ) const override
	{
		// This is a simplified implementation - in a real app, you'd serialize the item
		return sizeof( Item* );
	}

	bool GetDataHere( const wxDataFormat&, void* buf ) const override
	{
		// This is a simplified implementation - in a real app, you'd serialize the item
		*static_cast< Item** >( buf ) = const_cast< Item* >( &item_ );
		return true;
	}

	bool SetData( const wxDataFormat&, size_t len, const void* buf ) override
	{
		// This is a simplified implementation - in a real app, you'd deserialize the item
		if( len != sizeof( Item* ) )
			return false;
		item_ = **static_cast< Item* const* >( buf );
		return true;
	}

private:
	Item item_;
};

// Custom drop source for initiating drag
class ItemDropSource : public wxDropSource
{
public:
	ItemDropSource( wxWindow* win, const Item& item )
		: wxDropSource( win ), dataObject_( item ) {
		SetData( dataObject_ );
	}

private:
	ItemDataObject dataObject_;
};

LeftPanel::LeftPanel( wxWindow* parent )
{
	panel_ = new wxPanel( parent, wxID_ANY );
	createControls( );
	bindEvents( );
}

wxPanel* LeftPanel::getPanel( ) const
{
	return panel_;
}

void LeftPanel::createControls( )
{
	// Create a sizer
	auto* sizer = new wxBoxSizer( wxVERTICAL );

	// Create a list control
	listCtrl_ = new wxListCtrl( panel_, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_SINGLE_SEL );

	// Add columns
	listCtrl_->AppendColumn( "Name" );
	listCtrl_->AppendColumn( "Type" );
	listCtrl_->AppendColumn( "Action" );
	listCtrl_->AppendColumn( "Timeout" );

	// Add the list to the sizer
	sizer->Add( listCtrl_, 1, wxEXPAND | wxALL, 5 );

	// Set the sizer
	panel_->SetSizer( sizer );
}

void LeftPanel::bindEvents( )
{
	// Bind list events
	listCtrl_->Bind( wxEVT_LIST_BEGIN_DRAG, &LeftPanel::onListItemBeginDrag, this );
	listCtrl_->Bind( wxEVT_LIST_ITEM_SELECTED, &LeftPanel::onListItemSelected, this );
}

void LeftPanel::loadItems( const Config& config )
{
	items_ = config.getItems( );
	updateList( );
}

const std::vector<Item>& LeftPanel::getItems( ) const
{
	return items_;
}

void LeftPanel::updateList( )
{
	// Clear the list
	listCtrl_->DeleteAllItems( );

	// Add each item
	for( size_t i = 0; i < items_.size( ); ++i ) {
		const auto& item = items_[ i ];

		long index = listCtrl_->InsertItem( i, item.getName( ) );
		listCtrl_->SetItem( index, 1, item.getType( ) );
		listCtrl_->SetItem( index, 2, item.getAction( ) );
		listCtrl_->SetItem( index, 3, wxString::Format( "%d", item.getTimeout( ) ) );

		// Store the item index for later retrieval
		listCtrl_->SetItemData( index, i );
	}

	// Resize columns
	for( int i = 0; i < 4; ++i )
		listCtrl_->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
}

void LeftPanel::onListItemBeginDrag( wxListEvent& event )
{
	// Get the selected item
	long itemIndex = event.GetIndex( );
	long dataIndex = listCtrl_->GetItemData( itemIndex );

	if( dataIndex >= 0 && dataIndex < static_cast< long >( items_.size( ) ) )
	{
		const Item& item = items_[ dataIndex ];

		// Create a data object
		ItemDataObject itemData( item );

		// Create a drop source
		ItemDropSource dropSource( panel_, item );

		// Do drag and drop
		wxDragResult result = dropSource.DoDragDrop( wxDrag_DefaultMove );

		// Notify parent about drag start
		wxCommandEvent dragEvent( EVT_ITEM_DRAG_BEGIN );
		dragEvent.SetClientData( new Item( item ) ); // Set client data to a copy of the item
		panel_->ProcessEvent( dragEvent );
	}
}

void LeftPanel::onListItemSelected( wxListEvent& event )
{
	// This could be used for preview or additional actions
}
