option (ENABLE_AZURE_BLOB_STORAGE "Enable Azure blob storage" ${ENABLE_LIBRARIES})

if (NOT ENABLE_AZURE_BLOB_STORAGE OR OS_FREEBSD)
    message(STATUS "Not using Azure blob storage")
    return()
endif()

set(AZURE_DIR "${ClickHouse_SOURCE_DIR}/contrib/azure")
set(AZURE_SDK_LIBRARY_DIR "${AZURE_DIR}/sdk")

file(GLOB AZURE_SDK_SRC
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/src/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/src/credentials/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/src/cryptography/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/src/http/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/src/http/curl/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/src/io/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/src/tracing/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/identity/azure-identity/src/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/storage/azure-storage-blobs/src/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/storage/azure-storage-blobs/src/private/*.cpp"
    "${AZURE_SDK_LIBRARY_DIR}/storage/azure-storage-common/src/*.cpp"
)

set(AZURE_SDK_INCLUDES
    "${AZURE_SDK_LIBRARY_DIR}/core/azure-core/inc/"
    "${AZURE_SDK_LIBRARY_DIR}/identity/azure-identity/inc/"
    "${AZURE_SDK_LIBRARY_DIR}/storage/azure-storage-common/inc/"
    "${AZURE_SDK_LIBRARY_DIR}/storage/azure-storage-blobs/inc/"
)

if (CLICKHOUSE_CLOUD)
    list(APPEND AZURE_SDK_SRC
        "${AZURE_SDK_LIBRARY_DIR}/keyvault/azure-security-keyvault-keys/src/*.cpp"
        "${AZURE_SDK_LIBRARY_DIR}/keyvault/azure-security-keyvault-keys/src/cryptography/*.cpp"
    )
    list(APPEND AZURE_SDK_INCLUDES
        "${AZURE_SDK_LIBRARY_DIR}/keyvault/azure-security-keyvault-keys/inc/"
        "${AZURE_SDK_LIBRARY_DIR}/keyvault/azure-security-keyvault-shared/inc/"
    )
endif()

file(GLOB AZURE_SDK_UNIFIED_SRC
    ${AZURE_SDK_SRC}
)

add_library(_azure_sdk ${AZURE_SDK_UNIFIED_SRC})
target_compile_definitions(_azure_sdk PRIVATE BUILD_CURL_HTTP_TRANSPORT_ADAPTER)

# Originally, on Windows azure-core is built with bcrypt and crypt32 by default
if (TARGET OpenSSL::SSL)
    target_link_libraries(_azure_sdk PRIVATE OpenSSL::Crypto OpenSSL::SSL)
endif()

# Originally, on Windows azure-core is built with winhttp by default
if (TARGET ch_contrib::curl)
    target_link_libraries(_azure_sdk PRIVATE ch_contrib::curl)
endif()

target_link_libraries(_azure_sdk PRIVATE ch_contrib::libxml2)

target_include_directories(_azure_sdk SYSTEM BEFORE PUBLIC ${AZURE_SDK_INCLUDES})

add_library(ch_contrib::azure_sdk ALIAS _azure_sdk)
