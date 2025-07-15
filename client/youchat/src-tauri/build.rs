use std::{fs, path::Path};

fn main() {

    pkg_config::probe_library("jansson").expect("Failed to find Jansson via pkg-config");

    // Tell Cargo to rerun this script if anything in c_src or c_inc changes
    println!("cargo:rerun-if-changed=c_src");
    println!("cargo:rerun-if-changed=c_inc");

    // Create a new C compiler build
    let mut build = cc::Build::new();

    // Include the header directory
    build.include("c_inc");
    build.include("/opt/homebrew/include/");

    // Iterate over all entries in c_src/
    let src_dir = Path::new("c_src");
    for entry in fs::read_dir(src_dir).expect("Failed to read c_src directory") {
        let entry = entry.expect("Failed to read directory entry");
        let path = entry.path();

        // Only compile files ending with .c
        if let Some(ext) = path.extension() {
            if ext == "c" {
                build.file(path);
            }
        }
    }

    // Compile into a static library named libmy_c_code.a
    build.compile("client");

    println!("cargo:rustc-link-lib=jansson");
    
    tauri_build::build()
}
