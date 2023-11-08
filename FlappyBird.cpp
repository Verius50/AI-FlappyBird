#include <iostream>
#include <SFML/Graphics.hpp>
#include "AIbird.h"

using namespace sf;
Texture backgroundTX, birdTX, pipeTX;
Sprite background,* birds, pipes[4];
AIBird* AI;
Font font;
Text text("hello", font);


struct inputData {
    int data[3];
};

int main()
{
    bool rd;
    int nm = 200;
    std::cout << "Enter number of birds in popukation: ";
    std::cin >> nm;
    std::cout << "Read the best weights from file?(0/1): ";
    std::cin >> rd;

    RenderWindow window(sf::VideoMode(800, 600), "Flappy Bird!");
    window.setFramerateLimit(60);

    birds = new Sprite[nm];
    AI = new AIBird[nm];

    backgroundTX.loadFromFile("background.png");
    birdTX.loadFromFile("bird.png");
    pipeTX.loadFromFile("pipe.png");
    font.loadFromFile("arial.ttf");

    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    text.setPosition(10, 10);
    background.setTexture(backgroundTX);

    srand(unsigned int(time(0)));

    for (size_t i = 0; i < nm; i++)
    {
        if(rd)
            AI[i].InitBestWeights("bestWeights.txt");
        else AI[i].InitRand();
        
        AI[i].print();
        birds[i].setTexture(birdTX);
        birds[i].setPosition({ 300, 200 });
        birds[i].setOrigin({ 40, 30 });
    }

    srand(unsigned int(time(0)));
    for (size_t i = 0; i < 4; i++)
    {
        pipes[i].setTexture(pipeTX);
        pipes[i].setPosition(Vector2f(500 + i * 400, -350 + rand() % 300));
    }

    bool* pressed = new bool[nm];
    int* jump = new int[nm], * fall = new int[nm], * scores = new int[nm], maxScore = 0,
        maxScoreNum = 0, recordScore = 0, aliveNum = nm, generationNum = 1;
    inputData* data = new inputData[nm];



    for (size_t i = 0; i < nm; i++)
    {
        pressed[i] = 0; jump[i] = 20, fall[i] = 0, scores[i] = 0;
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (size_t i = 0; i < 4; i++)
        {
            if (pipes[i].getPosition().x < 320 && pipes[i].getPosition().x > 160)
                for (size_t j = 0; j < nm; j++)
                {
                    if (scores[j] > maxScore)
                    {
                        maxScore = scores[j];
                        if (maxScore / 100 > recordScore)
                            recordScore = maxScore / 100;
                    }
                    if (birds[j].getPosition().y < pipes[i].getPosition().y + 430 || birds[j].getPosition().y > pipes[i].getPosition().y + 615)
                    {
                        if (scores[j] >= maxScore)
                            maxScoreNum = int(j);

                        scores[j] = -1;
                    }

                }  
        }
        text.setString("Generation: " + std::to_string(generationNum) + "\nCurent score: " + std::to_string(maxScore / 100)
            + "\nBest score: " + std::to_string(recordScore) + "\nPopulation size: " + std::to_string(nm) + 
            "\nAlive: " + std::to_string(aliveNum));
        window.draw(background);
        window.draw(text);

        for (size_t i = 0; i < nm; i++)
            if (scores[i] != -1)
                window.draw(birds[i]);

        for (size_t i = 0; i < nm; i++)
        {
            if (jump[i] > 15)
            {
                if (fall[i] < 11)
                    birds[i].setRotation(-30 + fall[i] * 3);
                else birds[i].setRotation(fall[i] * 2);
                birds[i].move({ 0, 7 });
                if (fall[i] < 30)
                    fall[i]++;
            }
            else {
                birds[i].move({ 0, -10 });
                birds[i].setRotation(-jump[i] * 2);
                jump[i]++;
            }
        }
        for (size_t i = 0; i < 4; i++)
        {
            if (pipes[i].getPosition().x < -130) 
                pipes[i].setPosition(Vector2f(1400, -350 + rand() % 300));

            window.draw(pipes[i]);
            pipes[i].move({ -4, 0 });
        }
        for (size_t i = 0; i < nm; i++)
        {
            if (scores[i] != -1)
            {

                for (int j = 0; j < 4; j++)
                {
                    if (pipes[j].getPosition().x < 550 && pipes[j].getPosition().x > 150) {
                        data[i].data[0] = birds[i].getPosition().y - pipes[j].getPosition().y + 430;
                        data[i].data[1] = pipes[j].getPosition().y + 615 - birds[i].getPosition().y;
                        data[i].data[2] = pipes[j].getPosition().x - birds[i].getPosition().x;
                    }
                }

                if (AI[i].InputData(data[i].data))
                {
                    jump[i] = 0; fall[i] = 0;
                }
            }
        }

        aliveNum = nm;
        for (size_t i = 0; i < nm; i++)
        {
            if (birds[i].getPosition().y > 600 )
                scores[i] = -1;
            if (birds[i].getPosition().y < 15)
                birds[i].setPosition(300, 15);
            if (scores[i] == -1)
                aliveNum--;
        }



        for (size_t i = 0; i < nm; i++) {
            if (scores[i] != -1)
                scores[i]++;
        }

        if (aliveNum == 0) {
            AI[maxScoreNum].SaveWeights();
            for (size_t i = 0; i < nm; i++)
            {
                AI[i].InitBestWeights("weights.txt");
                pressed[i] = 0; jump[i] = 20, fall[i] = 0, scores[i] = 0;
                for (size_t i = 0; i < 4; i++)
                    pipes[i].setPosition(Vector2f(500 + i * 400, -350 + rand() % 300));
                birds[i].setPosition(Vector2f(300, 200));
            }
            maxScore = 0;
            aliveNum = nm;
            generationNum++;
        }

        
        window.draw(text);
        window.display();
        
    }
}
