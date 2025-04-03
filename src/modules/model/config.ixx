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
export module model.config;

import model.item;
import <string>;
import <vector>;
import <functional>;
import <optional>;
import <filesystem>;
import <algorithm>;
import <fstream>;

// Forward declare YAML::Node for use with header-only yaml-cpp library
namespace YAML {
    class Node;
}

/**
 * @brief Configuration manager that handles loading and saving YAML configuration
 */
export class Config {
public:
    // Default constructor
    Config() = default;
    
    // Constructor with items
    explicit Config(std::vector<Item> items);
    
    // Load from YAML file
    [[nodiscard]] static std::optional<Config> loadFromYaml(const std::filesystem::path& filePath);
    
    // Save to YAML file
    bool saveToYaml(const std::filesystem::path& filePath) const;
    
    // Get all items
    [[nodiscard]] const std::vector<Item>& getItems() const noexcept;
    
    // Functional add, remove, update operations (immutable)
    [[nodiscard]] Config withAddedItem(Item item) const;
    [[nodiscard]] Config withRemovedItem(const Item& item) const;
    [[nodiscard]] Config withUpdatedItem(const Item& oldItem, Item newItem) const;
    
private:
    std::vector<Item> items_;
};

// Implementation will be added separately since it depends on yaml-cpp
// which is not a module-compatible library yet.