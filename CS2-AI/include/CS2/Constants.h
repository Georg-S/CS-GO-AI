#pragma once
#include <string>
#include <filesystem>

const std::filesystem::path config_folder_path = "Configuration";
const std::filesystem::path config_file_path = config_folder_path / "config.json";
const std::filesystem::path general_offsets_file_path = config_folder_path / "offsets.json";
const std::filesystem::path client_offsets_file_path = config_folder_path / "client_dll.json";
const std::filesystem::path engine_offsets_file_path = config_folder_path / "engine2_dll.json";
const std::filesystem::path buttons_offsets_file_path = config_folder_path / "buttons.json";