#include "NodeActor.h"
#include "Input.h"
#include <string>

NodeActor::NodeActor(int data, Vector2 position, NodeActor* parent)
	: Actor(std::to_string(data).c_str(), Color::White, position),
    parent(parent),
    data(data)
{
    // ±ôºýÀÌ´Â ÁÖ±â ¼³Á¤ÇÏ´Â Å¸ÀÌ¸Ó
    timer.Reset();
    timer.SetTargetTime(0.1f);
}

void NodeActor::Tick(float deltaTime)
{
    // ±ôºýÀÌ´Â È¿°ú
    if (blink)
    {
        timer.Tick(deltaTime);

        if (timer.IsTimeout())
        {
            code = (code + 1) % 4;
            ColorChange();
            timer.Reset();
            blink++;
            if (blink == 13)
            {
                blink = 0;
                code = 0;
                color = Color::White;
            }
        }
    }

}

void NodeActor::Render()
{
    super::Render();
}

void NodeActor::ColorChange()
{
    switch (code)
    {
        case 0 :
            color = Color::White;
            break;
        case 1:
            color = Color::RedIntensity;
            break;
        case 2:
            color = Color::Red;
            break;
        case 3:
            color = Color::RedIntensity;
            break;
    }
}

void NodeActor::Blink()
{
    blink++;
}
