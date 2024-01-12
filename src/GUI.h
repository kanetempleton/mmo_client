#pragma once

#include <SFML/Graphics.hpp>
#include <thread>

class GUI {
public:
    GUI();
    ~GUI();
    void run();

    //net
    std::string serverResponse;

private:

    //state tracking
    enum class State {
        ConnectButton,
        LoginScreen,
        Loading
    };

    void setState(State s);


    // initialization methods
    void initLoginPanel();
    void initWindow();
    void initGraphics();
    void initFirstState();
    void initFonts();
    void initTextures();

    //io
    void handleEventsGeneral();
    void handleEventsConnectButton();
    void handleEventsLoginScreen();
    void handleMouseLeftClick(int mousex, int mousey);

    

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
    void handleConnectButton();
    void handleCancelButton();
    void handleLoginButton(const std::string& user, const std::string& pass);


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

    //info
    std::string infoText;


    // networking
    std::thread clientThread;
    
    std::mutex responseMutex;
    void backgroundThread(const std::string& ip, int port,const std::string& usr, const std::string& pass);
    void startLoginTimer();
    void endLoginTimer();


    

};
