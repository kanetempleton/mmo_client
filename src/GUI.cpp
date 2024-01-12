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

GUI::~GUI() {
if (clientThread.joinable()) {
        clientThread.join();
        // or clientThread.detach(); if you want to detach the thread
    }
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
    // State::ConnectButton;
    setState(State::ConnectButton);

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
         //   printf("hey\n");
        } else if (currentState == State::Loading) {
          //  printf("current state is loading\n");
            handleEventsGeneral();
            renderLoading();
        } else {
            printf("Invalid state rendering\n");
        }
    }
}

void GUI::setState(State s) {
    currentState = s;
    printf("Changed GUI state: %d\n",s);
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
                   // currentState = State::LoginScreen;
                    setState(State::LoginScreen);

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
void GUI::handleLoginButton(const std::string& user, const std::string& pass) {
    std::printf("Username: %s\n", enteredUsername.c_str());
    std::printf("Password: %s\n", enteredPassword.c_str());
   //currentState = State::Loading;
   // setState(State::Loading);
    infoText = "Loading, please wait...";
    startLoginTimer();
    handleConnection(enteredUsername, enteredPassword);  // Move the connection logic here if needed
}
std::chrono::seconds timerDuration{10};  // Set the timer duration to 10 seconds
bool isTimerActive{false};

void GUI::startLoginTimer() {
    isTimerActive = true;
    std::thread([this]() {
        std::this_thread::sleep_for(timerDuration);
        endLoginTimer();
    }).detach();
}

void GUI::endLoginTimer() {
    isTimerActive = false;
    if (serverResponse.length()<=0) {
        infoText = "No response from login server.";
    }
}
void GUI::handleEventsGeneral() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleMouseLeftClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }
}
void GUI::handleEventsLoginScreen() {
    //printf("handleeventsloginscreen\n");
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                //sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                handleMouseLeftClick(event.mouseButton.x, event.mouseButton.y);
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

void GUI::handleMouseLeftClick(int mousex, int mousey) {
    sf::Vector2f mousePos = sf::Vector2f(mousex, mousey);

    if (cancelButton.getGlobalBounds().contains(mousePos)) {
        handleCancelButton();
    } else if (logInButton.getGlobalBounds().contains(mousePos)) {
        handleLoginButton(enteredUsername, enteredPassword);
    }
}

void GUI::backgroundThread(const std::string& ip, int port, const std::string& usr, const std::string& pass)  {
    Client client(this);
    client.connectToServer(ip, port);
    client.sendData("hello "+usr+" "+pass);
    client.receiveData();
}
void GUI::handleConnection(const std::string& username, const std::string& password) {
    isLoading = true;
    infoText = "Loading, please wait...";
    setState(State::Loading);

    std::string ip = "127.0.0.1";
    int port = 43595; 

    clientThread = std::thread(&GUI::backgroundThread, this, ip, port,username,password);
    //clientThread.join();
}


void GUI::handleCancelButton() {
    if (currentState == State::LoginScreen) {
         setState(State::ConnectButton);
    }
}
void GUI::updateConnectButton() {
    if (currentState == State::ConnectButton) {
        
    }
}

void GUI::updateLoginScreen() {
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
    std::string serverText = "";
    if (serverResponse.length()>0) {
        serverText = ""+serverResponse;
    }
    loadingText.setString(infoText+serverText);
    loadingText.setCharacterSize(24);
    loadingText.setFillColor(sf::Color::White);

    // Calculate the center of the black box
    float centerX = loadingBox.getPosition().x + loadingBox.getSize().x / 2.0f;
    float centerY = loadingBox.getPosition().y + loadingBox.getSize().y / 2.0f;

    // Adjust the position of the text to center it within the black box
    loadingText.setPosition(centerX - loadingText.getLocalBounds().width / 2.0f,
                             centerY - loadingText.getLocalBounds().height / 2.0f);

    window.draw(loadingText);
    window.display();
}



void GUI::renderLoginScreen() {
   // printf("renderloginscreen\n");
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
   // printf("donerenderingloginscreen\n");
}
