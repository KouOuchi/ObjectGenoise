include_external_msproject(
    test-og-net ${CMAKE_CURRENT_SOURCE_DIR}/test-og.net.csproj
    TYPE 3AC096D0-A1C2-E12C-1390-A8335801FDAB TYPE FAE04EC0-301F-11D3-BF4B-00C04F79EFBC
    GUID 6DCCA72A-98FC-43A9-BDFD-8DA3EE9801DA
)

add_dependencies(test-og-net og-net)
