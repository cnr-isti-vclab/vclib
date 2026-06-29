// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/system/native_window_handle.h>

#import <Cocoa/Cocoa.h>

namespace vcl::detail {

void* cretateCocoaWindow(
    const char* title,
    int         width,
    int         height,
    bool        hidden)
{
    NSRect rect = NSMakeRect(0, 0, width, height);
    NSWindow* window = [
        [NSWindow alloc]
        initWithContentRect:rect
                  styleMask:0
                    backing:NSBackingStoreBuffered defer:NO
    ];
    NSString* appName = [NSString stringWithUTF8String: title];
    [window setTitle:appName];
    [window setBackgroundColor:[NSColor blackColor]];
    return (void*) window;
}

void closeCocoaWindow(void* window)
{
    [(NSWindow*) window close];
}

} // namespace vcl::detail
