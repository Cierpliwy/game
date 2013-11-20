#ifndef GAME_EXCEPTION
#define GAME_EXCEPTION
#include <exception>
#include <string>

class GameException : public std::exception
{
public:
    enum ModuleType {
        SDL,
        SDL_IMAGE,
        GLEW,
        GL,
        INTERNAL
    };

    GameException(ModuleType type, const char* msg = NULL, int value = 0);
    virtual ~GameException() throw() {}
    const char* what() const throw() { return m_msgPtr; }

private:
    std::string m_msg;
    const char *m_msgPtr;
};

#endif //GAME_EXCEPTION
