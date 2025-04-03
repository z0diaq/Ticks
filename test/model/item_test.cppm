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
module;

#include <gtest/gtest.h>

export module item_test;

import model.item;
import <string>;

// Test fixture for Item tests
class ItemTest : public ::testing::Test {
protected:
    // Test values
    const std::string TEST_NAME = "Test Item";
    const std::string TEST_TYPE = "Test Type";
    const std::string TEST_ACTION = "Test Action";
    const int TEST_TIMEOUT = 120;
    
    // Create test item
    Item createTestItem() const {
        return Item(TEST_NAME, TEST_TYPE, TEST_ACTION, TEST_TIMEOUT);
    }
};

// Test default constructor
TEST_F(ItemTest, DefaultConstructor) {
    Item item;
    
    EXPECT_EQ("", item.getName());
    EXPECT_EQ("", item.getType());
    EXPECT_EQ("", item.getAction());
    EXPECT_EQ(0, item.getTimeout());
}

// Test parameterized constructor
TEST_F(ItemTest, ParameterizedConstructor) {
    Item item(TEST_NAME, TEST_TYPE, TEST_ACTION, TEST_TIMEOUT);
    
    EXPECT_EQ(TEST_NAME, item.getName());
    EXPECT_EQ(TEST_TYPE, item.getType());
    EXPECT_EQ(TEST_ACTION, item.getAction());
    EXPECT_EQ(TEST_TIMEOUT, item.getTimeout());
}

// Test withName method
TEST_F(ItemTest, WithName) {
    auto item = createTestItem();
    std::string newName = "New Name";
    
    auto newItem = item.withName(newName);
    
    // Original item should be unchanged
    EXPECT_EQ(TEST_NAME, item.getName());
    
    // New item should have the new name but same other values
    EXPECT_EQ(newName, newItem.getName());
    EXPECT_EQ(TEST_TYPE, newItem.getType());
    EXPECT_EQ(TEST_ACTION, newItem.getAction());
    EXPECT_EQ(TEST_TIMEOUT, newItem.getTimeout());
}

// Test withType method
TEST_F(ItemTest, WithType) {
    auto item = createTestItem();
    std::string newType = "New Type";
    
    auto newItem = item.withType(newType);
    
    // Original item should be unchanged
    EXPECT_EQ(TEST_TYPE, item.getType());
    
    // New item should have the new type but same other values
    EXPECT_EQ(TEST_NAME, newItem.getName());
    EXPECT_EQ(newType, newItem.getType());
    EXPECT_EQ(TEST_ACTION, newItem.getAction());
    EXPECT_EQ(TEST_TIMEOUT, newItem.getTimeout());
}

// Test withAction method
TEST_F(ItemTest, WithAction) {
    auto item = createTestItem();
    std::string newAction = "New Action";
    
    auto newItem = item.withAction(newAction);
    
    // Original item should be unchanged
    EXPECT_EQ(TEST_ACTION, item.getAction());
    
    // New item should have the new action but same other values
    EXPECT_EQ(TEST_NAME, newItem.getName());
    EXPECT_EQ(TEST_TYPE, newItem.getType());
    EXPECT_EQ(newAction, newItem.getAction());
    EXPECT_EQ(TEST_TIMEOUT, newItem.getTimeout());
}

// Test withTimeout method
TEST_F(ItemTest, WithTimeout) {
    auto item = createTestItem();
    int newTimeout = 300;
    
    auto newItem = item.withTimeout(newTimeout);
    
    // Original item should be unchanged
    EXPECT_EQ(TEST_TIMEOUT, item.getTimeout());
    
    // New item should have the new timeout but same other values
    EXPECT_EQ(TEST_NAME, newItem.getName());
    EXPECT_EQ(TEST_TYPE, newItem.getType());
    EXPECT_EQ(TEST_ACTION, newItem.getAction());
    EXPECT_EQ(newTimeout, newItem.getTimeout());
}

// Test equality operators
TEST_F(ItemTest, Equality) {
    auto item1 = createTestItem();
    auto item2 = createTestItem();
    auto item3 = item1.withName("Different Name");
    
    EXPECT_EQ(item1, item2);
    EXPECT_NE(item1, item3);
}

// Test factory function
TEST_F(ItemTest, Factory) {
    auto item = makeItem(TEST_NAME, TEST_TYPE, TEST_ACTION, TEST_TIMEOUT);
    
    EXPECT_EQ(TEST_NAME, item.getName());
    EXPECT_EQ(TEST_TYPE, item.getType());
    EXPECT_EQ(TEST_ACTION, item.getAction());
    EXPECT_EQ(TEST_TIMEOUT, item.getTimeout());
}

// Test functional composition of with* methods
TEST_F(ItemTest, FunctionalComposition) {
    auto item = createTestItem();
    std::string newName = "New Name";
    std::string newType = "New Type";
    
    auto newItem = item
        .withName(newName)
        .withType(newType);
    
    EXPECT_EQ(newName, newItem.getName());
    EXPECT_EQ(newType, newItem.getType());
    EXPECT_EQ(TEST_ACTION, newItem.getAction());
    EXPECT_EQ(TEST_TIMEOUT, newItem.getTimeout());
}