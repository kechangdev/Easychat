# EasyChat Client

This repository contains the client-side implementation of EasyChat, a user management and messaging application. The client is built using C++ and Qt, providing a user-friendly interface for users to log in, register, view user lists, send messages, and manage chat history. This README outlines the various interfaces and provides the necessary setup instructions.

## Features

1. **User Login**: Authenticate users based on their username and password.
2. **User Registration**: Allow new users to register with a username and password.
3. **Fetch User List**: Retrieve a list of all registered users.
4. **Send Message**: Send messages to other users and store the messages in the database.
5. **Chat History**: Retrieve and clear the chat history between two users.

## Interfaces

### Interface 1 - User Login

The client sends a POST request to the server with the following JSON payload:

```json
{
  "username": "user",
  "password": "pass",
  "method": "login"
}
```

The server responds with a JSON message indicating whether the login was successful.

### Interface 2 - User Registration

The client sends a POST request to the server with the following JSON payload:

```json
{
  "username": "new_user",
  "password": "new_pass",
  "method": "signup"
}
```

The server responds with a JSON message indicating whether the signup was successful.

### Interface 3 - Fetch User List

The client sends a POST request to the server with the following JSON payload:

```json
{
  "method": "userlist"
}
```

The server responds with a JSON array containing the list of registered users.

### Interface 4 - Send Message

The client sends a POST request to the server with the following JSON payload:

```json
{
  "usernameA": "sender",
  "usernameB": "receiver",
  "message": "Hello!",
  "method": "chat"
}
```

The server responds with a JSON message indicating whether the message was successfully received.

### Interface 5 - Chat History

The client sends a POST request to the server with the following JSON payload:

```json
{
  "usernameA": "user1",
  "usernameB": "user2",
  "method": "chathistory"
}
```

The server responds with a JSON array containing the chat history between the two users.

### Interface 6 - Clear Chat History

The client sends a POST request to the server with the following JSON payload:

```json
{
  "usernameA": "user1",
  "usernameB": "user2",
  "method": "clearhistory"
}
```

The server responds with a JSON message indicating whether the chat history was successfully cleared.

## Setup Instructions

1. **Clone the repository**:
   ```sh
   git clone https://github.com/your-username/easychat-client.git
   cd easychat-client
   ```

2. **Ensure you have Qt installed**. You can download and install Qt from [Qt's official website](https://www.qt.io/download).

3. **Configure the project using CMake**:
   ```sh
   mkdir build
   cd build
   cmake ..
   ```

4. **Build the project**:
   ```sh
   make
   ```

5. **Run the application**:
   ```sh
   ./Easychat
   ```

## CMakeLists.txt

Ensure your `CMakeLists.txt` file is set up correctly to include Qt components:

```cmake
cmake_minimum_required(VERSION 3.5)

project(Easychat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        login_window.cpp
        login_window.h
        login_window.ui
        chatwindow.cpp
        chatwindow.h
        chatwindow.ui
        signupwindow.cpp
        signupwindow.h
        signupwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Easychat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        signupwindow.h signupwindow.cpp signupwindow.ui
        chatwindow.h chatwindow.cpp chatwindow.ui
        userlist.h userlist.cpp userlist.ui
        notification.h notification.cpp notification.ui
    )
else()
    if(ANDROID)
        add_library(Easychat SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Easychat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Easychat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Easychat)
endif()
set_target_properties(Easychat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Easychat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Easychat)
endif()
```

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss any changes.

## License

This project is licensed under the Apache-2.0 License. See the `LICENSE` file for details.
