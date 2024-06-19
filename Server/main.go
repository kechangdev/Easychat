package main

import (
    "encoding/json"
    "log"
    "net/http"
    "os"
    "time"

    "gorm.io/driver/mysql"
    "gorm.io/gorm"
)

var db *gorm.DB

func init() {
    var err error
    databaseURL := os.Getenv("DATABASE_URL")
    if databaseURL == "" {
        log.Fatalf("DATABASE_URL environment variable not set")
    }
    log.Printf("Connecting to database with URL: %s", databaseURL)
    db, err = gorm.Open(mysql.Open(databaseURL), &gorm.Config{})
    if err != nil {
        log.Fatalf("Failed to connect to database: %v", err)
    }
    log.Println("Database connection established")

    // Automatically migrate your schema, to keep your schema up to date.
    log.Println("Starting database migrations")
    if err := db.AutoMigrate(&User{}, &Message{}); err != nil {
        log.Fatalf("Failed to migrate database: %v", err)
    }
    log.Println("Database migration completed")
}

type User struct {
    ID        uint       `gorm:"primary_key" json:"id"`
    CreatedAt time.Time  `json:"created_at"`
    UpdatedAt time.Time  `json:"updated_at"`
    DeletedAt *time.Time `gorm:"index" json:"deleted_at,omitempty"`
    Username  string     `gorm:"unique;not null" json:"username"`
    Password  string     `gorm:"not null" json:"password"`
}

type Message struct {
    ID        uint       `gorm:"primary_key" json:"id"`
    CreatedAt time.Time  `json:"created_at"`
    UpdatedAt time.Time  `json:"updated_at"`
    DeletedAt *time.Time `gorm:"index" json:"deleted_at,omitempty"`
    UsernameA string     `gorm:"not null" json:"usernameA"`
    UsernameB string     `gorm:"not null" json:"usernameB"`
    Message   string     `gorm:"not null" json:"message"`
    Timestamp time.Time  `json:"timestamp"`
}

func main() {
    log.Println("Starting server on port 10086...")
    http.HandleFunc("/api", apiHandler)
    log.Fatal(http.ListenAndServe(":10086", nil))
}

func apiHandler(w http.ResponseWriter, r *http.Request) {
    log.Println("Received API request")
    var req map[string]string
    if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
        log.Printf("Failed to decode request body: %v", err)
        http.Error(w, err.Error(), http.StatusBadRequest)
        return
    }
    log.Printf("Request body: %v", req)

    method := req["method"]
    log.Printf("Request method: %s", method)
    switch method {
    case "login":
        handleLogin(w, req)
    case "signup":
        handleSignup(w, req)
    case "userlist":
        handleUserList(w)
    case "chat":
        handleChat(w, req)
    case "chathistory":
        handleChatHistory(w, req)
    case "clearhistory":
        handleClearHistory(w, req)
    default:
        log.Printf("Unknown method: %s", method)
        http.Error(w, "Unknown method", http.StatusBadRequest)
    }
}

func handleLogin(w http.ResponseWriter, req map[string]string) {
    log.Println("Handling login request")
    var user User
    if err := db.Where("username = ? AND password = ?", req["username"], req["password"]).First(&user).Error; err != nil {
        log.Printf("Login failed for user: %s", req["username"])
        json.NewEncoder(w).Encode(map[string]string{"Status": "Login Failed"})
        return
    }
    log.Printf("Login successful for user: %s", req["username"])
    json.NewEncoder(w).Encode(map[string]string{"Status": "Login Successful"})
}

func handleSignup(w http.ResponseWriter, req map[string]string) {
    log.Println("Handling signup request")
    user := User{Username: req["username"], Password: req["password"]}
    if err := db.Create(&user).Error; err != nil {
        log.Printf("Signup failed for user: %s", req["username"])
        json.NewEncoder(w).Encode(map[string]string{"message": "Signup failed"})
        return
    }
    log.Printf("Signup successful for user: %s", user.Username)
    json.NewEncoder(w).Encode(map[string]string{"message": "Signup successful", "username": user.Username})
}

func handleUserList(w http.ResponseWriter) {
    var users []User
    if err := db.Find(&users).Error; err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    var userObjects []map[string]string
    for _, user := range users {
        userObject := map[string]string{"username": user.Username}
        userObjects = append(userObjects, userObject)
    }
    if err := json.NewEncoder(w).Encode(userObjects); err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
    }
}

func handleChat(w http.ResponseWriter, req map[string]string) {
    log.Println("Handling chat request")
    message := Message{
        UsernameA: req["usernameA"],
        UsernameB: req["usernameB"],
        Message:   req["message"],
        Timestamp: time.Now(),
    }
    if err := db.Create(&message).Error; err != nil {
        log.Printf("Failed to save message from %s to %s: %v", req["usernameA"], req["usernameB"], err)
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    log.Printf("Message from %s to %s saved successfully", req["usernameA"], req["usernameB"])
    json.NewEncoder(w).Encode(map[string]bool{"success": true})
}

func handleChatHistory(w http.ResponseWriter, req map[string]string) {
    log.Println("Handling chat history request")
    var messages []Message
    if err := db.Where("username_a = ? AND username_b = ?", req["usernameA"], req["usernameB"]).Find(&messages).Error; err != nil {
        log.Printf("Failed to retrieve chat history between %s and %s: %v", req["usernameA"], req["usernameB"], err)
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    log.Printf("Retrieved chat history between %s and %s: %v", req["usernameA"], req["usernameB"], messages)
    json.NewEncoder(w).Encode(messages)
}

func handleClearHistory(w http.ResponseWriter, req map[string]string) {
    log.Println("Handling clear chat history request")
    if err := db.Where("username_a = ? AND username_b = ?", req["usernameA"], req["usernameB"]).Delete(&Message{}).Error; err != nil {
        log.Printf("Failed to clear chat history between %s and %s: %v", req["usernameA"], req["usernameB"], err)
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    log.Printf("Cleared chat history between %s and %s", req["usernameA"], req["usernameB"])
    json.NewEncoder(w).Encode(map[string]bool{"success": true})
}
