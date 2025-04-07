#!/bin/bash

# Define the directory where the SQLite source code is located
SQLITE_SOURCE_DIR="/media/victor/New Volume/source/Utils/SQLiteUtils/SQLite3" # Adjust if needed
SRC_DIR="$SQLITE_SOURCE_DIR/src"
INCLUDE_DIR="$SQLITE_SOURCE_DIR" # Assuming sqlite3.h is directly in the source root
INSTALL_PREFIX="/usr/local/sqlite3-fpic"
LIB_DIR="$INSTALL_PREFIX/lib"
INCLUDE_INSTALL_DIR="$INSTALL_PREFIX/include"

# Create install directories if they don't exist
mkdir -p "$LIB_DIR"
mkdir -p "$INCLUDE_INSTALL_DIR"

# Navigate to the source directory
cd "$SRC_DIR" || {
    echo "Error: Could not change directory to $SRC_DIR"
    exit 1
}

# List C source files
SOURCE_FILES=$(find . -name "*.c")

# Compile all .c files with -fPIC and create object files (.o)
echo "Compiling SQLite source files with -fPIC..."
OBJECT_FILES=""
for FILE in $SOURCE_FILES; do
    OBJECT_FILE="$(basename "$FILE" .c).o"
    gcc -fPIC -c "$FILE" -I"$INCLUDE_DIR" -DSQLITE_THREADSAFE=1 -DSQLITE_ENABLE_FTS5 -o "$OBJECT_FILE"
    if [ $? -ne 0 ]; then
        echo "Error compiling $FILE"
        exit 1
    fi
    OBJECT_FILES="$OBJECT_FILES $OBJECT_FILE"
done

# Create a shared library (libsqlite3.so) from the object files
echo "Creating shared library libsqlite3.so..."
gcc -shared -o "$LIB_DIR/libsqlite3.so" $OBJECT_FILES -lpthread -ldl -lm
if [ $? -ne 0 ]; then
    echo "Error creating shared library"
    exit 1
fi

# Create a static library (libsqlite3.a) from the object files
echo "Creating static library libsqlite3.a..."
ar rcs "$LIB_DIR/libsqlite3.a" $OBJECT_FILES
if [ $? -ne 0 ]; then
    echo "Error creating static library"
    exit 1
fi

# Install the header file
echo "Installing header file..."
sudo install "$SQLITE_SOURCE_DIR/sqlite3.h" "$INCLUDE_INSTALL_DIR/"

# Update the shared library cache
echo "Updating shared library cache..."
sudo ldconfig

echo "SQLite built and installed with -fPIC to $INSTALL_PREFIX"
