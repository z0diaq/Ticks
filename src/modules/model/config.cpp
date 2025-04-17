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

//#include <yaml-cpp/yaml.h>

import model.config;
import model.item;

#include <yaml-cpp/yaml.h>
//import <yaml-cpp/yaml.h>;

// Custom namespace for YAML conversion
namespace YAML
{
	template<>
	struct convert<Item>
	{
		static Node encode( const Item& item )
		{
			Node node;
			node[ "name" ] = item.getName( );
			node[ "type" ] = item.getType( );
			node[ "action" ] = item.getAction( );
			node[ "timeout" ] = item.getTimeout( );
			return node;
		}

		static bool decode( const Node& node, Item& item )
		{
			if( !node.IsMap( ) )
				return false;

			// Use default values if properties are missing
			std::string name = node[ "name" ] ? node[ "name" ].as<std::string>( ) : "";
			std::string type = node[ "type" ] ? node[ "type" ].as<std::string>( ) : "";
			std::string action = node[ "action" ] ? node[ "action" ].as<std::string>( ) : "";
			int timeout = node[ "timeout" ] ? node[ "timeout" ].as<int>( ) : 0;

			item = Item( name, type, action, timeout );
			return true;
		}
	};
}

Config::Config( std::vector<Item> items )
	: items_( std::move( items ) )
{
}

std::optional<Config> Config::loadFromYaml( const std::filesystem::path& filePath ) {
	try {
		if( !std::filesystem::exists( filePath ) )
			return std::nullopt;

		YAML::Node rootNode = YAML::LoadFile( filePath.string( ) );
		if( !rootNode[ "items" ] || !rootNode[ "items" ].IsSequence( ) )
			return Config( ); // Return empty config if no items section

		std::vector<Item> items;
		for( const auto& itemNode : rootNode[ "items" ] ) {
			if( itemNode.IsMap( ) )
				items.push_back( itemNode.as<Item>( ) );
		}

		return Config( std::move( items ) );
	}
	catch( const std::exception& )
	{
		return std::nullopt;
	}
}

bool Config::saveToYaml( const std::filesystem::path& filePath ) const {
	try {
		YAML::Node rootNode;
		YAML::Node itemsNode;

		for( const auto& item : items_ )
			itemsNode.push_back( item );

		rootNode[ "items" ] = itemsNode;

		std::ofstream fout( filePath );
		if( !fout )
			return false;

		fout << YAML::Dump( rootNode );
		return true;
	}
	catch( const std::exception& )
	{
		return false;
	}
}

const std::vector<Item>& Config::getItems( ) const noexcept
{
	return items_;
}

Config Config::withAddedItem( Item item ) const
{
	auto newItems = items_;
	newItems.push_back( std::move( item ) );
	return Config( std::move( newItems ) );
}

Config Config::withRemovedItem( const Item& item ) const
{
	auto newItems = items_;
	newItems.erase(
		std::remove_if( newItems.begin( ), newItems.end( ),
		[&item]( const auto& existingItem ) { return existingItem == item; } ),
		newItems.end( )
	);
	return Config( std::move( newItems ) );
}

Config Config::withUpdatedItem( const Item& oldItem, Item newItem ) const
{
	auto newItems = items_;
	std::replace_if( newItems.begin( ), newItems.end( ),
		[&oldItem]( const auto& existingItem ) { return existingItem == oldItem; },
		std::move( newItem )
	);
	return Config( std::move( newItems ) );
}
