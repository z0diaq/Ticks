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
module controller.application;

#include <wx/wx.h>
#include <wx/stdpaths.h>

// wxApp implementation
class AppImpl : public wxApp {
public:
    bool OnInit() override {
        return true;
    }
};

IMPLEMENT_APP_NO_MAIN(AppImpl)

Application::Application() {
    // Create the wxApp instance
    wxApp::SetInstance(new AppImpl());
}

bool Application::initialize(int& argc, char** argv) {
    // Initialize the wxWidgets application
    app_ = wxApp::GetInstance();
    if (!wxEntryStart(argc, argv)) {
        return false;
    }
    
    if (!app_->OnInit()) {
        return false;
    }
    
    // Set application name
    app_->SetAppName("DragDropTimer");
    
    // Create main frame
    mainFrame_ = std::make_unique<MainFrame>(
        "DragDropTimer",
        wxDefaultPosition,
        wxSize(800, 600)
    );
    
    // Show the frame
    mainFrame_->getFrame()->Show(true);
    
    // Load configuration
    std::filesystem::path configPath = "config/default_config.yaml";
    auto config = loadConfiguration(configPath);
    
    if (config) {
        mainFrame_->initialize(*config);
    } else {
        // Create an empty configuration
        mainFrame_->initialize(Config());
        
        // Show error message
        wxMessageBox("Failed to load configuration. Using empty configuration.",
                    "Configuration Error", wxOK | wxICON_WARNING);
    }
    
    return true;
}

int Application::run() {
    return app_->OnRun();
}

std::optional<Config> Application::loadConfiguration(const std::filesystem::path& path) {
    // Try to load from the specified path
    auto config = Config::loadFromYaml(path);
    if (config) {
        return config;
    }
    
    // If failed, try to find the file in the executable directory
    auto exePath = wxStandardPaths::Get().GetExecutablePath();
    auto exeDir = std::filesystem::path(exePath.ToStdString()).parent_path();
    auto defaultPath = exeDir / "config" / "default_config.yaml";
    
    return Config::loadFromYaml(defaultPath);
}