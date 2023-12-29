
# MMO Game Client

MMO game client over TCP. Written in C++ using SFML for 2d graphics rendering. Communication protocol is synced with a Go server which runs the game.


## Dependencies

Before you begin, ensure you have the following prerequisites installed on your system:

For Running:
- Git

For Developing:
- C++ compiler (e.g., g++)
- SFML library (NOTE: change the SFML location in build.sh if it does not match your install location)

## Getting Started

1. Clone the repository:

    ```bash
    git clone https://github.com/kanetempleton/mmo_client.git
    cd mmo_client
    ```

2. (Optional) Run the build script to compile the source code:

    ```bash
    chmod +x src/build.sh
    ./src/build.sh
    ```

    This script will compile the C++ source code using the necessary flags and link the SFML library.

3. Run the compiled executable:

    ```bash
    ./run.sh
    ```

