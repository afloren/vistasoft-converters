cmake_minimum_required(VERSION 2.6)

project(mrVista-converters)

find_path(NIFTI_INCLUDES nifti1_io.h PATH_SUFFIXES nifti)
find_library(LIBNIFTIIO libniftiio.so)
find_library(LIBZNZ libznz.so)
set(NIFTI_LIBRARIES ${LIBNIFTIIO} ${LIBZNZ} CACHE FILEPATH "libnifti libraries.")
set(MATH_LIBRARY m CACHE FILEPATH "default math library.")

find_package(ZLIB REQUIRED)
#find_package(NIFTI REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${ZLIB_INCLUDE_DIR})
include_directories(${NIFTI_INCLUDES})

add_library(grayio STATIC gray_io.c)
add_library(niftiutil STATIC nifti_util.c)

set(LINK_LIBRARIES grayio niftiutil ${MATH_LIBRARY} ${ZLIB_LIBRARY} ${NIFTI_LIBRARIES})

add_executable(nii2dat nii2dat.c)
target_link_libraries(nii2dat ${LINK_LIBRARIES})

add_executable(dat2nii dat2nii.c)
target_link_libraries(dat2nii ${LINK_LIBRARIES})

add_executable(nii2class nii2class.c)
target_link_libraries(nii2class ${LINK_LIBRARIES})

add_executable(class2nii class2nii.c)
target_link_libraries(class2nii ${LINK_LIBRARIES})

#install(FILES gray_io.h nifti_util.h DESTINATION include)
install(TARGETS nii2dat dat2nii nii2class class2nii RUNTIME DESTINATION bin)

