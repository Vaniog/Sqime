#include "DynamicButtonCreatingUI.h"

DynamicButtonCreatingUI::DynamicButtonCreatingUI(int startXIn, int startYIn, int width)
{
    startX = startXIn;
    startY = startYIn;

    Image uiImage;
    uiImage.loadFromFile("images//ButtonCreatingUi.png");
    scale = (float)width / uiImage.getSize().x;

    float okButtonX = 102, okButtonY = 125, okButtonTexX = 16, okButtonTexY = 15;
    okButton = new Button(startX + okButtonX * scale, startY + okButtonY * scale, 0, 0, okButtonTexX, okButtonTexY, scale * okButtonTexX, scale * okButtonTexY, "images//OKButtonForDB.png");
    uiTexture.loadFromImage(uiImage);
    uiSprite.setTexture(uiTexture);
    uiSprite.setPosition(startX, startY);
    uiSprite.setScale(scale, scale);

    buttonMode = new ButtonsInf(39, 11, scale);
    buttonOff = new ButtonsInf(27, 27, scale);
    buttonOn = new ButtonsInf(27, 27, scale);
    buttonLength = new ButtonsInf(11, 11, scale);
    buttonDir = new ButtonsInf(11, 11, scale);

    buttonMode->addButton(43, 4), buttonMode->addButton(43, 16);
    buttonOff->addButton(4, 45), buttonOff->addButton(34, 45), buttonOff->addButton(64, 45), buttonOff->addButton(94, 45);
    buttonOn->addButton(4, 90), buttonOn->addButton(34, 90), buttonOn->addButton(64, 90), buttonOn->addButton(94, 90);
    buttonLength->addButton(4, 131), buttonLength->addButton(16, 131), buttonLength->addButton(28, 131);
    buttonDir->addButton(45, 131), buttonDir->addButton(57, 131), buttonDir->addButton(69, 131), buttonDir->addButton(81, 131);
}

int DynamicButtonCreatingUI::getValue(char valueName)
{
    switch(valueName)
    {
    case 'm':
        return buttonMode->value[0];
    case 'f':
        return buttonOff->value[0];
    case 'n':
        return buttonOn->value[0];
    case 'l':
        return buttonLength->value[0] + 1;
    case 'd':
        switch(buttonDir->value[0])
        {
        case 0:
            return 2;
        case 1:
            return 0;
        case 2:
            return 3;
        case 3:
            return 1;
        }
    }
}

DynamicButtonCreatingUI::~DynamicButtonCreatingUI()
{
    delete buttonMode;
    delete buttonOff;
    delete buttonOn;
    delete buttonDir;
    delete buttonLength;
    delete okButton;
}
int DynamicButtonCreatingUI::draw(RenderWindow *window)
{
    window->draw(uiSprite);
    int mouseX = (Mouse::getPosition().x - startX) / scale;
    int mouseY = (Mouse::getPosition().y - startY) / scale;
    int isPressed = Mouse::isButtonPressed(Mouse::Left);

    buttonMode->checkButtonDrawRect(mouseX, mouseY, isPressed, window, startX, startY);
    buttonOff->checkButtonDrawRect(mouseX, mouseY, isPressed, window, startX, startY);
    buttonOn->checkButtonDrawRect(mouseX, mouseY, isPressed, window, startX, startY);
    buttonLength->checkButtonDrawRect(mouseX, mouseY, isPressed, window, startX, startY);
    buttonDir->checkButtonDrawRect(mouseX, mouseY, isPressed, window, startX, startY);
    return okButton->buttonDisplayAndCheck(window, -1, -1);
}
