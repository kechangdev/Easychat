# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/Easychat_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Easychat_autogen.dir/ParseCache.txt"
  "Easychat_autogen"
  )
endif()
