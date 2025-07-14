use std::ffi::CString;
use std::os::raw::c_char;

#[link(name = "client")]
extern "C" {
// match your C function signature
    pub fn client_connect() -> i32;
    pub fn client_close();
    pub fn signup(username: *const c_char, nickname: *const c_char, password: *const c_char) -> i32;
}

pub fn connect_to_server() -> i32 {
    unsafe { client_connect() }
}

pub fn disconnect_from_server() {
    unsafe { client_close() }
}

pub fn signup_user(username: &str, nickname: &str, password: &str) -> i32 {
    let c_username = CString::new(username).unwrap();
    let c_nickname = CString::new(nickname).unwrap();
    let c_password = CString::new(password).unwrap();

    unsafe { signup(c_username.as_ptr(), c_nickname.as_ptr(), c_password.as_ptr()) }
}

