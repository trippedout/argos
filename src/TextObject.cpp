#include "TextObject.h"

TextObject::TextObject(std::string text, ci::Font* font, ci::ColorA color) :ImageView()
{
	textLayout.setFont(*font);
	textLayout.setColor(color);
	textLayout.addLine(text);

	setTexture(new ci::gl::Texture(textLayout.render(true))); 
}

TextObject::~TextObject(void)
{
	GLuint ids[1];
	ids[0] = texture->getId(); 
	
	glDeleteTextures(1, ids); 
}
