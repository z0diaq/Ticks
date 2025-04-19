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
import view.right_panel;

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/dnd.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

class SimpleTimerDataObject : public wxDataObjectSimple
{
};

// Custom drop target for receiving items
class ItemDropTarget : public wxDropTarget
{
public:
	using OnDropCallback = std::function<void( wxCoord, wxCoord, const Item& )>;
	//using OnDragEnterCallback = std::function<void( wxDragResult& )>;
	using OnDragOverCallback = std::function<void( wxCoord, wxCoord, wxDragResult& )>;

	ItemDropTarget( OnDropCallback onDrop,
		//OnDragEnterCallback onDragEnter = nullptr
		OnDragOverCallback onDragOver = nullptr )
		: wxDropTarget( ),
		onDrop_( std::move( onDrop ) ),
		//onDragEnter_( std::move( onDragEnter ) )//,
		onDragOver_( std::move( onDragOver ) )
	{

		// Create and set data object
		auto* dataObject = new SimpleTimerDataObject( );
		SetDataObject( dataObject );
	}

	/*wxDragResult OnDragEnter( wxCoord x, wxCoord y, wxDragResult defResult ) override
	{
		if( onDragEnter_ )
		{
			wxDragResult result = defResult;
			onDragEnter_( result );
			return result;
		}
		return defResult;
	}*/

	wxDragResult OnDragOver( wxCoord x, wxCoord y, wxDragResult defResult ) override
	{
		if( onDragOver_ )
		{
			wxDragResult result = defResult;
			onDragOver_( x, y, result );
			return result;
		}
		return defResult;
	}

	wxDragResult OnData( wxCoord x, wxCoord y, wxDragResult defResult ) override
	{
		// Simplified implementation - we're assuming the last dragged item
		// In a real app, you'd get the data from the data object
		if( onDrop_ && lastDraggedItem_ )
			onDrop_( x, y, *lastDraggedItem_ );
		
		return defResult;
	}

	static void setLastDraggedItem( const Item& item )
	{
		lastDraggedItem_ = std::make_unique<Item>( item );
	}

	static void clearLastDraggedItem( )
	{
		lastDraggedItem_.reset( );
	}

private:
	OnDropCallback onDrop_;
	//OnDragEnterCallback onDragEnter_;
	OnDragOverCallback onDragOver_;

	// This is a hack for simplicity - normally you'd use the data object
	static inline std::unique_ptr<Item> lastDraggedItem_;
};


RightPanel::RightPanel( wxWindow* parent ) {
	panel_ = new wxPanel( parent, wxID_ANY );
	timer_ = new wxTimer( panel_, TIMER_ID );

	createControls( );
	bindEvents( );

	// Start the timer for updating active items
	timer_->Start( 1000 ); // Update every second
}

wxPanel* RightPanel::getPanel( ) const
{
	return panel_;
}

void RightPanel::createControls( )
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
	listCtrl_->AppendColumn( "Remaining" );
	listCtrl_->AppendColumn( "ETA" );

	// Set up drop target
	auto onDrop = [this]( wxCoord x, wxCoord y, const Item& item )
	{
		this->onDrop( x, y, item );
	};

	auto onDragEnter = [this]( wxDragResult& result )
	{
		this->onDragEnter( result );
	};

	auto onDragOver = [this]( wxCoord x, wxCoord y, wxDragResult& result )
	{
		this->onDragOver( x, y, result );
	};

	auto* dropTarget = new ItemDropTarget( onDrop, /*onDragEnter,*/ onDragOver );
	listCtrl_->SetDropTarget( dropTarget );

	// Add the list to the sizer
	sizer->Add( listCtrl_, 1, wxEXPAND | wxALL, 5 );

	// Set the sizer
	panel_->SetSizer( sizer );
}

void RightPanel::bindEvents( )
{
	// Bind timer event
	panel_->Bind( wxEVT_TIMER, &RightPanel::onTimer, this, TIMER_ID );

	// Bind list events
	listCtrl_->Bind( wxEVT_LIST_ITEM_ACTIVATED, &RightPanel::onItemActivated, this );
}

bool RightPanel::addItem( const Item& item )
{
	// Create config dialog
	ConfigDialog dialog( panel_, item );

	// Show dialog and process result
	if( dialog.showDialog( ) ) {
		// Get the modified item
		Item modifiedItem = dialog.getModifiedItem( );

		// Create a callback for when the timer completes
		auto onComplete = [this]( ) {
			// Could play a sound or show a notification
			wxBell( );
			};

		// Create an active item
		activeItems_.emplace_back( modifiedItem, onComplete );

		// Start the timer
		activeItems_.back( ).start( );

		// Update the list
		updateList( );

		return true;
	}

	return false;
}

void RightPanel::updateTimers( )
{
	// Update all active items
	for( auto& activeItem : activeItems_ ) {
		activeItem.update( );
	}

	// Update the display
	updateList( );
}

void RightPanel::updateList( )
{
	// Clear the list
	listCtrl_->DeleteAllItems( );

	// Add each active item
	for( size_t i = 0; i < activeItems_.size( ); ++i ) {
		const auto& activeItem = activeItems_[ i ];
		const auto& item = activeItem.getItem( );

		long index = listCtrl_->InsertItem( i, item.getName( ) );
		listCtrl_->SetItem( index, 1, item.getType( ) );
		listCtrl_->SetItem( index, 2, item.getAction( ) );
		listCtrl_->SetItem( index, 3, activeItem.getRemainingTimeString( ) );
		listCtrl_->SetItem( index, 4, formatTimePoint( activeItem.getETA( ) ) );

		// Store the item index for later retrieval
		listCtrl_->SetItemData( index, i );
	}

	// Resize columns
	for( int i = 0; i < 5; ++i )
		listCtrl_->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
}

void RightPanel::onDragEnter( wxDragResult& result )
{
	// Accept the drag
	result = wxDragCopy;
}

void RightPanel::onDragOver( wxCoord x, wxCoord y, wxDragResult& result )
{
	// Accept the drag
	result = wxDragCopy;
}

void RightPanel::onDrop( wxCoord x, wxCoord y, const Item& item )
{
	// Add the item
	addItem( item );
}

void RightPanel::onTimer( wxTimerEvent& event )
{
	// Update all timers
	updateTimers( );
}

void RightPanel::onItemActivated( wxListEvent& event )
{
	// Get the selected item
	long itemIndex = event.GetIndex( );
	long dataIndex = listCtrl_->GetItemData( itemIndex );

	if( dataIndex >= 0 && dataIndex < static_cast< long >( activeItems_.size( ) ) ) {
		// Toggle the timer
		auto& activeItem = activeItems_[ dataIndex ];

		if( activeItem.isRunning( ) ) {
			activeItem.stop( );
		}
		else if( !activeItem.isCompleted( ) ) {
			activeItem.start( );
		}
		else {
			activeItem.reset( );
			activeItem.start( );
		}

		// Update the display
		updateList( );
	}
}

wxString RightPanel::formatTimePoint( const std::chrono::system_clock::time_point& timePoint )
{
	// Convert to time_t
	std::time_t time = std::chrono::system_clock::to_time_t( timePoint );

	// Format using strftime
	std::stringstream ss;
	ss << std::put_time( std::localtime( &time ), "%H:%M:%S" );

	return ss.str( );
}
