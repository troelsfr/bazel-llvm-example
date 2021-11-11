use std::ffi::CStr;
use std::os::raw::c_char;

#[no_mangle]
pub extern fn log_message(s: *const c_char) {
    
    let raw = unsafe {CStr::from_ptr(s) };

    if let Ok(msg) = raw.to_str() {
        println!("Rust logger: {}", msg);
    } 
}