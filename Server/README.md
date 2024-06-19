# Go Server for User Management and Messaging

This repository contains a Go-based server implementation for a user management and messaging system. The server listens on port `10086` and interacts with a database specified by the `databaseURL` constant. It provides several endpoints to handle user login, user registration, fetching user lists, sending messages, and retrieving chat history.

## Features

1. **User Login**: Authenticates users based on their username and password.
2. **User Registration**: Allows new users to register with a username and password.
3. **Fetch User List**: Retrieves a list of all registered users.
4. **Send Message**: Allows users to send messages to each other, storing the messages in the database.
5. **Chat History**: Retrieves the chat history between two users and allows clearing the chat history.

## Endpoints

### User Login

- **Method**: POST

- **Endpoint**: `/login`

- **Request Body**:

  ```json
  {
    "username": "user",
    "password": "pass",
    "method": "login"
  }
  ```

- **Response**:

  ```json
  {
    "status": "Login Successful"
  }
  ```

### User Registration

- **Method**: POST

- **Endpoint**: `/signup`

- **Request Body**:

  ```json
  {
    "username": "new_user",
    "password": "new_pass",
    "method": "signup"
  }
  ```

- **Response**:

  ```json
  {
    "message": "Signup successful",
    "username": "new_user"
  }
  ```

### Fetch User List

- **Method**: POST

- **Endpoint**: `/userlist`

- **Request Body**:

  ```json
  {
    "method": "userlist"
  }
  ```

- **Response**:

  ```json
  [
    {"username": "user1"},
    {"username": "user2"},
    ...
  ]
  ```

### Send Message

- **Method**: POST

- **Endpoint**: `/chat`

- **Request Body**:

  ```json
  {
    "usernameA": "sender",
    "usernameB": "receiver",
    "message": "Hello!",
    "method": "chat"
  }
  ```

- **Response**:

  ```json
  {
    "success": true
  }
  ```

### Chat History

- **Method**: POST

- **Endpoint**: `/chathistory`

- **Request Body**:

  ```json
  {
    "usernameA": "user1",
    "usernameB": "user2",
    "method": "chathistory"
  }
  ```

- **Response**:

  ```json
  [
    {"message": "Hello!", "timestamp": "2023-06-19T12:34:56Z"},
    {"message": "Hi!", "timestamp": "2023-06-19T12:35:00Z"},
    ...
  ]
  ```

### Clear Chat History

- **Method**: POST

- **Endpoint**: `/clearhistory`

- **Request Body**:

  ```json
  {
    "usernameA": "user1",
    "usernameB": "user2",
    "method": "clearhistory"
  }
  ```

- **Response**:

  ```json
  {
    "success": true
  }
  ```

## Getting Started

1. **Clone the repository**:

   ```sh
   https://github.com/kechangdev/Easychat.git
cd Easychat/Server
   ```

2. **Install dependencies**:

   ```sh
   go get -u github.com/jinzhu/gorm
   go get -u github.com/jinzhu/gorm/dialects/mysql
   ```

3. **Update `databaseURL`** in the source code with your actual database connection string.

4. **Run the server**:

   ```sh
   go run main.go
   ```

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss any changes.

## License

This project is licensed under the Apache-2.0 License. See the `LICENSE` file for details.
