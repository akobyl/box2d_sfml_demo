#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


namespace Physics {
    const auto WIDTH = 1920;
    const auto HEIGHT = 1080;
    const auto SCALE = 30.0f;

    // SFML -> Box2D
    inline b2Vec2 pixelsToMeters(sf::Vector2f pos) {
        return {pos.x / SCALE, (HEIGHT - pos.y) / SCALE};
    }

    // Box2D -> SFML
    inline sf::Vector2f metersToPixels(b2Vec2 pos) {
        return {pos.x * SCALE, HEIGHT - pos.y * SCALE};
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto window = sf::RenderWindow({1920u, 1080u}, "PhysicsDemo");
    window.setFramerateLimit(60);
    auto clock = sf::Clock();

    // Box2D world
    // A world needs a gravity vector
    b2Vec2 gravity = {0.0f, -10.0f};
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = gravity;

    // WorldID is a handle passed to all shapes when created
    b2WorldId worldId = b2CreateWorld(&worldDef);

    // Ground body
    // Create body def
    // Create body id
    // Create polygon shape
    // create shape def
    // create the polygon shape and add it to the body id
    const float boxWidth = 20.0f;
    const float boxHeight = 1.0f;
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    // The origin of a b2Box is at the geometric center
    groundBodyDef.position = {30.0f, 5.0f};
    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
    // b2MakeBox creates vertices at +- width and height, so the total size needs to be halved
    b2Polygon groundBox = b2MakeBox(boxWidth / 2, boxHeight / 2);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    // Ground body visual
    sf::RectangleShape groundShape;
    groundShape.setSize({boxWidth * Physics::SCALE, boxHeight * Physics::SCALE});
    groundShape.setFillColor(sf::Color::Green);
    auto origin = Physics::metersToPixels(groundBodyDef.position);
    groundShape.setOrigin(groundShape.getSize().x / 2, groundShape.getSize().y / 2);
    groundShape.setPosition(origin);


    // Ball body
    const float radius = 1.0f;
    b2BodyDef ballBodyDef = b2DefaultBodyDef();
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position = {30.0f, 15.0f};
    b2BodyId ballId = b2CreateBody(worldId, &ballBodyDef);
    b2ShapeDef ballShapeDef = b2DefaultShapeDef();
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.3f;
    ballShapeDef.restitution = 0.7f;
    b2Circle ballCircle = {0.0f, 0.0f, radius};
    b2CreateCircleShape(ballId, &ballShapeDef, &ballCircle);


    // Ball Visual
    sf::CircleShape ball;
    ball.setRadius(radius * Physics::SCALE);
    ball.setFillColor(sf::Color::Red);
    auto ballPosition = Physics::metersToPixels(b2Body_GetPosition(ballId));
    ball.setOrigin(radius * Physics::SCALE, radius * Physics::SCALE);
    ball.setPosition(ballPosition);

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        b2World_Step(worldId, 1.0f / 60.0f, 4);

        auto events = b2World_GetBodyEvents(worldId);
        for (auto i = 0; i != events.moveCount; ++i) {
            auto event = events.moveEvents[i];
            auto rawBallPosition = event.transform.p;
            ballPosition = Physics::metersToPixels(rawBallPosition);
            ball.setPosition(ballPosition);
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto mousePosition = sf::Mouse::getPosition(window);

                    const float pushForce = 1000.0f;
                    float dx = ballPosition.x - mousePosition.x;
                    float dy = ballPosition.y - mousePosition.y;
                    float length = sqrt(dx * dx + dy * dy);
                    b2Vec2 pushVector = {pushForce * dx / length, -pushForce * dy / length};
                    b2Body_ApplyForceToCenter(ballId, pushVector, false);
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    auto mousePosition = sf::Mouse::getPosition(window);
                    auto ballPosition = Physics::pixelsToMeters({(float) mousePosition.x, (float) mousePosition.y});
                    b2Body_SetTransform(ballId, ballPosition, b2Body_GetRotation(ballId));
                    b2Body_SetLinearVelocity(ballId, {0.0f, 0.0f});
                    b2Body_SetAngularVelocity(ballId, 0.0f);
                }
            }
        }

        window.clear();
        window.draw(groundShape);
        window.draw(ball);
        window.display();
    }

    b2DestroyWorld(worldId);
    return 0;
}