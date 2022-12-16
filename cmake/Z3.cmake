# warning, only tested with: z3-4.11.2
function(get_z3_zip_file vnum)
    message(STATUS "Looking for Z3 release for platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
    set(z3vstr "gh:Z3Prover/z3#${vnum}" PARENT_SCOPE)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(z3vstr "https://github.com/Z3Prover/z3/releases/download/${vnum}/${vnum}-glibc-2.31.zip" PARENT_SCOPE)
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")
            set(z3vstr "https://github.com/Z3Prover/z3/releases/download/${vnum}/${vnum}-x64-win.zip" PARENT_SCOPE)
        elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
            set(z3vstr "https://github.com/Z3Prover/z3/releases/download/${vnum}/${vnum}-x86-win.zip" PARENT_SCOPE)
        endif()
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")
            set(z3vstr "https://github.com/Z3Prover/z3/releases/download/${vnum}/${vnum}-x64-osx-10.16.zip" PARENT_SCOPE)
        elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            set(z3vstr "https://github.com/Z3Prover/z3/releases/download/${vnum}/${vnum}-arm64-osx-11.0.zip" PARENT_SCOPE)
        endif()
    endif()
endfunction()
