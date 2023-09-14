#include "game.hpp"
#include "solver.hpp"
#include "altproj.hpp"
#include <regex>
using namespace std;

void play_again_prompt(int size, int nobs)
{
    std::string user_response;

    do{
        std::cout << "Bạn có muốn chơi lại không? y/n\n";
        std::getline(std::cin, user_response);
    } while ((user_response != "y") && (user_response != "n"));

    if (user_response == "y")
        play_game(size, nobs);
    return;
}

void play_game(int size, int nobs) 
{
    std::string user_entry;
    bool solverSucces = false;
    Board board = generate_puzzle(size, nobs);

    int input_int[3] = {0}; 

    do {
        board.print_puzzle();
        std::getline(std::cin, user_entry); 

        if (user_entry == "Solve" || user_entry == "solve") {
            solverSucces = solve(board, 0, 0);
            board.print_puzzle();
            break;
        }

        if (user_entry == "Clear" || user_entry == "clear") {
            board.clear_puzzle();
            continue;
        }

        int count = sscanf(user_entry.c_str(), "%d %d %d", &input_int[0], &input_int[1], &input_int[2]);

        if (count != 3) {
            std::cout << "Định dạng đầu vào không hợp lệ. Vui lòng nhập ba số nguyên cách nhau bởi dấu cách.\n";
            continue;
        }

        bool invalid = false;
        for (int i = 0; i < 3; i++) {
            if (!board.in_bounds(input_int[i])) {
                invalid = true;
                break;
            }
        }

        if (invalid) {
            std::cout << "Giá trị được nhập phải nằm trong khoảng từ 1 đến " << board.get_size() << std::endl;
            continue;
        }

        if (board.feasible_user(input_int[0] - 1, input_int[1] - 1, input_int[2])) {
            if (!board.check_immutable(input_int[0] - 1, input_int[1] - 1)) {
                board.assign_value(input_int[0] - 1, input_int[1] - 1, input_int[2]);
            } else {
                std::cout << "Bạn không thể thay đổi giá trị đó!\n";
            }
        } else {
            std::cout << "Không khả thi!\n";
        }

        for (int i = 0; i < 3; i++) {
            input_int[i] = 0;
        }

    } while (!board.check_puzzle());

    std::string user_response;

    if (solverSucces) {
        std::cout << "The puzzle is solved!\n";
        play_again_prompt(size, nobs);
    } else {
        std::cout << "Không thể giải câu đố dựa trên các giá trị được điền sẵn\n";
        do {
            std::cout << "Bạn có muốn làm rõ và cố gắng giải quyết? y/n\n";
            std::getline(std::cin, user_response);
        } while ((user_response != "y") && (user_response != "n"));

        if (user_response == "y") {
            board.clear_puzzle();
            solve(board, 0, 0);
            board.print_puzzle();
            play_again_prompt(size, nobs);
        }
    }
}