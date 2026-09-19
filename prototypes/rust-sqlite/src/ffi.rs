//! Experimental C ABI adapter for the HLA-PERSIST storage slice.

use crate::{Event, Store, Stream};
use std::ffi::{CStr, c_char, c_int, c_void};

fn checked_stream(stream: c_int) -> Option<Stream> {
    match stream {
        0 => Some(Stream::Reality),
        1 => Some(Stream::Knowledge),
        _ => None,
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn campistoria_store_new() -> *mut c_void {
    match Store::open(":memory:") {
        Ok(store) => Box::into_raw(Box::new(store)).cast(),
        Err(_) => std::ptr::null_mut(),
    }
}

/// `handle` must be a live pointer returned by `campistoria_store_new`, or null.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn campistoria_store_free(handle: *mut c_void) {
    if !handle.is_null() {
        // SAFETY: The caller must pass an owned, live handle from this library.
        unsafe { drop(Box::from_raw(handle.cast::<Store>())) };
    }
}

/// Both pointers must be live; `event_json` must be NUL-terminated.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn campistoria_store_append(
    handle: *mut c_void,
    stream: c_int,
    event_json: *const c_char,
) -> c_int {
    if handle.is_null() || event_json.is_null() {
        return -1;
    }
    let Some(stream) = checked_stream(stream) else {
        return -1;
    };
    // SAFETY: Pointer validity and string termination are obligations of the C caller.
    let Ok(json) = unsafe { CStr::from_ptr(event_json) }.to_str() else {
        return -1;
    };
    let Ok(event) = serde_json::from_str::<Event>(json) else {
        return -1;
    };
    // SAFETY: The caller must pass a live handle created by this library.
    let store = unsafe { &*handle.cast::<Store>() };
    if store.append(stream, &event).is_ok() {
        0
    } else {
        -1
    }
}

/// `handle` must be a live pointer returned by `campistoria_store_new`.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn campistoria_store_count(handle: *mut c_void, stream: c_int) -> c_int {
    if handle.is_null() {
        return -1;
    }
    let Some(stream) = checked_stream(stream) else {
        return -1;
    };
    // SAFETY: The caller must pass a live handle created by this library.
    let store = unsafe { &*handle.cast::<Store>() };
    match store.events(stream) {
        Ok(events) => c_int::try_from(events.len()).unwrap_or(-1),
        Err(_) => -1,
    }
}
