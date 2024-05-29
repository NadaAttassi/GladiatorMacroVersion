cmake_minimum_required(VERSION  3.20)
project(test_GTK C)

set(CMAKE_C_STANDARD 99)

# On utilise le module PkgConfig pour détecter la librairie GTK+ sur le système
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# On dit à CMake d'utiliser GTK+, on indique au compilateur où trouver les fichiers headers
# Et au linker où trouver les librairies
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Instructions pour le compilateur
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

find_package(LibXml2 REQUIRED)
add_compile_definitions(test_GTK PUBLIC ${LIBXML2_DEFINITIONS})
include_directories(PUBLIC ${LIBXML2_INCLUDE_DIRS})
link_libraries(${LIBXML2_LIBRARIES})


##Pour linux, va servir à correctement liers les signaux de notre application au code
#if (UNIX)
#    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-export-dynamic")
#endif (UNIX)

# Ajoute un éxecutable à partir du main.c
add_executable(test_GTK
        main.c
        arena.c
        gtk_utils.c
        createCharacter.h
)

# Lie à l'éxecutable la librairie GTK+
TARGET_LINK_LIBRARIES(test_GTK ${GTK3_LIBRARIES})

target_link_libraries(test_GTK winmm)