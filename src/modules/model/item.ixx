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
export module model.item;

import <string>;
import <functional>;
import <optional>;

/**
 * @brief Represents an immutable item with name, type, action, and timeout
 */
export class Item {
public:
    // Constructor with default values
    explicit Item(std::string name = "",
                 std::string type = "",
                 std::string action = "",
                 int timeout = 0);
    
    // Pure functional setters that return new items
    [[nodiscard]] Item withName(std::string newName) const;
    [[nodiscard]] Item withType(std::string newType) const;
    [[nodiscard]] Item withAction(std::string newAction) const;
    [[nodiscard]] Item withTimeout(int newTimeout) const;
    
    // Getters
    [[nodiscard]] const std::string& getName() const noexcept;
    [[nodiscard]] const std::string& getType() const noexcept;
    [[nodiscard]] const std::string& getAction() const noexcept;
    [[nodiscard]] int getTimeout() const noexcept;
    
    // Equality operators
    bool operator==(const Item& other) const;
    bool operator!=(const Item& other) const;
    
private:
    std::string name_;
    std::string type_;
    std::string action_;
    int timeout_;
};

// Factory function
export template<typename... Args>
[[nodiscard]] auto makeItem(Args&&... args) {
    return Item(std::forward<Args>(args)...);
}

// Implementation
Item::Item(std::string name, std::string type, std::string action, int timeout)
    : name_(std::move(name)), 
      type_(std::move(type)), 
      action_(std::move(action)), 
      timeout_(timeout) {}

Item Item::withName(std::string newName) const {
    return Item(std::move(newName), type_, action_, timeout_);
}

Item Item::withType(std::string newType) const {
    return Item(name_, std::move(newType), action_, timeout_);
}

Item Item::withAction(std::string newAction) const {
    return Item(name_, type_, std::move(newAction), timeout_);
}

Item Item::withTimeout(int newTimeout) const {
    return Item(name_, type_, action_, newTimeout);
}

const std::string& Item::getName() const noexcept {
    return name_;
}

const std::string& Item::getType() const noexcept {
    return type_;
}

const std::string& Item::getAction() const noexcept {
    return action_;
}

int Item::getTimeout() const noexcept {
    return timeout_;
}

bool Item::operator==(const Item& other) const {
    return name_ == other.name_ &&
           type_ == other.type_ &&
           action_ == other.action_ &&
           timeout_ == other.timeout_;
}

bool Item::operator!=(const Item& other) const {
    return !(*this == other);
}