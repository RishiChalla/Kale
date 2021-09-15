/*
   Copyright 2021 Rishi Challa

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#ifdef ISLANDS_SDL

/**
 Forward declaration of the SDL Window
 */
struct SDL_Window;

namespace Islands {

    class Window {
    private:
        
        /**
         * A pointer to the main sdl window
         */
        SDL_Window* window;
        
        /**
         * Whether or not the window is open
         */
        bool open = false;
        
    protected:
        
        /**
         * Clears the screen for rendering the next frame
         */
        void clear();
        
        /**
         * Renders everything placed in the frame buffer
         */
        void render();
        
        friend class Application;
//        friend class Scene;
        
    public:
        
        /**
         * Initializes SDL
         */
        Window();
        
        /**
         * Frees resources of the window
         */
        ~Window();
        
        /**
         * Creates a new window. Some Operating systems (IOS/Android/TvOS) only support the creation
         * of a single window
         * @param title The title of the window
         */
        void create(const char* title);
        
        /**
         * Thread safe method to check whether or not the window is currently open
         * @returns Whether or not the window is open
         */
        bool isOpen() const;
        
    };
}

#endif
