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
module view.main_frame;

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

enum {
    ID_OPEN_CONFIG = wxID_HIGHEST + 1,
    ID_SAVE_CONFIG
};

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) {
    frame_ = new wxFrame(nullptr, wxID_ANY, title, pos, size);
    
    createControls();
    bindEvents();
}

wxFrame* MainFrame::getFrame() const {
    return frame_;
}

void MainFrame::createControls() {
    // Create menu bar
    auto* menuBar = new wxMenuBar;
    
    // File menu
    auto* fileMenu = new wxMenu;
    fileMenu->Append(ID_OPEN_CONFIG, "&Open Config...\tCtrl+O");
    fileMenu->Append(ID_SAVE_CONFIG, "&Save Config...\tCtrl+S");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT);
    menuBar->Append(fileMenu, "&File");
    
    // Help menu
    auto* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT);
    menuBar->Append(helpMenu, "&Help");
    
    frame_->SetMenuBar(menuBar);
    
    // Create status bar
    frame_->CreateStatusBar();
    frame_->SetStatusText("Ready");
    
    // Create splitter window
    splitter_ = new wxSplitterWindow(frame_, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    
    // Create panels
    leftPanel_ = std::make_unique<LeftPanel>(splitter_);
    rightPanel_ = std::make_unique<RightPanel>(splitter_);
    
    // Split the window
    splitter_->SplitVertically(leftPanel_->getPanel(), rightPanel_->getPanel());
    splitter_->SetMinimumPaneSize(200);
    splitter_->SetSashPosition(frame_->GetSize().GetWidth() / 2);
}

void MainFrame::bindEvents() {
    // Bind menu events
    frame_->Bind(wxEVT_MENU, &MainFrame::onAbout, this, wxID_ABOUT);
    frame_->Bind(wxEVT_MENU, &MainFrame::onExit, this, wxID_EXIT);
    frame_->Bind(wxEVT_MENU, &MainFrame::onOpenConfig, this, ID_OPEN_CONFIG);
    frame_->Bind(wxEVT_MENU, &MainFrame::onSaveConfig, this, ID_SAVE_CONFIG);
}

void MainFrame::initialize(const Config& config) {
    // Load items into left panel
    leftPanel_->loadItems(config);
}

void MainFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("DragDropTimer\n\nA timer application with drag and drop functionality.",
                "About DragDropTimer", wxOK | wxICON_INFORMATION);
}

void MainFrame::onExit(wxCommandEvent& event) {
    frame_->Close(true);
}

void MainFrame::onOpenConfig(wxCommandEvent& event) {
    // Show file dialog
    wxFileDialog openDialog(frame_, "Open Configuration", "", "",
                          "YAML files (*.yaml;*.yml)|*.yaml;*.yml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    // Get the file path
    configPath_ = openDialog.GetPath().ToStdString();
    
    // Load the configuration
    auto config = Config::loadFromYaml(configPath_);
    if (!config) {
        wxMessageBox("Failed to load configuration.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Initialize with the new configuration
    initialize(*config);
    
    // Update status
    frame_->SetStatusText("Configuration loaded from: " + openDialog.GetPath());
}

void MainFrame::onSaveConfig(wxCommandEvent& event) {
    // Show file dialog
    wxFileDialog saveDialog(frame_, "Save Configuration", "", "",
                          "YAML files (*.yaml;*.yml)|*.yaml;*.yml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (saveDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    // Get the file path
    auto savePath = saveDialog.GetPath().ToStdString();
    
    // Create configuration from left panel items
    Config config(leftPanel_->getItems());
    
    // Save the configuration
    if (!config.saveToYaml(savePath)) {
        wxMessageBox("Failed to save configuration.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Update status
    frame_->SetStatusText("Configuration saved to: " + saveDialog.GetPath());
}