#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

struct State
{
    bool *cells;
    short cellsCount;
};

void initState(State &state, short cellsCount, bool initialValue) noexcept;
void nextState(State &oldState, State &newState) noexcept;
void drawState(State &state) noexcept;

HANDLE hConsole;

int main(int argc, char const *argv[])
{

    //console preparing
    SetConsoleTitle("RULE 160");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    PCONSOLE_SCREEN_BUFFER_INFO psbi;
    GetConsoleScreenBufferInfo(hConsole, psbi);
    SMALL_RECT consoleSizes = psbi->srWindow;

    //data preparing
    State states[2];
    initState(states[0], consoleSizes.Right - consoleSizes.Left / 18, false);
    initState(states[1], consoleSizes.Right - consoleSizes.Left / 18, false);

    states[0].cells[states[0].cellsCount / 2] = true;

    int fg = 0;

    drawState(states[fg]);

    while (true)
    {
        int bg = 1 - fg;

        nextState(states[fg], states[bg]);

        fg = bg;

        drawState(states[fg]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    delete states[0].cells;
    delete states[1].cells;
}

void initState(State &state, short cellsCount, bool initialValue) noexcept
{
    state.cellsCount = cellsCount;

    state.cells = new bool[state.cellsCount];

    for (short i = 0; i < state.cellsCount; i++)
    {
        state.cells[i] = false;
    }
}

//only for true states
void nextState(State &oldState, State &newState) noexcept
{
    for (short i = 1; i < oldState.cellsCount - 1; i++)
    {

        if (oldState.cells[i - 1] && oldState.cells[i] && oldState.cells[i + 1])
        {
            newState.cells[i] = true;
        }
        else if (oldState.cells[i - 1] && !oldState.cells[i] && oldState.cells[i + 1])
        {
            newState.cells[i] = true;
        }
        else if (!oldState.cells[i - 1] && !oldState.cells[i] && !oldState.cells[i + 1])
        {
            newState.cells[i] = true;
        }
        else
        {
            newState.cells[i] = false;
        }
    }
}

void drawState(State &state) noexcept
{

    for (short i = 0; i < state.cellsCount; i++)
    {

        if (state.cells[i])
        {
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_BLUE | COMMON_LVB_TRAILING_BYTE);
            std::cout << " ";
        }
        else
        {
            // SetConsoleTextAttribute(hConsole, 2);
            SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | COMMON_LVB_TRAILING_BYTE);
            std::cout << " ";
        }
    }
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | COMMON_LVB_TRAILING_BYTE);
    std::cout << std::endl;
}