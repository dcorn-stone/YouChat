// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

mod ffi;

use ffi::*;

#[tauri::command]
fn client_connect() -> i32 {
    connect_to_server()
}

#[tauri::command]
fn client_close(){
    disconnect_from_server()
}

#[tauri::command]
fn signup(username: String, nickname: String, password: String) -> i32 {
    signup_user(&username, &nickname, &password)
}


// fn main() {
//   tauri::Builder::default()
//     .invoke_handler(tauri::generate_handler![process_text])
//     .run(tauri::generate_context!())
//     .expect("error while running tauri");
// }

fn main() {
    tauri::Builder::default()
    .invoke_handler(tauri::generate_handler![
        client_connect,
        client_close,
        signup,
    ])
    .run(tauri::generate_context!())
    .expect("error while running tauri");

}
