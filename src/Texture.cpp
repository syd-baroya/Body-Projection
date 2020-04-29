#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Texture::initParams()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
	glTexImage2D(GL_TEXTURE_2D, 0, colortype, width, height, 0, colororder, datatype, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void TextureArray::initParams()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, colortype, images_width, images_height, num_images);
	if (num_images > 1)
		initSubImageParams();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magfilter);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void TextureArray::initSubImageParams() {
	std::vector<unsigned char> buffer((int)images_width * (int)images_height * 4 * (int)num_images);
	int sizepicoffset = 0;
	char txt[1000];
	for (int ii = 0; ii < 25; ii++)
	{
		sprintf(txt, filepath, ii + 1);
		data = stbi_load(txt, &width, &height, &channels, 4);
		if (data == NULL)
		{
			std::cout << "data is null" << std::endl;
			exit(1);
		}
		int sizepic = width * height * 4;
		memcpy(&buffer[sizepicoffset], data, sizepic);
		sizepicoffset = sizepic;
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			mipmapnumber,                     //Mipmap number
			0, 0, ii,                 //xoffset, yoffset, zoffset
			width, height, 1,                 //width, height, depth
			colororder,                //format
			datatype,      //type
			data);
	}
}

SimpleTexture2D::SimpleTexture2D(std::string file) : Texture() {
	strcpy(filepath, file.c_str());
	data = stbi_load(filepath, &width, &height, &channels, 4);
	wrap = GL_REPEAT;
}
void SimpleTexture2D::setTexName(const GLchar* name)
{
	this->tex_name = name;
}
void SimpleTexture2D::setFile(std::string file) {
	strcpy(filepath, file.c_str());
	data = stbi_load(filepath, &width, &height, &channels, 4);
}

void SimpleTexture2D::uploadToGPU(GLuint pid, int location_num)
{
	GLuint TexLoc = glGetUniformLocation(pid, tex_name);
	glUniform1i(TexLoc,location_num);
}
