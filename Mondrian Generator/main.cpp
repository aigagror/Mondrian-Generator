
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <queue>
#include <chrono>
#include <thread>
// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"


//helper function for drawMondrian()
void drawMondrian(int startX, int endX, int startY, int endY, int scale, int lineThickness, int width, int height, sf::RenderWindow &window, std::queue<sf::RectangleShape> &drawQueue) {
    //decide whether or not to draw a line. The probability of drawing a line should be proportional to the area/scale (maybe raise it to a power of 1/n, where n is some positive integer)
    //if decide not to draw, decide whether to color that section in. Then, return. Else...
    
    //vary the line thickness a bit
    //decide whether it be vertical or horizontal, and whether it should traverse the whole screen or just the section (50-50 chance)
    
    //decide where to draw it (should be weighted a little more towards the middle)
    
    //call itself twice with new dimensions based on the split by the line
    int area = (endX - startX + 1) * (endY - startY + 1);
    double random = double(rand() % 1000) / 1000.0;
    double probability = double(area) / double(scale);
    probability = pow((probability < 0) ? (-1) * probability : probability, 1.0/3);
    std::cout << "random, probability: " << random << ", " << probability << std::endl;
    if (random > probability) {
        double colorProbability = double(rand() % 400) / 1000.0;
        double shouldColor = double(rand() % 1000) / 1000.0;
        if (shouldColor < colorProbability) {
            sf::RectangleShape colorBlock(sf::Vector2f(endX - startX + 1, endY - startY + 1));
            colorBlock.setPosition(startX, startY);
            int randomColor = rand() % 3;
            switch (randomColor) {
                case 0:
                    colorBlock.setFillColor(sf::Color(180, 0 , 0));
                    break;
                case 1:
                    colorBlock.setFillColor(sf::Color(180, 180, 0));
                    break;
                case 2:
                    colorBlock.setFillColor(sf::Color(0, 0, 180));
                    break;
                default:
                    break;
            }
            drawQueue.push(colorBlock);
            std::cout << "colored an area" << std::endl;
        }
        return;
    }
    
    int newLineThickness = lineThickness;
    double lineVariationPercentage = double(rand() % 25) / 100;
    int increaseOrDecrease = rand() % 3;
    if (increaseOrDecrease == 0) {
        newLineThickness = lineThickness + lineThickness * lineVariationPercentage;
        std::cout << "increased lineThickness by " << lineVariationPercentage << "%" << std::endl;
    }
    else  if (increaseOrDecrease == 1){
        newLineThickness = lineThickness - lineThickness * lineVariationPercentage;
        std::cout << "decreased lineThickness by " << lineVariationPercentage << "%" << std::endl;
    }
    
    std::cout << "newLineThickness: " << newLineThickness << std::endl;
    
    int verticalOrHorizontal = rand() % 2;
    int wholeLine = rand() % 2;
    int possibleLinePosition;
    if (verticalOrHorizontal == 0) {//vertical
        possibleLinePosition = endX - startX + 1 - lineThickness + 1;
    }
    else {//horizontal
        possibleLinePosition = endY - startY + 1 - lineThickness + 1;
    }
    
    if (possibleLinePosition < lineThickness) {//section is too small to fit the line
        return;
    }
    
    int randomPosition1 = rand() % possibleLinePosition;
    int randomPosition2 = rand() % possibleLinePosition;
    int randomPosition3 = rand() % possibleLinePosition;
    int randomPosition = std::max(std::min(randomPosition1,randomPosition2), std::min(std::max(randomPosition1,randomPosition2),randomPosition3));
    
    sf::RectangleShape line;
    if (verticalOrHorizontal == 0) {//vertical
        if (wholeLine == 0) {//traverse the whole piece
            line.setSize(sf::Vector2f(newLineThickness, height));
            line.setFillColor(sf::Color(0, 0, 0));
            line.setPosition(startX + randomPosition, 0);
        }
        else {//traverse only the section
            line.setSize(sf::Vector2f(newLineThickness, endY - startY + 1));
            line.setFillColor(sf::Color(0, 0, 0));
            line.setPosition(startX + randomPosition, startY);
        }
        drawQueue.push(line);
        
        drawMondrian(startX, startX + randomPosition - 1, startY, endY, scale, lineThickness, width, height, window, drawQueue);
        drawMondrian(startX + randomPosition + newLineThickness, endX, startY, endY, scale, lineThickness, width, height, window, drawQueue);
    }
    else {//horizontal
        if (wholeLine == 0) {//traverse the whole piece
            line.setSize(sf::Vector2f(width, newLineThickness));
            line.setFillColor(sf::Color(0, 0, 0));
            line.setPosition(0, startY + randomPosition);
        }
        else {//traverse just the section
            line.setSize(sf::Vector2f(endX - startX + 1, newLineThickness));
            line.setFillColor(sf::Color(0, 0, 0));
            line.setPosition(startX, startY + randomPosition);
        }
        drawQueue.push(line);
        
        drawMondrian(startX, endX, startY, startY + randomPosition - 1, scale, lineThickness, width, height, window, drawQueue);
        drawMondrian(startX, endX, startY + randomPosition + newLineThickness, endY, scale, lineThickness, width, height, window, drawQueue);
    }
}

void drawMondrian(int lineThickness, sf::RenderWindow &window) {
    std::queue<sf::RectangleShape> drawQueue;

    sf::RectangleShape background;
    background.setFillColor(sf::Color(210,210,210));
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setPosition(sf::Vector2f(0,0));
    drawQueue.push(background);
    
    drawMondrian(0, window.getSize().x - 1, 0, window.getSize().y - 1, window.getSize().x * window.getSize().y, lineThickness, window.getSize().x, window.getSize().y, window, drawQueue);
    
    int numberOfShapes = drawQueue.size();
    sf::RectangleShape *shapeArray = new sf::RectangleShape[numberOfShapes];
    for (int i = 0; i < numberOfShapes; i++) {
        shapeArray[i] = drawQueue.front();
        drawQueue.pop();
    }
    
    for (int i = 1; i <= numberOfShapes; i++) {
        window.clear();
        for (int j = 0; j < i; j++) {
            window.draw(shapeArray[j]);
        }
        window.display();
        std::this_thread::sleep_for (std::chrono::microseconds(100000));
    }
    sleep(2);
    delete[] shapeArray;
}

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "SFML window");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile(resourcePath() + "cute_image.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Sprite sprite(texture);
    
    sf::Sprite backgroundImage;
    backgroundImage.setScale(window.getSize().x, window.getSize().y);
    
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text drawingText("Drawing Composition of RYB", font, 50);
    drawingText.setColor(sf::Color::Black);
    
    sf::Text finishedText("Finished", font, 50);
    finishedText.setColor(sf::Color::Black);

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile(resourcePath() + "nice_music.ogg")) {
        return EXIT_FAILURE;
    }
    
    sf::RectangleShape line(sf::Vector2f(5, 150));
    line.setFillColor(sf::Color(0, 0, 0));
    line.setPosition(400, 300);

    // Play the music
    music.play();
    

    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        window.clear();
        drawMondrian(20, window);
    }

    return EXIT_SUCCESS;
}


