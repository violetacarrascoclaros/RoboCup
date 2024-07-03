#include "gtest/gtest.h"
#include "../functions.h"
#include"../estructuras.h"
#include "../CosteVSRecompensa.h"
#include <vector>
using namespace std;

// Parse strings tests

TEST(ParseInitialMessage, ParseInitialMessageTest)
{
    Player player;
    string message = "(init l 1 before_kick_off)";
    player = parseInitialMessage(message, player);
    EXPECT_EQ(player.side, "l");
    EXPECT_EQ(player.unum, 1);
}

TEST(CalcularCosteYRecompensaIrAPorLaPelotaTest, TestBasico) {
    Player player;
    player.stamina = 70;
    player.rol = "DELANTERO";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;

    Ball ball;
    ball.distance = 30;

    Goal goal;
    goal.distance = 50;

    double coste, recompensa;
    calcularCosteYRecompensaIrAPorLaPelota(player, ball, goal, coste, recompensa);

    // Aseguramos que los valores de coste y recompensa sean razonables
    EXPECT_GT(coste, 0.0);
    EXPECT_EQ(recompensa, 105);
    EXPECT_GT(recompensa, 0.0);
}

TEST (CalcularCosteYRecompensaIrAPorLaPelotaTest, TestFueraDeCampo)
{
    Player player;
    player.stamina = 70;
    player.rol = "DELANTERO";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = true;

    Ball ball;
    ball.distance = 30;

    Goal goal;
    goal.distance = 50;

    double coste, recompensa;
    calcularCosteYRecompensaIrAPorLaPelota(player, ball, goal, coste, recompensa);

    // Aseguramos que los valores de coste y recompensa sean razonables
    EXPECT_GT(coste, 0.0);
    EXPECT_GT(recompensa, 9999);
}

TEST(CalcularCosteYRecompensaPasarTest, TestBasico) {
    Player player;
    player.stamina = 70;
    player.rol = "DEFENSA";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;

    Ball ball;
    ball.distance = 20;

    Goal goal;
    goal.distance = 40;

    double coste, recompensa;
    calcularCosteYRecompensaPasar(player, ball, goal, coste, recompensa);

    // Aseguramos que los valores de coste y recompensa sean razonables
    EXPECT_GT(coste, 0.0);
    EXPECT_LT(recompensa, 100.0);
    EXPECT_GT(recompensa, 0.0);
}

TEST(CalcularCosteYRecompensaChutarTest, TestBasico) {
    Player player;
    player.stamina = 70;
    player.rol = "DELANTERO";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;

    Ball ball;
    ball.distance = 10;

    Goal goal;
    goal.distance = 30;

    double coste, recompensa;
    calcularCosteYRecompensaChutar(player, ball, goal, coste, recompensa);

    // Aseguramos que los valores de coste y recompensa sean razonables
    EXPECT_GT(coste, 0.0);
    EXPECT_EQ(recompensa, 105.0);
    EXPECT_GT(recompensa, 0.0);
}

TEST(CalcularCosteYRecompensaVolverAZonaTest, TestBasico) {
    Player player;
    player.stamina = 70;
    player.rol = "DEFENSA";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;

    Ball ball;
    ball.distance = 30;

    Goal goal;
    goal.distance = 50;

    double coste, recompensa;
    calcularCosteYRecompensaVolverAZona(player, ball, goal, coste, recompensa);

    // Aseguramos que los valores de coste y recompensa sean razonables
    EXPECT_GT(coste, 0.0);
    EXPECT_LT(recompensa, 100.0);
    EXPECT_GT(recompensa, 0.0);
}

// Ejemplo de test para obtener la mejor acción
TEST(ObtenerMejorAccionTest, TestIrAPorLaPelota) {
    Player player;
    player.stamina = 70;
    player.rol = "DELANTERO";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;

    Ball ball;
    ball.distance = 20;

    Goal goal;
    goal.distance = 40;

    int mejorAccion = obtenerMejorAccion(player, ball, goal);

    // Aseguramos que la mejor acción devuelta sea válida (0, 1, 2 o 3)

    EXPECT_EQ(mejorAccion, 0);
}

TEST(ObtenerMejorAccionTest, TestPasar) {
    Player player;
    player.stamina = 70;
    player.rol = "DEFENSA";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;

    Ball ball;
    ball.distance = 1;

    Goal goal;
    goal.distance = 40;

    int mejorAccion = obtenerMejorAccion(player, ball, goal);

    // Aseguramos que la mejor acción devuelta sea válida (0, 1, 2 o 3)

    EXPECT_EQ(mejorAccion, 1);
}

TEST(ObtenerMejorAccionTest, TestChutar) {
    Player player;
    player.stamina = 70;
    player.rol = "DELANTERO";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;

    Ball ball;
    ball.distance = 1;

    Goal goal;
    goal.distance = 30;

    int mejorAccion = obtenerMejorAccion(player, ball, goal);

    // Aseguramos que la mejor acción devuelta sea válida (0, 1, 2 o 3)

    EXPECT_EQ(mejorAccion, 2);
}
TEST (ObtenerMejorAccionTest, TestVolverAZona)
{
    Player player;
    player.stamina = 70;
    player.rol = "DEFENSA";
    player.see_ball = true;
    player.see_opponent_goal = true;
    player.OutOfField = false;
    player.distancia_a_zona= 50;

    Ball ball;
    ball.distance = 20;

    Goal goal;
    goal.distance = 50;

    int mejorAccion = obtenerMejorAccion(player, ball, goal);

    // Aseguramos que la mejor acción devuelta sea válida (0, 1, 2 o 3)

    EXPECT_EQ(mejorAccion, 3);
}

