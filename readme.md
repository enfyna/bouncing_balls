<p align="center">
    <img width=20% src=https://github.com/user-attachments/assets/366ae7b2-4f47-4c5c-a945-806efc04835f>
</p>
<h1 align="center">
  Bouncing Balls
</h1>
<p align="center">
    <img src=https://img.shields.io/badge/raylib-ffffff.svg?style=for-the-badge&logo=c&logoColor=black&labelColor=white>
    <img src=https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white>
</p>
<br><br>
<p align="center">
    2D bouncing balls.
</p>
<p align="center">
    <img width=20% src=https://github.com/user-attachments/assets/560f3609-657c-43bc-8308-5783ed96d222 >
    <img width=20% src=https://github.com/user-attachments/assets/375a1430-e709-48dd-abb8-4490276b9365 >
    <img width=20% src=https://github.com/user-attachments/assets/e9e1d792-547a-4c88-8995-fec656716259 >
</p>
<br>

# Installation

1. Clone the repo.

        git clone https://github.com/enfyna/bouncing_balls
        cd bouncing_balls

> [!NOTE]
> If you have missing dependencies try to install them.
2. Build the project.

            make

    You can build for windows with:

            make win


5. Run

       ./build/main 


# Parameters

When you run the program from the command line you can pass these parameters: 

Square:

    square

Container Size:

    -cs <int>

Damping:

    -cd <float>

Line mode:

    -clm <int>

Gravity:

    -g <float>

Ball Padding:

    -bp <int>

Ball Size:

    -bs <int>

Ball Count:

    -bc <int>


# Develop

### Clangd

generate compile_commands.json:

    bear -- make
