project(
    'vulkan',
    'cpp',
    version : '1.0.0',
    default_options : [
        'buildtype=debug',
        'optimization=0',
        'cpp_std=c++17'
    ],
    meson_version: '>= 0.60.0',
)

vulkan = dependency('vulkan')

executable('triangle', 'triangle/main.cpp', dependencies: vulkan)

executable('vulkan',
           'main.cpp'
)

