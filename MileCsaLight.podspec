Pod::Spec.new do |s|

  s.name         = "MileCsaLight"
  s.version      = "0.5"
  s.summary      = "MileCsaLight is a SDK to create Mile Core Wallet"
  s.description  = "MileCsaLight is a SDK helps to generate Mile wallet keys: Public and Private. Also the SDK gives api to sign client transactions."                   

  s.homepage     = "https://mile.global"

  s.license      = { :type => "MIT", :file => "LICENSE" }
  s.authors            = { "denis svinarchuk" => "denn.nevera@gmail.com" }
  s.social_media_url   = "https://mile.global"

  s.platform     = :ios
  s.platform     = :osx

  s.ios.deployment_target = "11.0"
  s.osx.deployment_target = "10.12"
 
  s.swift_version = "4.2"

  s.source       = { :git => "https://bitbucket.org/mile-core/mile-cpp-api", :tag => "#{s.version}" }

  s.source_files  = "platforms/ios/mile-ios-sdk/Classes/*.{h,m,mm}",
                    "src/*.cpp",
                    "include/**/*.hpp",
                    "vendor/nlohmann/*.{hpp}",
                    "vendor/mile-crypto/include/*.{hpp,h}",
                    "vendor/mile-crypto/src/*.{cpp,c}",
                    "vendor/mile-crypto/src/private_include/*.{h,hpp}",
                    "vendor/mile-crypto/src/ed25519/src/*.{c,h}",
                    "vendor/mile-crypto/src/ed25519/include/*.{h}",
  s.exclude_files = "test",
                    "wasm"

  s.public_header_files = "platforms/ios/mile-ios-sdk/Classes/*.{h,hpp}"

  s.frameworks = "Foundation"
  s.libraries  = 'c++'

  s.requires_arc = true
  s.compiler_flags = '-Wno-format', '-x objective-c++', '-DNDEBUG', '-DUSE_DEC_FIXEDPOINT', '-DR128_STDC_ONLY'
  
  s.xcconfig = { 'GCC_PREPROCESSOR_DEFINITIONS' => 'CSA=1' , 'OTHER_CFLAGS' => '',
                 'HEADER_SEARCH_PATHS' => '"/usr/local/include" "${PODS_ROOT}" "${PODS_ROOT}/src" "${PODS_ROOT}/../../../src"  "${PODS_ROOT}/../../mile-cpp-api/src"',
                    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
                    'CLANG_CXX_LIBRARY' => 'libc++'}
  
end
