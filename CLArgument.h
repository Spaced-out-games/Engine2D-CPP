#pragma once
#include "engineCore.h" // Assuming engineCore is defined in this header or include the correct header file for engineCore
#include <cstring> // For strncpy
#include <SDL.h> // For SDL_SetWindowTitle
#include <string> // For std::string

class engineCore; // Forward declaration to avoid circular dependency

class CLArg
{
public:
    CLArg(char* command, char* argument) : mCommand(command), mArgument(argument) {}
    CLArg(char* command) : mCommand(command), mArgument(nullptr) {}
    CLArg() : mCommand(nullptr), mArgument(nullptr) {}

    bool isValid() { return mCommand != nullptr; }
    bool hasArg() { return mArgument != nullptr; }

    void execute(engineCore* target_engine);

private:
    char* mCommand;
    char* mArgument;
    friend engineCore;
    void copyArgument();
};

#include "engineCore.h"

void CLArg::execute(engineCore* target_engine)
{
    // don't do anything with a non-existent command
    if (!isValid()) { return; }

    char* argument = nullptr;

    if (hasArg())
    {
        // Is the argument surrounded by quotes? If so, don't copy it to the new buffer
        if ((mArgument[0] == '"' && mArgument[strlen(mArgument) - 1] == '"') || (mArgument[0] == '\'' && mArgument[strlen(mArgument) - 1] == '\''))
        {
            size_t arg_length = strlen(mArgument) - 2;
            argument = (char*)malloc(arg_length + 1);
            if (argument)
            {
                strncpy(argument, mArgument + 1, arg_length);
                argument[arg_length] = '\0'; // Null-terminate the copied string
            }
        }
        else
        {
            size_t arg_length = strlen(mArgument);
            argument = (char*)malloc(arg_length + 1);
            if (argument)
            {
                strncpy(argument, mArgument, arg_length);
                argument[arg_length] = '\0'; // Null-terminate the copied string
            }
        }

        // Example command processing
        if (compare_s(mCommand, "title"))
        {
            if (argument)
            {
                std::string title(argument);
                // Set the window title
                SDL_SetWindowTitle(target_engine->sdlWindow, title.c_str());
            }
        }

        else if (compare_s(mCommand, "width"))
        {
            if (argument)
            {
                target_engine->mWidth = std::stoi(argument);
            }
        }

        free(argument);
    }
    else
    {
        // Example command without argument
        if (compare_s(mCommand, "fullscreen"))
        {
            // Toggle fullscreen
            if (target_engine->mFullscreen)
            {
                SDL_SetWindowFullscreen(target_engine->sdlWindow, 0); // Disable fullscreen
                target_engine->mFullscreen = false;
            }
            else
            {
                SDL_SetWindowFullscreen(target_engine->sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); // Enable fullscreen
                target_engine->mFullscreen = true;
            }
        }
        // Add more commands here...
    }
}

#define compare_s(command, test) (strncmp(command, test, strlen(mCommand)) == 0)