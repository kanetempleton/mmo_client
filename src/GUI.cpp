#include "GUI.h"
#include "Client.h"  // Add this line
#include <cstdio>
#include <thread>
#include <mutex>

GUI::GUI() {
    
    initWindow();
    initGraphics();
    initFirstState();
    initLoginPanel();

}

void GUI::initWindow() {
    window.create(sf::VideoMode(800, 600), "SFML GUI Example");
}

void GUI::initGraphics() {
    initFonts();
    initTextures();
}

void GUI::initTextures() {
    // Load background texture
    if (!backgroundTexture.loadFromFile("images/background.jpg")) {
        std::printf("Error: Cannot load background image\n");
    }
    if (!texture_greenpink.loadFromFile("images/sprites/buttons/connect_button.jpg")) {
        std::printf("Error: Cannot load button image\n");
    }
    if (!texture_stars1.loadFromFile("images/textures/stars1.jpg")) {
        std::printf("Error: Cannot load Log In button image\n");
    }
}

void GUI::initFonts() {
    if (!font.loadFromFile("arial.ttf")) {
        std::printf("Error: Cannot load font\n");
    }
}

void GUI::initFirstState() {
    currentState = State::ConnectButton;

    // Set up the sprite for the "Connect" button
    connectButtonSprite.setTexture(texture_greenpink);
    connectButtonSprite.setPosition(300, 250);

    // Set up the "Connect" text
    connectButton.setFont(font);
    connectButton.setString("Connect");
    connectButton.setCharacterSize(24);
    connectButton.setFillColor(sf::Color::White);

    // Set up the background sprite
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );
}

void GUI::initLoginPanel() {
        // Login Screen Components
    usernameField.setSize(sf::Vector2f(200, 30));
    usernameField.setPosition(300, 200);
    usernameField.setFillColor(sf::Color::White);

    passwordField.setSize(sf::Vector2f(200, 30));
    passwordField.setPosition(300, 250);
    passwordField.setFillColor(sf::Color::White);


    cancelButton.setSize(sf::Vector2f(80, 30));
    cancelButton.setPosition(400, 300);  // Move to the right
    cancelButton.setFillColor(sf::Color::White);

    logInButton.setSize(sf::Vector2f(80, 30));
    logInButton.setPosition(300, 300);  // Move to the left
    logInButton.setFillColor(sf::Color::White);

    

    // Set up the sprite for the "Log In" button
    loginButtonSprite.setTexture(texture_stars1);
    loginButtonSprite.setPosition(300, 300);  // Adjust position as needed

    // Set up the sprite for the "Cancel" button
    cancelButtonSprite.setTexture(texture_stars1);
    cancelButtonSprite.setPosition(400, 300);  // Adjust position as needed

    // Get the dimensions of the buttons
    float buttonWidth = logInButton.getSize().x;
    float buttonHeight = logInButton.getSize().y;

    // Set up the text for the "Log In" button
    logInButtonText.setFont(font);
    logInButtonText.setString("Log In");
    logInButtonText.setCharacterSize(20);
    logInButtonText.setFillColor(sf::Color::Yellow);
    logInButtonText.setPosition(300 + buttonWidth / 2.0f - logInButtonText.getLocalBounds().width / 2.0f,
                                300 + buttonHeight / 2.0f - logInButtonText.getLocalBounds().height / 2.0f);

    // Set up the text for the "Cancel" button
    cancelButtonText.setFont(font);
    cancelButtonText.setString("Cancel");
    cancelButtonText.setCharacterSize(20);
    cancelButtonText.setFillColor(sf::Color::Yellow);
    cancelButtonText.setPosition(400 + buttonWidth / 2.0f - cancelButtonText.getLocalBounds().width / 2.0f,
                                300 + buttonHeight / 2.0f - cancelButtonText.getLocalBounds().height / 2.0f);

    loginButtonSprite.setScale(buttonWidth / texture_stars1.getSize().x, buttonHeight / texture_stars1.getSize().y);
    cancelButtonSprite.setScale(buttonWidth / texture_stars1.getSize().x, buttonHeight / texture_stars1.getSize().y);

    usernameText.setFont(font);
    usernameText.setCharacterSize(20);
    usernameText.setFillColor(sf::Color::Black);
    usernameText.setPosition(310, 203);

    passwordText.setFont(font);
    passwordText.setCharacterSize(20);
    passwordText.setFillColor(sf::Color::Black);
    passwordText.setPosition(310, 253);

    // Set focus to the username field initially
    focusUsername = true;


}

// Add a new State for Loading
enum class State {
    ConnectButton,
    LoginScreen,
    Loading
};

// Add a variable to store the server response
std::string serverResponse;

// Add a variable to store loading state
bool isLoading;


void GUI::run() {
    while (window.isOpen()) {
        if (currentState == State::ConnectButton) {
            handleEventsConnectButton();
            updateConnectButton();
            renderConnectButton();
        } else if (currentState == State::LoginScreen) {
            handleEventsLoginScreen();
            updateLoginScreen();
            renderLoginScreen();
        } else if (currentState == State::Loading) {
            renderLoading();
        }
    }
}



void GUI::handleEventsConnectButton() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

                if (connectButtonSprite.getGlobalBounds().contains(mousePos)) {
                    // Connect button clicked, switch to LoginScreen state
                    currentState = State::LoginScreen;

                    // Clear entered username and password
                    enteredUsername.clear();
                    enteredPassword.clear();

                    // Clear displayed text
                    usernameText.setString("");
                    passwordText.setString("");

                    // Set focus to the username field
                    focusUsername = true;
                }
            }
        }
    }
}



void GUI::handleEventsLoginScreen() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

                if (cancelButton.getGlobalBounds().contains(mousePos)) {
                    // Cancel button clicked, return to ConnectButton state
                    currentState = State::ConnectButton;
                } else if (logInButton.getGlobalBounds().contains(mousePos)) {
                    // Log In button clicked, print values and return to ConnectButton state
                    std::printf("Username: %s\n", enteredUsername.c_str());
                    std::printf("Password: %s\n", enteredPassword.c_str());
                    currentState = State::ConnectButton;

                    // Start a new thread to handle the connection
                    std::thread connectionThread(&GUI::handleConnection, this, enteredUsername, enteredPassword);
                    connectionThread.detach();  // Detach the thread to let it run independently
                    currentState = State::ConnectButton;
                }
            }
        } else if (event.type == sf::Event::TextEntered) {
            // Handle text input for username and password fields
            if (focusUsername) {
                if (event.text.unicode == 9) {  // Tab key pressed
                    focusUsername = false;
                } else if (event.text.unicode == 8 && !enteredUsername.empty()) {  // Backspace key pressed
                    enteredUsername.pop_back();
                } else if (event.text.unicode >= 32 && event.text.unicode < 127) { 
                    enteredUsername += static_cast<char>(event.text.unicode);
                }
                usernameText.setString(enteredUsername);
            } else {
                if (event.text.unicode == 9) {  // Tab key pressed
                    focusUsername = true;
                } else if (event.text.unicode == 8 && !enteredPassword.empty()) {  // Backspace key pressed
                    enteredPassword.pop_back();
                } else if (event.text.unicode >= 32 && event.text.unicode < 127) {  // Printable ASCII characters
                    enteredPassword += static_cast<char>(event.text.unicode);
                }
                passwordText.setString(std::string(enteredPassword.size(), '*'));  // Mask the password
            }
        }
    }
}

void GUI::handleConnection(const std::string& username, const std::string& password) {
    isLoading = true;
    currentState = State::Loading;

    serverResponse = "Loading, please wait...";

    // Create an instance of the Client class
    Client client;

    // Set the callback to update the loading text
    client.setOnDataReceivedCallback([this](const std::string& receivedData) {
        // Use a mutex to synchronize access to serverResponse
        std::lock_guard<std::mutex> lock(responseMutex);
        serverResponse = receivedData;
    });

    // Connect to the server on port 43595
    if (client.connectToServer("127.0.0.1", 43595)) {
        // Send the message "hello $username $password" to the server
        std::string message = "hello " + username + " " + password;
        client.sendData(message);

        // Start a new thread to handle data receiving
        std::thread receiveThread([&client, this]() {
            // Receive data from the server
            client.receiveData();
        });

        // Wait for the receive thread to finish
        receiveThread.join();
    }

    {
        // Update the GUI in the main thread
        std::lock_guard<std::mutex> lock(responseMutex);
        isLoading = false;
        currentState = State::ConnectButton;
    }
}





void GUI::updateConnectButton() {
    // Add any logic for the ConnectButton state here
}

void GUI::updateLoginScreen() {
    // Add any logic for the LoginScreen state here
}

void GUI::renderConnectButton() {
    window.clear();

    // Draw the background sprite
    window.draw(backgroundSprite);

    // Draw the "Connect" button sprite
    window.draw(connectButtonSprite);

    // Adjust the position of the "Connect" text to be centered on the button
    sf::FloatRect textBounds = connectButton.getLocalBounds();
    connectButton.setOrigin(textBounds.left + textBounds.width / 2.0f,
                            textBounds.top + textBounds.height / 2.0f);
    connectButton.setPosition(connectButtonSprite.getPosition().x + connectButtonSprite.getLocalBounds().width / 2.0f,
                              connectButtonSprite.getPosition().y + connectButtonSprite.getLocalBounds().height / 2.0f);

    // Draw the "Connect" text
    window.draw(connectButton);

    window.display();
}

void GUI::renderLoading() {

    window.clear();

    // Draw the background sprite
    window.draw(backgroundSprite);

    // Create a black box in the center of the screen
    sf::RectangleShape loadingBox(sf::Vector2f(400, 200));
    loadingBox.setPosition(200, 200);
    loadingBox.setFillColor(sf::Color::Black);
    window.draw(loadingBox);

    // Display the server response in the black box
    sf::Text loadingText;
    loadingText.setFont(font);
    loadingText.setString(serverResponse);
    loadingText.setCharacterSize(24);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setPosition(300 - loadingText.getLocalBounds().width / 2.0f,
                            300 - loadingText.getLocalBounds().height / 2.0f);
    window.draw(loadingText);

    window.display();
}


void GUI::renderLoginScreen() {
    window.clear();

    // Draw the background sprite
    window.draw(backgroundSprite);

    // Draw the login screen components
    window.draw(usernameField);
    window.draw(passwordField);

    // Draw the "Log In" button texture
    window.draw(loginButtonSprite);
    

    //login panel
    window.draw(cancelButtonSprite);
    window.draw(logInButtonText);
    window.draw(cancelButtonText);
    window.draw(usernameText);
    window.draw(passwordText);

    window.display();
}
