/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
