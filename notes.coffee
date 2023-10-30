games ideas 

BREAKOUT 
ARKANOID

COMPILE - WINDOWS

g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

vivi_@LAPTOP-8BV53D7K MINGW64 - FOLDER

não esquecer de inicializar o xlaunch

###############################################################################################################

###############################################################################################################

###############################################################################################################

###############################################################################################################
SNAKE

DATA: 29/10/2023

    - Run: g++ -o main.exe main.cpp -static -std=c++17

    - Based on https://www.youtube.com/watch?v=e8lYLYlrGLg - for learning purposes;

    - Thanks to https://github.com/OneLoneCoder/Javidx9/blob/master/SimplyCode/OneLoneCoder_Snake.cpp

    - TODO:
        - enhance the commands to move (put 4 options) -> enhancement
        - add play/pause/game over functionalities -> enhancemente
        - add options with walls -> enhancement
        - work on different levels -> enhancement
        - input logic has a bug (commands are not ok) -> fix it
        - putting a new fruit in place has a bug -> fix it
        - time it moves vertically or horizontally is different -> fix it
        - buttuns (timing is not perfect, still fails) -> fix it
###############################################################################################################
TETRIS 

DATA: 21/10/2023

	- Based on https://www.youtube.com/watch?v=8OK8_tHeCIA - for learning purposes;

	- Based on: https://github.com/OneLoneCoder/Javidx9/blob/master/SimplyCode/OneLoneCoder_Tetris.cpp

	- Run: g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
	       g++ -o main.exe main.cpp -static -std=c++17

	- Game timing - since computers arent the same, we need a way to make sure they are going to work similarly

	- In computing top-left is always {0, 0};

	- Tips to run: https://www.youtube.com/watch?v=jnI1gMxtrB4

TODO:
    - change pieces styles (it is confusing and might lead to errors)
    - show next piece
    - add music
    - add color 
    - add play/pause/game over functionalities


###############################################################################################################

FIRST PERSON SHOOTER

DATA: 15/10/2023

    First Person Shooter

    - Based on https://www.youtube.com/watch?v=xW8skO7MFYw and https://github.com/OneLoneCoder/CommandLineFPS 
    for learning purposes;

    - Compiling: g++ -o main.exe main.cpp

    - UNICODE: The Unicode Standard is the specification of an encoding scheme for written
      characters and text. It is a universal standard that enables consistent encoding of 
      multilingual text and allows text data to be interchanged internationally without conflict.
      https://www.ibm.com/docs/en/i/7.4?topic=set-unicode-standard-c-only

    - Based on https://pt.wikipedia.org/wiki/Wolfenstein_3D

    - Wolfenstein 3D: vide renderização ray casting gif;

    - Para rodar corretamente é necessário ajustar manualmente o tamanho da janela e definir 
      os valores de largura e altura nas propriedades do console (prompt);

    - Como o computador compartilha recursos com outras aplicações, não é possível saber com que
      frequência a tela será atualizada. Por isso, é necessário utiliza o tempo para computar os 
      frames e ter certeza que tudo será calculado seguindo um padrão;

    - Unicode - https://www.ssec.wisc.edu/~tomw/java/unicode.html

    - Várias lógicas matemática ainda precisa ser mais estudadas para melhor entender parted do
      algoritmo. Pincipalmente em relação aos ângulos.

TODO:
    - melhorar lógica do pequeno mapa (bug)
    - entender a lógica matemática
