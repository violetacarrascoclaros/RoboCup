#include "gtest/gtest.h"
#include "../functions.h"


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