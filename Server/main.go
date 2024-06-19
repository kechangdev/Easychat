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
    // username:password@tcp(127.0.0.1:3306)/dbname?charset=utf8mb4&parseTime=True&loc=Local
    databaseURL := os.Getenv("DATABASE_URL")
    if databaseURL == "" {
        log.Fatalf("DATABASE_URL environment variable not set")
    }
    db, err = gorm.Open(mysql.Open(databaseURL), &gorm.Config{})
    if err != nil {
        log.Fatalf("failed to connect to database: %v", err)
    }

    // Automatically migrate your schema, to keep your schema up to date.
    db.AutoMigrate(&User{}, &Message{})
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
    http.HandleFunc("/api", apiHandler)
    log.Println("Starting server on port 10086...")
    log.Fatal(http.ListenAndServe(":10086", nil))
}

func apiHandler(w http.ResponseWriter, r *http.Request) {
    var req map[string]string
    if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
        http.Error(w, err.Error(), http.StatusBadRequest)
        return
    }

    method := req["method"]
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
        http.Error(w, "Unknown method", http.StatusBadRequest)
    }
}

func handleLogin(w http.ResponseWriter, req map[string]string) {
    var user User
    if err := db.Where("username = ? AND password = ?", req["username"], req["password"]).First(&user).Error; err != nil {
        json.NewEncoder(w).Encode(map[string]string{"Status": "Login Failed"})
        return
    }
    json.NewEncoder(w).Encode(map[string]string{"Status": "Login Successful"})
}

func handleSignup(w http.ResponseWriter, req map[string]string) {
    user := User{Username: req["username"], Password: req["password"]}
    if err := db.Create(&user).Error; err != nil {
        json.NewEncoder(w).Encode(map[string]string{"message": "Signup failed"})
        return
    }
    json.NewEncoder(w).Encode(map[string]string{"message": "Signup successful", "username": user.Username})
}

func handleUserList(w http.ResponseWriter) {
    var users []User
    if err := db.Find(&users).Error; err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    var usernames []string
    for _, user := range users {
        usernames = append(usernames, user.Username)
    }
    json.NewEncoder(w).Encode(usernames)
}

func handleChat(w http.ResponseWriter, req map[string]string) {
    message := Message{
        UsernameA: req["usernameA"],
        UsernameB: req["usernameB"],
        Message:   req["message"],
        Timestamp: time.Now(),
    }
    if err := db.Create(&message).Error; err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    json.NewEncoder(w).Encode(map[string]bool{"success": true})
}

func handleChatHistory(w http.ResponseWriter, req map[string]string) {
    var messages []Message
    if err := db.Where("username_a = ? AND username_b = ?", req["usernameA"], req["usernameB"]).Find(&messages).Error; err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    json.NewEncoder(w).Encode(messages)
}

func handleClearHistory(w http.ResponseWriter, req map[string]string) {
    if err := db.Where("username_a = ? AND username_b = ?", req["usernameA"], req["usernameB"]).Delete(Message{}).Error; err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }
    json.NewEncoder(w).Encode(map[string]bool{"success": true})
}
