from sys import platform
import os

if platform == 'win32':
    this_path = os.path.dirname(__file__)
    qt6_path = os.path.abspath(os.path.join(this_path, '..', 'PyQt6', 'Qt6', 'bin'))

    # Ensure the DLLs are loaded correctly
    os.add_dll_directory(qt6_path)

# importing vclib modules here

