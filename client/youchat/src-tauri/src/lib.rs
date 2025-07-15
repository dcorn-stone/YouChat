// src-tauri/src/lib.rs

#![cfg_attr(mobile, feature(tauri_mobile))]

use std::ffi::CString;
use std::os::raw::{c_char, c_int};

// 1. Link to the static C library (built via build.rs)
#[link(name = "client", kind = "static")]
extern "C" {
    fn client_connect(address: *const c_char, port: c_int) -> c_int;
    fn client_close();
    fn signup(username: *const c_char, nickname: *const c_char, password: *const c_char) -> c_int;
    fn login(username: *const c_char, password: *const c_char) -> c_int;
}

// 2. Safe Rust wrappers around the C functions
pub fn connect_to_server(address: &str, port: i32) -> i32 {
    let c_address = CString::new(address).expect("NUL byte in username");

    let end = unsafe { client_connect(c_address.as_ptr(), port as c_int)};
    end as i32
}

pub fn signup_user(username: &str, nickname: &str, password: &str) -> i32 {
    let c_username = CString::new(username).expect("NUL byte in username");
    let c_nickname = CString::new(nickname).expect("NUL byte in nickname");
    let c_password = CString::new(password).expect("NUL byte in password");

    // Unsafe FFI call
    let code = unsafe { signup(c_username.as_ptr(), c_nickname.as_ptr(), c_password.as_ptr()) };
    code as i32
}

pub fn log_in(username: &str, password: &str) -> i32{
    let c_username = CString::new(username).expect("NUL byte in username");
    let c_password = CString::new(password).expect("NUL byte in password");
    let code = unsafe { login(c_username.as_ptr(), c_password.as_ptr()) };
    code as i32
}

// 3. Expose as Tauri commands
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
fn connect(address: String, port:i32) -> i32 {
    // Unsafe FFI call
    connect_to_server(&address, port)
}

#[tauri::command]
fn disconnect() {
    unsafe { client_close() }
}

#[tauri::command]
fn sign_up(username: String, nickname: String, password: String) -> i32 {
    signup_user(&username, &nickname, &password)
}

#[tauri::command]
fn sign_in(username: String, password: String) -> i32 {
    log_in(&username, &password)
}

// 4. Tauri entrypoint
#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![
            greet,
            connect,
            disconnect,
            sign_up,
            sign_in
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

