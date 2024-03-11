/*
    Ping Pong Version 2.0
    Author : Kadda Aoues
    Date   : 3 / 11 /2024
*/
#include "pingpong_v_2.h"
#include "MyLib/chrono/Game_timer.h"
#include "MyLib/random_generator.h"

template<typename F>
void execute_if_key_released(int key, F  f) {
    if (KeyReleased(key)) {
        f();
    }
}

int main() {
    // console global variable 
    int g_lx = 145;
    int g_ly = 35;

    int g_fieldx = 45;
    int g_fieldy = 34;
      
    int id{};

    int y_extLine1 = 3;
    int y_extLine2 = 31;

    std::wstring    player_name1{ L"Adem" };
    std::wstring    player_name2{ L"Arwa" };

    std::wstring     wstrPlayer1Title = L"Player 1{" + player_name1 + L"}";
    std::wstring     wstrPlayer1Data;

    std::wstring     wstrPlayer2Title = L"Player 2{" + player_name2 + L"}";
    std::wstring     wstrPlayer2Data;
    std::wstring     wstrBallData;

    cgu::iRect  field_rect{ 1,1,g_fieldx + 1, g_fieldy + 1 };

    cgu::console.construct_console(g_lx, g_ly);

    // object initialization

    std::shared_ptr<Ball> ptr = std::make_shared<Ball>();

    Ball    ball;
    ball.motion.set_bounds(2, 2, g_fieldx-2, g_fieldy-1);
    ball.motion.set_speed(0.2f, 0.2f);


    int length_palette = 8;
    std::wstring      strPlayer(length_palette, 0x2588);
    cgu::DynSprite    player1{strPlayer, length_palette ,1, 4, y_extLine1 + 1};
    cgu::DynSprite    player2{strPlayer, length_palette ,1, 4, y_extLine2 - 1};

    PlayerMove        move1;
    move1.set_speed(0.7f, 0.7f);
    move1.set_bounds(1,1,g_fieldx - length_palette, g_fieldy);
    PlayerMove        move2;
    move2.set_speed(0.7f, 0.7f);
    move2.set_bounds(1,1,g_fieldx - length_palette, g_fieldy);
    move2.set_controle_direction_key({0,0, _u('D'), _u('A')});

    // Menu definition setup
    std::wstring      wstrMenuTail = LR"(M:Menu ESC:Quite U:incr I:decr)";
    std::wstring      wstrMenuTitle = L"Menu";
    std::wstring      wstrMenuBody = LR"(M: Menu
U: Increase Ball speed
I: Decrease Ball speed
O: Reset score 0,0
R: Reset Ball
ESC: Quite Game)";


    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Main Loop  
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    Time::Game_Timer   timer;
    Time::Game_Timer   ball_timer;
    bool               b_pause{ false };
    cgu::Dt60          = 1.5f;

    int                score1{};
    int                score2{};
    int                number_ball{};
    float              ball_times{};
    float              higher_time{};

    auto reset_ball = [&] {
        rng::fRG<float> frand;
        rng::iRG<int>   irand;
        ball.set_position(frand(10.f, 30.f), frand(10.f, 17.f));
        ball.reverse(irand(0, 2));
        // ball time
        ball_times = ball_timer.get_elapsed_time();
        ball_timer.reset();
        higher_time = ball_times > higher_time ? ball_times : higher_time;
        };


    while (true) {
        // header timer, frame calculation, title
        cgu::Dtime = cgu::Dt60 * 60.f * cgu::elps;

        timer.reset();
        swprintf_s(cgu::title, 255, L"Fps[%5.2f]" , cgu::fps );
        TITLE(cgu::title);


        // window clear
        cgu::console.clear_screen();

        // ---------------------------- Data Of Game -------------------------------------- //

        memset(cgu::title, 0, 255);
        swprintf_s(cgu::title, 255, L"Name : %s\nSore : %d", player_name1.c_str(), score1);
        cgu::message_box(g_fieldx, 2, wstrPlayer1Title, cgu::title); 

        memset(cgu::title, 0, 255);
        swprintf_s(cgu::title, 255, L"Name : %s\nSore : %d", player_name2.c_str(), score2);
        cgu::message_box(g_fieldx, 29, wstrPlayer2Title, cgu::title);

        cgu::progress_bar(player_name1 ,g_fieldx, 10, 20, score1);
        cgu::progress_bar(player_name2, g_fieldx, 11, 20, score2);

        memset(cgu::title, 0, 255);
        swprintf_s(cgu::title, 255, L"Last Time : %3.2f\nHigher Time : %3.2f",
            ball_times, higher_time);
        cgu::message_box(g_fieldx, 15, L"Ball Data", cgu::title);



        // Menu affich
        if (KeyReleased(_u('M'))) {
            b_pause = !b_pause;
        }
        if (b_pause) {
            cgu::message_box(20 + g_lx / 2, g_ly / 2, wstrMenuTitle, wstrMenuBody);
            goto __next;
        }

        // Generation and Selection
        if (KeyReleased(_u('B'))) {

        }
        if (KeyReleased(_u('U'))) {
            cgu::fPoint2d v = ball.motion.get_speed();
            ball.motion.set_speed(v.x + 0.01f, v.y + 0.01f);
        }
        if (KeyReleased(_u('I'))) {
            cgu::fPoint2d v = ball.motion.get_speed();
            ball.motion.set_speed(v.x - 0.01f, v.y - 0.01f);
        }

        execute_if_key_released(_u('R'), [&] {
            reset_ball();
            });

        execute_if_key_released(_u('O'), [&] {
            score1 = 0 ;
            score2 = 0 ;
            });



        // Cinematics And controle
        ball.move(cgu::Dtime);
        move1(&player1, cgu::Dtime);
        move2(&player2, cgu::Dtime);

        // Collision ---------------------------------------------
        // 1. reset the ball if get out of extrem line y_ext

        if (cgu::is_collid(player1,ball)) {
            ball.reverse(2);
        }

        if (cgu::is_collid(player2, ball)) {
            ball.reverse(2);
        }

        if (ball.get_position().y > y_extLine2  ) {
            reset_ball();
            ++score1;
            //std::this_thread::sleep_for(std::chrono::seconds(3));
        }
          
		if (ball.get_position().y < y_extLine1 + 1.f) {
            reset_ball();
            ++score2;
            //std::this_thread::sleep_for(std::chrono::seconds(3));
		}


    __next:
        // drawing
        ball.draw();
        player1.draw();
        player2.draw();

        // console display
        cgu::draw_hline(y_extLine1,  2, g_fieldx - 1, cgu::whLine);
        cgu::draw_hline(y_extLine2,  2, g_fieldx - 1, cgu::whLine);

        cgu::draw_rectangle({ 1,1, g_fieldx - 2, g_fieldy - 2 }, 0x2592);
        cgu::draw_rectangle({ 0, 0, g_lx - 1, g_ly - 1 });
        cgu::print_text_at(wstrMenuTail, 1, 34);
        cgu::console.display();

        // Quite by escape
        if (cgu::get_out()) break;
        // calculate time frame per second
        cgu::elps = timer.get_elapsed_time();
        cgu::fps = 1.f / cgu::elps;

    }  // End Of Loop


    
    std::cout << "End of Ping Pong \n";
    return 0;
}

