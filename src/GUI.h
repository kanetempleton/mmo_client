#pragma once

#include <SFML/Graphics.hpp>

class GUI {
public:
    GUI();
    void run();

private:
    enum class State {
        ConnectButton,
        LoginScreen,
        Loading
    };


    // initialization methods
    void initLoginPanel();
    void initWindow();
    void initGraphics();
    void initFirstState();
    void initFonts();
    void initTextures();

    // actions
    void handleEventsConnectButton();
    void handleEventsLoginScreen();

    // updates
    void updateConnectButton();
    void updateLoginScreen();
    void updateServerResponse();
    void updateLoadingScreen();

    // rendering
    void renderConnectButton();
    void renderLoginScreen();
    void renderLoading();
    
    // util
    void handleConnection(const std::string& username, const std::string& password);

    // window
    sf::RenderWindow window;

    // fonts
    sf::Font font;

    // buttons
    sf::Text connectButton;

    // textures
    sf::Texture backgroundTexture;
    sf::Texture texture_greenpink;
    sf::Texture texture_stars1;

    //sprites
    sf::Sprite backgroundSprite;
    sf::Sprite connectButtonSprite;
    sf::Sprite loginButtonSprite;
    sf::Texture cancelButtonTexture;
    sf::Sprite cancelButtonSprite;

    // text
    sf::Text logInButtonText;
    sf::Text cancelButtonText;

    State currentState;

    // Login Screen Components
    sf::RectangleShape usernameField;
    sf::RectangleShape passwordField;
    sf::Text usernameText;
    sf::Text passwordText;
    sf::RectangleShape cancelButton;
    sf::RectangleShape logInButton;

    std::string enteredUsername;
    std::string enteredPassword;
    bool focusUsername;
    bool isLoading;


    // networking
    std::string serverResponse;
    std::mutex responseMutex;
};
