#include <glm/glm.hpp>

//Checking collision (square check), but possible to change by overriding TypeOfCollision function
class CollisionInterface{

    glm::vec2 up_left, down_right;

    protected:
    void setBounds(glm::vec2 ,glm::vec2 down_right){this->up_left = up_left; this->down_right = down_right;}

    public:
    enum TypeOfCollision{
        NO_COLLISION,
        COLLISION,
        DAMAGE,
        POINT_UP
    };
    virtual TypeOfCollision checkCollision(glm::vec2 up_left, glm::vec2 down_right);
};