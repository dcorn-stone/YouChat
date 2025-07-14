use std::fs;

fn main() {
    tauri_build::build();

    pkg_config::probe_library("jansson").unwrap();
    let mut build = cc::Build::new();

    // Add the include path (for header files)
    build.include("c_inc");
    build.include("/opt/homebrew/include/");

    // Collect all .c files in src directory
    let src_files = fs::read_dir("c_src")
        .expect("Could not read c_src directory")
        .filter_map(|entry| {
            let path = entry.ok()?.path();
            if path.extension()? == "c" {
                Some(path)
            } else {
                None
            }
        });

    // Add each .c file to the compiler
    for file in src_files {
        println!("cargo:rerun-if-changed={}", file.display());
        build.file(file);
    }

    // Compile the C code as a static library
    build.compile("client");

    println!("cargo:rustc-link-lib=jansson");
}

