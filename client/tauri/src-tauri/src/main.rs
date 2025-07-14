// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

// fn main() {
//     my_tauri_chat_lib::run()
// }
//
use std::ffi::{CStr, CString};

#[link(name = "client", kind = "dylib")] // Use "dylib" for macOS, "shared" for Linux
extern "C" {
    fn client_connect() -> i32;
    fn client_close();
}

fn main() {
    // Example: Call add
    let sum = unsafe { client_connect() };

}
