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
export module model.active_item;

import model.item;
import model.timer;
import <memory>;
import <string>;
import <functional>;
import <optional>;

/**
 * @brief A class representing an active item with timer functionality
 */
export class ActiveItem
{
public:
	using Callback = std::function<void( )>;

	// Constructor
	explicit ActiveItem( Item item, std::optional<Callback> onCompleteCallback = std::nullopt );

	// Start the timer
	void start( );

	// Stop the timer
	void stop( );

	// Reset the timer
	void reset( );

	// Update the timer - to be called periodically
	void update( );

	// Get the original item
	[[nodiscard]] const Item& getItem( ) const noexcept;

	// Get remaining time as string
	[[nodiscard]] std::string getRemainingTimeString( ) const;

	// Get ETA as time_point
	[[nodiscard]] Timer::TimePoint getETA( ) const;

	// Check if timer is running
	[[nodiscard]] bool isRunning( ) const;

	// Check if timer is completed
	[[nodiscard]] bool isCompleted( ) const;

	// Functional setter for item
	[[nodiscard]] ActiveItem withItem( Item newItem ) const;

private:
	Item item_;
	Timer timer_;
};

// Implementation
ActiveItem::ActiveItem( Item item, std::optional<Callback> onCompleteCallback )
	: item_( std::move( item ) ),
	timer_( item_.getTimeout( ), std::move( onCompleteCallback ) )
{
}

void ActiveItem::start( )
{
	timer_.start( );
}

void ActiveItem::stop( )
{
	timer_.stop( );
}

void ActiveItem::reset( )
{
	timer_.reset( );
}

void ActiveItem::update( )
{
	timer_.update( );
}

const Item& ActiveItem::getItem( ) const noexcept
{
	return item_;
}

std::string ActiveItem::getRemainingTimeString( ) const
{
	return timer_.getRemainingTimeString( );
}

Timer::TimePoint ActiveItem::getETA( ) const
{
	return timer_.getETA( );
}

bool ActiveItem::isRunning( ) const
{
	return timer_.isRunning( );
}

bool ActiveItem::isCompleted( ) const
{
	return timer_.isCompleted( );
}

ActiveItem ActiveItem::withItem( Item newItem ) const
{
	return ActiveItem( std::move( newItem ) );
}