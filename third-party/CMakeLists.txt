message("- [json]")
add_subdirectory(json)

message("- [toml]")

# some checkups and settings
message("- [opts]")
add_subdirectory(opts)

message("- [fmt]")
add_subdirectory(fmt)

message("- [spdlog]")
set(SPDLOG_FMT_EXTERNAL ON)
set(SPDLOG_NO_EXCEPTIONS ON)
add_subdirectory(spdlog)

message("- [http]")
set(HTTPLIB_COMPILE ON)
add_subdirectory(http)
