pub struct Greeter {
    greeting: String,
}

impl Greeter {
    pub fn new(greeting: &str) -> Greeter {
        Greeter { greeting: greeting.to_string(), }
    }

    pub fn greet(&self, thing: &str) {
        println!("{} {}", &self.greeting, thing);
    }
}

use std::ffi::CStr;
use std::os::raw::c_char;

#[no_mangle]
pub extern fn log_message(s: *const c_char) {
    
    let raw = unsafe {CStr::from_ptr(s) };

    if let Ok(msg) = raw.to_str() {
        println!("Rust logger: {}", msg);
    } 
}