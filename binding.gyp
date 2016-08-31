{
    'targets': [
        {
            'target_name': 'win-wavplay',
            'sources': [
                './src/main.cc',
                './src/win32/WinPlayer.cc',
                './src/ext/gwutils/Flags.cpp',
                './src/ext/gwutils/Logger.cpp',
                './src/ext/gwutils/ThreadSafe.cpp',
                './src/ext/gwutils/CharsetUtil.cpp',
            ],

            'include_dirs': [
                './src',
                './src/win32',
                './src/gwutils'
            ],

            'conditions': [
                ['OS == "win"', {
                    'defines': [
                        'UNICODE',
                    ],
                    'libraries': ['winmm.lib'],
                    'configurations' : {
                        'Debug' : {
                            'msvs_settings': {
                                'VCCLCompilerTool': {
                                    'RuntimeLibrary': '3' # /MDd
                                },
                                'VCLinkerTool': {
                                    'AdditionalOptions': ['/ignore:4099'],
                                    'IgnoreDefaultLibraryNames': ['libcmtd.lib']
                                }
                            }
                        },
                        'Release' : {
                            'msvs_settings': {
                                'VCCLCompilerTool': {
                                    'RuntimeTypeInfo': 'true', # To disable '/GR-'
                                    'RuntimeLibrary': '2' # /MDd
                                },
                                'VCLinkerTool': {
                                    'AdditionalOptions': ['/ignore:4099'],
                                    'IgnoreDefaultLibraryNames': ['libcmt.lib']
                                }
                            }
                        }
                    }
                }],
                ['OS == "mac"', {
                    'xcode_settings': {
                        'GCC_ENABLE_CPP_RTTI': 'YES',
                        "OTHER_CFLAGS": [
                            "-D__MAC_OS__ -DOSX"
                        ]
                    },
                    'libraries': []
                }]
            ]
        }
    ]
}
