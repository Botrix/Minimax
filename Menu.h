#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "../MyEngine/MainEngine/Window.h"
#include "../MyEngine/MainEngine/SpriteFont.h"
#include "../MyEngine/MainEngine/FPSLimiter.h"
#include "../MyEngine/MainEngine/GLSLProgram.h"
#include "../MyEngine/MainEngine/Camera2D.h"
#include "../MyEngine/MainEngine/Screen.h"

struct Button {
    bool                        isPressed(crvec2 mouse) const {
        return ((mouse.x >= bounds.x && mouse.x <= bounds.x+bounds[2]) &&
                (mouse.y <= bounds.y && mouse.y >= bounds.y-bounds[3]));
    }
    void                        draw(nta::SpriteFont* font, nta::SpriteBatch& batch) const {
        batch.addGlyph(bounds, glm::vec4(0,0,1,1), backgroundTexture, .5, backgroundColor);
        font->drawText(batch, name, bounds);
    }
    glm::vec4                   bounds;
    glm::vec4                   backgroundColor;
    std::string                 name;
    GLuint                      backgroundTexture;
};

class Menu : public nta::Screen {
private:
    nta::SpriteFont*            m_font = nullptr;
    nta::GLSLProgram*           m_simpleProg = nullptr;
    nta::SpriteBatch            m_hudBatch;
    nta::Camera2D               m_hudCamera;
    Button                      m_buttons[2];
public:
                                Menu();
                                ~Menu();
    void                        update();
    void                        render();
    void                        init();
};

#endif // MENU_H_INCLUDED
